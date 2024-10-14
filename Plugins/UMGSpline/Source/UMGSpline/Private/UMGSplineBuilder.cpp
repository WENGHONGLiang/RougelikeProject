// Copyright Qibo Pang 2022. All Rights Reserved.

/*
* Below Code mainly copy from SlateCore / ElementBatcher.cpp FLineBuilder, I made a few changes to support:
* 1. Add uv information for spline geometry
* 2. Made continuous spline geometry
* 3. Calculate spline length when build spline geometry
*/

#include "UMGSplineBuilder.h"
#include "UMGSplineRenderBatch.h"

const static float      CONST_SplineVScale = 0.01f;

// Will append 5 vertexes to OutBatchVertices and 9 indexes to
	// OutBatchIndices. Creates the following cap geometry:
	//
	// Cap Vertex Indexes              Cap Measurements
	//
	//     U == 0
	//   2-4----                        2-------4-------....
	//   |\| 							|       |     ^ 
	//   | 0  <-- U==0.5				|<- d ->o    2h  
	//   |/|							|       |     v
	//   1-3----						1-------3-------....
	//     U == 0
	//                                 d is CapDirection
	//                                 h is Up
	//                                 o is CapOrigin

void FUMGLineBuilder::MakeCap(
	const FVector2D& CapOrigin,
	const FVector2D& CapDirection,
	const FVector2D& Up,
	const FColor& Color
)
{
	const uint32 FirstVertIndex = RenderBatch.GetNumVertices();

	float CapLength = FVector2D::Distance(CapDirection, FVector2D::ZeroVector) * CONST_SplineVScale;
	
	bool bStartSideCap = (FirstVertIndex == 0);
	float CapStartVCoord, CapEndVCoord;
	if (bStartSideCap)
	{
		CapStartVCoord = CapLength * CONST_SplineVScale;
		CapEndVCoord = 0.0f;
	}
	else
	{
		CapStartVCoord = SplineVCoord;
		CapEndVCoord = SplineLength + CapLength * CONST_SplineVScale;
	}

	SplineLength += CapLength;
	SplineVCoord += CapLength * CONST_SplineVScale * CustomVertsVCoordScale;

	RenderBatch.AddVertex(FSlateVertex::Make<ESlateVertexRounding::Disabled>(RenderTransform, FVector2f(CapOrigin), FVector2f(0.5f, CapStartVCoord), FVector2f(Thickness, FilterScale), Color));
	RenderBatch.AddVertex(FSlateVertex::Make<ESlateVertexRounding::Disabled>(RenderTransform, FVector2f(CapOrigin + CapDirection + Up), FVector2f(1.0f, CapEndVCoord), FVector2f(Thickness, FilterScale), Color));
	RenderBatch.AddVertex(FSlateVertex::Make<ESlateVertexRounding::Disabled>(RenderTransform, FVector2f(CapOrigin + CapDirection - Up), FVector2f(0.0f, CapEndVCoord), FVector2f(Thickness, FilterScale), Color));
	RenderBatch.AddVertex(FSlateVertex::Make<ESlateVertexRounding::Disabled>(RenderTransform, FVector2f(CapOrigin + Up), FVector2f(1.0f, CapEndVCoord), FVector2f::ZeroVector, Color));
	RenderBatch.AddVertex(FSlateVertex::Make<ESlateVertexRounding::Disabled>(RenderTransform, FVector2f(CapOrigin - Up), FVector2f(0.0f, CapEndVCoord), FVector2f::ZeroVector, Color));

	RenderBatch.AddIndex(FirstVertIndex + 0);
	RenderBatch.AddIndex(FirstVertIndex + 3);
	RenderBatch.AddIndex(FirstVertIndex + 1);

	RenderBatch.AddIndex(FirstVertIndex + 0);
	RenderBatch.AddIndex(FirstVertIndex + 1);
	RenderBatch.AddIndex(FirstVertIndex + 2);

	RenderBatch.AddIndex(FirstVertIndex + 0);
	RenderBatch.AddIndex(FirstVertIndex + 2);
	RenderBatch.AddIndex(FirstVertIndex + 4);
}

