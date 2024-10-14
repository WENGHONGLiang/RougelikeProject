// Copyright Qibo Pang 2022. All Rights Reserved.

/*
* Below Code mainly copy from SlateCore / ElementBatcher.cpp FLineBuilder, I made a few changes to support:
* 1. Add uv information for spline geometry
* 2. Made continuous spline geometry
*/

#pragma once

// Core
#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "HAL/PlatformApplicationMisc.h"
#include "Framework/Application/SlateApplication.h"

class FUMGSplineRenderBatch;

/** Notification for FUMGSplineInfo value change */
DECLARE_DELEGATE_ThreeParams(FOnUMGSplineBuildSegmentAdded, int32/*Index*/, const FVector2D&/*Position*/, float/*Length*/)

/** Utility class for building a strip of lines. */
struct UMGSPLINE_API FUMGLineBuilder
{
	
	FUMGLineBuilder(FUMGSplineRenderBatch& InRenderBatch, const FVector2D StartPoint, float HalfThickness, float InThickness, float InFilterScale, float InCustomVertsVCoordScale, const FSlateRenderTransform& InRenderTransform, const FColor& InColor)
		: RenderBatch(InRenderBatch)
		, RenderTransform(InRenderTransform)
		, LastPointAdded()
		, LastNormal(FVector2D::ZeroVector)
		, HalfLineThickness(HalfThickness)
		, Thickness(InThickness)
		, FilterScale(InFilterScale)
		, CustomVertsVCoordScale(InCustomVertsVCoordScale)
		, NumPointsAdded(1)
		, SingleColor(InColor)
		, SplineLength(0.0f)
		, SplineVCoord(0.0f)
	{
		LastPointAdded[0] = LastPointAdded[1] = StartPoint;
	}

	void SetDelegateOnSegmentAdded(const FOnUMGSplineBuildSegmentAdded& InOnSegmentAdded)
	{
		OnSegmentAdded = InOnSegmentAdded;
	}

	void MakeCap(const FVector2D& CapOrigin, const FVector2D& CapDirection, const FVector2D& Up, const FColor& Color);

	void BuildBezierGeometry_WithColorGradient(const TArray<FSlateGradientStop>& GradientStops, int32 GradientStopIndex, const FVector2D& P0, const FVector2D& P1, const FVector2D& P2, const FVector2D& P3, const FSlateElementBatcher& InBatcher);

	void BuildBezierGeometry(const FVector2D& P0, const FVector2D& P1, const FVector2D& P2, const FVector2D& P3);

	void Finish(const FVector2D& LastPoint, const FColor& InColor, bool bCloseLoop = false);

private:
	void AppendPoint(const FVector2D NewPoint, const FColor& InColor);


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
	static float ComputeCurviness(const FVector2D& P0, const FVector2D& P1, const FVector2D& P2, const FVector2D& P3);


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
	static void deCasteljauSplit(const FVector2D& P0, const FVector2D& P1, const FVector2D& P2, const FVector2D& P3, FVector2D OutCurveParams[7]);

	/** More general form of the deCasteljauSplit splits the curve into two parts at a point between 0 and 1 along the curve's length. */
	static void deCasteljauSplit_WithColorGradient(const FVector2D& P0, const FVector2D& P1, const FVector2D& P2, const FVector2D& P3, FVector2D OutCurveParams[7], float SplitPoint = 0.5f);

	static void Subdivide(const FVector2D& P0, const FVector2D& P1, const FVector2D& P2, const FVector2D& P3, FUMGLineBuilder& LineBuilder, float MaxBiasTimesTwo = 2.0f);

	static void Subdivide_WithColorGradient(const FLinearColor& StartColor, const FLinearColor& EndColor, const FSlateElementBatcher& InBatcher, const FVector2D& P0, const FVector2D& P1, const FVector2D& P2, const FVector2D& P3, FUMGLineBuilder& LineBuilder, float MaxBiasTimesTwo = 2.0f);

private:
	FUMGSplineRenderBatch& RenderBatch;
	const FSlateRenderTransform& RenderTransform;
	FVector2D LastPointAdded[2];
	FVector2D LastNormal;
	float HalfLineThickness;
	float Thickness;
	float FilterScale;
	float CustomVertsVCoordScale;
	int32 NumPointsAdded;
	FColor SingleColor;
	float SplineLength;
	float SplineVCoord;
	FOnUMGSplineBuildSegmentAdded OnSegmentAdded;

};