void FUMGLineBuilder::BuildBezierGeometry_WithColorGradient(const TArray<FSlateGradientStop>& GradientStops, int32 GradientStopIndex, const FVector2D& P0, const FVector2D& P1, const FVector2D& P2, const FVector2D& P3, const FSlateElementBatcher& InBatcher)
{
	const int32 NumGradientStops = GradientStops.Num();
	const float SubdivisionPoint = 1.0f / (NumGradientStops - GradientStopIndex);

	if (GradientStopIndex < NumGradientStops - 1)
	{
		FVector2D TwoCurves[7];
		deCasteljauSplit_WithColorGradient(P0, P1, P2, P3, TwoCurves, SubdivisionPoint);
		Subdivide_WithColorGradient(GradientStops[GradientStopIndex - 1].Color, GradientStops[GradientStopIndex].Color, InBatcher, TwoCurves[0], TwoCurves[1], TwoCurves[2], TwoCurves[3], *this, 1.0f);
		BuildBezierGeometry_WithColorGradient(GradientStops, GradientStopIndex + 1, TwoCurves[3], TwoCurves[4], TwoCurves[5], TwoCurves[6], InBatcher);
	}
	else
	{
		// We have reached the last gradient stop, so we can finish this spline.
		Subdivide_WithColorGradient(GradientStops[GradientStopIndex - 1].Color, GradientStops[GradientStopIndex].Color, InBatcher, P0, P1, P2, P3, *this, 1.0f);
		//Finish(P3, InBatcher.PackVertexColor(GradientStops[GradientStopIndex].Color));
	}

}

void FUMGLineBuilder::BuildBezierGeometry(const FVector2D& P0, const FVector2D& P1, const FVector2D& P2, const FVector2D& P3)
{
	Subdivide(P0, P1, P2, P3, *this, 1.0f);
	//Finish(P3, SingleColor);
}

void FUMGLineBuilder::Finish(const FVector2D& LastPoint, const FColor& InColor, bool bCloseLoop)
{
	if (NumPointsAdded < 3)
	{
		// Line builder needs at least two line segments (3 points) to
		// complete building its geometry.
		// This will only happen in the case when we have a straight line.
		AppendPoint(LastPoint, InColor);
	}
	else
	{
		// We have added the last point, but the line builder only builds
		// geometry for the previous line segment. Build geometry for the
		// last line segment.
		const FVector2D LastUp = LastNormal * HalfLineThickness;

		SplineLength += FVector2D::Distance(LastPointAdded[1], LastPointAdded[0]);
		SplineVCoord = SplineLength * CONST_SplineVScale * CustomVertsVCoordScale;

		OnSegmentAdded.ExecuteIfBound(NumPointsAdded - 2, LastPointAdded[0], SplineLength);

		if (bCloseLoop)
		{
			FSlateRenderTransform TempRenderTransform = FSlateRenderTransform(1.0f);
			RenderBatch.AddVertex(FSlateVertex::Make<ESlateVertexRounding::Disabled>(TempRenderTransform, (*RenderBatch.SourceVertices)[0].Position, FVector2f(1.0f, SplineVCoord), FVector2f(Thickness, FilterScale), InColor));
			RenderBatch.AddVertex(FSlateVertex::Make<ESlateVertexRounding::Disabled>(TempRenderTransform, (*RenderBatch.SourceVertices)[1].Position, FVector2f(0.0f, SplineVCoord), FVector2f(Thickness, FilterScale), InColor));
		}
		else
		{
			RenderBatch.AddVertex(FSlateVertex::Make<ESlateVertexRounding::Disabled>(RenderTransform, FVector2f(LastPointAdded[0] + LastUp), FVector2f(1.0f, SplineVCoord), FVector2f(Thickness, FilterScale), InColor));
			RenderBatch.AddVertex(FSlateVertex::Make<ESlateVertexRounding::Disabled>(RenderTransform, FVector2f(LastPointAdded[0] - LastUp), FVector2f(0.0f, SplineVCoord), FVector2f(Thickness, FilterScale), InColor));
		}

		const int32 NumVerts = RenderBatch.GetNumVertices();

		// Counterclockwise winding on triangles
		RenderBatch.AddIndex(NumVerts - 3);
		RenderBatch.AddIndex(NumVerts - 4);
		RenderBatch.AddIndex(NumVerts - 2);

		RenderBatch.AddIndex(NumVerts - 3);
		RenderBatch.AddIndex(NumVerts - 2);
		RenderBatch.AddIndex(NumVerts - 1);
	}
}

void FUMGLineBuilder::AppendPoint(const FVector2D NewPoint, const FColor& InColor)
{
	// We only add vertexes for the previous line segment.
	// This is because we want to average the previous and new normals
	// In order to prevent overlapping line segments on the spline.
	// These occur especially when curvature is high.

	const FVector2D NewNormal = FVector2D(LastPointAdded[0].Y - NewPoint.Y, NewPoint.X - LastPointAdded[0].X).GetSafeNormal();

	if (NumPointsAdded == 2)
	{
		// Once we have two points, we have a normal, so we can generate the first bit of geometry.
		const FVector2D LastUp = LastNormal * HalfLineThickness;

		OnSegmentAdded.ExecuteIfBound(NumPointsAdded - 2, LastPointAdded[0], SplineLength);

		RenderBatch.AddVertex(FSlateVertex::Make<ESlateVertexRounding::Disabled>(RenderTransform, FVector2f(LastPointAdded[1] + LastUp), FVector2f(1.0f, SplineVCoord), FVector2f(Thickness, FilterScale), InColor));
		RenderBatch.AddVertex(FSlateVertex::Make<ESlateVertexRounding::Disabled>(RenderTransform, FVector2f(LastPointAdded[1] - LastUp), FVector2f(0.0f, SplineVCoord), FVector2f(Thickness, FilterScale), InColor));
	}
	
	if (NumPointsAdded >= 2)
	{
		const FVector2D AveragedUp = (0.5f * (NewNormal + LastNormal)).GetSafeNormal() * HalfLineThickness;

		SplineLength += FVector2D::Distance(LastPointAdded[1], LastPointAdded[0]);
		SplineVCoord = SplineLength * CONST_SplineVScale * CustomVertsVCoordScale;
		
		OnSegmentAdded.ExecuteIfBound(NumPointsAdded - 2, LastPointAdded[0], SplineLength);

		RenderBatch.AddVertex(FSlateVertex::Make<ESlateVertexRounding::Disabled>(RenderTransform, FVector2f(LastPointAdded[0] + AveragedUp), FVector2f(1.0f, SplineVCoord), FVector2f(Thickness, FilterScale), InColor));
		RenderBatch.AddVertex(FSlateVertex::Make<ESlateVertexRounding::Disabled>(RenderTransform, FVector2f(LastPointAdded[0] - AveragedUp), FVector2f(0.0f, SplineVCoord), FVector2f(Thickness, FilterScale), InColor));

		const int32 NumVerts = RenderBatch.GetNumVertices();

		// Counterclockwise winding on triangles
		RenderBatch.AddIndex(NumVerts - 3);
		RenderBatch.AddIndex(NumVerts - 4);
		RenderBatch.AddIndex(NumVerts - 2);

		RenderBatch.AddIndex(NumVerts - 3);
		RenderBatch.AddIndex(NumVerts - 2);
		RenderBatch.AddIndex(NumVerts - 1);
	}

	LastPointAdded[1] = LastPointAdded[0];
	LastPointAdded[0] = NewPoint;
	LastNormal = NewNormal;

	++NumPointsAdded;
}


/**
* Based on comp.graphics.algorithms: Adaptive Subdivision of Bezier Curves.
*
*   P1 + - - - - + P2
*     /           \
* P0 *             * P3
*
* In a perfectly flat curve P1 is the midpoint of (P0, P2) and P2 is the midpoint of (P1,P3).
* Computing the deviation of points P1 and P2 from the midpoints of P0,P2 and P1,P3 provides
* a simple and reliable measure of flatness.
*
* P1Deviation = (P0 + P2)/2 - P1
* P2Deviation = (P1 + P3)/2 - P2
*
* Eliminate divides: same expression but gets us twice the allowable error
* P1Deviation*2 = P0 + P2 - 2*P1
* P2Deviation*2 = P1 + P3 - 2*P2
*
* Use manhattan distance: 2*Deviation = |P1Deviation.x| + |P1Deviation.y| + |P2Deviation.x| + |P2Deviation.y|
*
*/
float FUMGLineBuilder::ComputeCurviness(const FVector2D& P0, const FVector2D& P1, const FVector2D& P2, const FVector2D& P3)
{
	FVector2D TwoP1Deviations = P0 + P2 - 2 * P1;
	FVector2D TwoP2Deviations = P1 + P3 - 2 * P2;
	float TwoDeviations = FMath::Abs(TwoP1Deviations.X) + FMath::Abs(TwoP1Deviations.Y) + FMath::Abs(TwoP2Deviations.X) + FMath::Abs(TwoP2Deviations.Y);
	return TwoDeviations;
}


/**
* deCasteljau subdivision of Bezier Curves based on reading of Gernot Hoffmann's Bezier Curves.
*
*       P1 + - - - - + P2                P1 +
*         /           \                    / \
*     P0 *             * P3            P0 *   \   * P3
*                                              \ /
*                                               + P2
*
*
* Split the curve defined by P0,P1,P2,P3 into two new curves L0..L3 and R0..R3 that define the same shape.
*
* Points L0 and R3 are P0 and P3.
* First find points L1, M, R2  as the midpoints of (P0,P1), (P1,P2), (P2,P3).
* Find two more points: L2, R1 defined by midpoints of (L1,M) and (M,R2) respectively.
* The final points L3 and R0 are both the midpoint of (L2,R1)
*
*/
void FUMGLineBuilder::deCasteljauSplit(const FVector2D& P0, const FVector2D& P1, const FVector2D& P2, const FVector2D& P3, FVector2D OutCurveParams[7])
{
	FVector2D L1 = (P0 + P1) * 0.5f;
	FVector2D M = (P1 + P2) * 0.5f;
	FVector2D R2 = (P2 + P3) * 0.5f;

	FVector2D L2 = (L1 + M) * 0.5f;
	FVector2D R1 = (M + R2) * 0.5f;

	FVector2D L3R0 = (L2 + R1) * 0.5f;

	OutCurveParams[0] = P0;
	OutCurveParams[1] = L1;
	OutCurveParams[2] = L2;
	OutCurveParams[3] = L3R0;
	OutCurveParams[4] = R1;
	OutCurveParams[5] = R2;
	OutCurveParams[6] = P3;
}

/** More general form of the deCasteljauSplit splits the curve into two parts at a point between 0 and 1 along the curve's length. */
void FUMGLineBuilder::deCasteljauSplit_WithColorGradient(const FVector2D& P0, const FVector2D& P1, const FVector2D& P2, const FVector2D& P3, FVector2D OutCurveParams[7], float SplitPoint)
{
	FVector2D L1 = FMath::Lerp(P0, P1, SplitPoint);
	FVector2D M = FMath::Lerp(P1, P2, SplitPoint);
	FVector2D R2 = FMath::Lerp(P2, P3, SplitPoint);

	FVector2D L2 = FMath::Lerp(L1, M, SplitPoint);
	FVector2D R1 = FMath::Lerp(M, R2, SplitPoint);

	FVector2D L3R0 = FMath::Lerp(L2, R1, SplitPoint);

	OutCurveParams[0] = P0;
	OutCurveParams[1] = L1;
	OutCurveParams[2] = L2;
	OutCurveParams[3] = L3R0;
	OutCurveParams[4] = R1;
	OutCurveParams[5] = R2;
	OutCurveParams[6] = P3;
}

void FUMGLineBuilder::Subdivide(const FVector2D& P0, const FVector2D& P1, const FVector2D& P2, const FVector2D& P3, FUMGLineBuilder& LineBuilder, float MaxBiasTimesTwo)
{
	const float Curviness = ComputeCurviness(P0, P1, P2, P3);
	if (Curviness > MaxBiasTimesTwo)
	{
		// Split the Bezier into two curves.
		FVector2D TwoCurves[7];
		deCasteljauSplit(P0, P1, P2, P3, TwoCurves);
		// Subdivide left, then right
		Subdivide(TwoCurves[0], TwoCurves[1], TwoCurves[2], TwoCurves[3], LineBuilder, MaxBiasTimesTwo);
		Subdivide(TwoCurves[3], TwoCurves[4], TwoCurves[5], TwoCurves[6], LineBuilder, MaxBiasTimesTwo);
	}
	else
	{
		LineBuilder.AppendPoint(P3, LineBuilder.SingleColor);
	}
}

void FUMGLineBuilder::Subdivide_WithColorGradient(const FLinearColor& StartColor, const FLinearColor& EndColor, const FSlateElementBatcher& InBatcher, const FVector2D& P0, const FVector2D& P1, const FVector2D& P2, const FVector2D& P3, FUMGLineBuilder& LineBuilder, float MaxBiasTimesTwo)
{
	const float Curviness = ComputeCurviness(P0, P1, P2, P3);
	if (Curviness > MaxBiasTimesTwo)
	{
		// Split the Bezier into two curves.
		FVector2D TwoCurves[7];
		deCasteljauSplit(P0, P1, P2, P3, TwoCurves);
		const FLinearColor MidpointColor = FLinearColor::LerpUsingHSV(StartColor, EndColor, 0.5f);
		// Subdivide left, then right
		Subdivide_WithColorGradient(StartColor, MidpointColor, InBatcher, TwoCurves[0], TwoCurves[1], TwoCurves[2], TwoCurves[3], LineBuilder, MaxBiasTimesTwo);
		Subdivide_WithColorGradient(MidpointColor, EndColor, InBatcher, TwoCurves[3], TwoCurves[4], TwoCurves[5], TwoCurves[6], LineBuilder, MaxBiasTimesTwo);
	}
	else
	{
		LineBuilder.AppendPoint(P3, InBatcher.PackVertexColor(EndColor));
	}
}