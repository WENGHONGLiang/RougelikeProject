// Copyright Qibo Pang 2022. All Rights Reserved.


#include "SplineWidget.h"
#include "Blueprint/SlateBlueprintLibrary.h"
#include "Engine/World.h"
#include "Materials/MaterialInterface.h"

EInterpCurveMode ConvertUMGSplineTypeToInterpCurveMode(EUMGSplineType SplineType)
{
	switch (SplineType)
	{
	case EUMGSplineType::Linear:				return CIM_Linear;
	case EUMGSplineType::Curve:					return CIM_CurveUser;

	default:									return CIM_Unknown;
	}
}

void FUMGSplineCurves::UpdateSpline(const FUMGSplineInfo& SplineInfo, bool bStationaryEndpoints, int32 InReparamStepsPerSegment, bool bClosedLoopPositionOverride, float LoopPosition, const FVector2D& Scale2D)
{
	ReparamStepsPerSegment = InReparamStepsPerSegment;

	EInterpCurveMode CurveMode = ConvertUMGSplineTypeToInterpCurveMode(SplineInfo.SplineType);

	Position.Points.Reset(SplineInfo.Points.Num());
	for (int32 Index = 0; Index < SplineInfo.Points.Num(); Index++)
	{
		Position.Points.Emplace(Index, SplineInfo.Points[Index].Location, SplineInfo.Points[Index].Direction, SplineInfo.Points[Index].Direction, CurveMode);
	}
	
	const int32 NumPoints = Position.Points.Num();
	bool bClosedLoop = SplineInfo.bClosedLoop;

	// Ensure splines' looping status matches with that of the spline widget
	if (bClosedLoop)
	{
		const float LastKey = Position.Points.Num() > 0 ? Position.Points.Last().InVal : 0.0f;
		const float LoopKey = bClosedLoopPositionOverride ? LoopPosition : LastKey + 1.0f;
		Position.SetLoopKey(LoopKey);
	}
	else
	{
		Position.ClearLoopKey();
	}

	// Automatically set the tangents on any CurveAuto keys
	// Position.AutoSetTangents(0.0f, bStationaryEndpoints);

	// Now initialize the spline reparam table
	const int32 NumSegments = bClosedLoop ? NumPoints : NumPoints - 1;
	if (NumSegments >= 0)
	{
		// Start by clearing it
		ReparamTable.Points.Reset(NumSegments * ReparamStepsPerSegment + 1);
		float AccumulatedLength = 0.0f;
		for (int32 SegmentIndex = 0; SegmentIndex < NumSegments; ++SegmentIndex)
		{
			for (int32 Step = 0; Step < ReparamStepsPerSegment; ++Step)
			{
				const float Param = static_cast<float>(Step) / ReparamStepsPerSegment;
				const float SegmentLength = (Step == 0) ? 0.0f : GetSegmentLength(SegmentIndex, Param, bClosedLoop, Scale2D);

				ReparamTable.Points.Emplace(SegmentLength + AccumulatedLength, SegmentIndex + Param, 0.0f, 0.0f, CIM_Linear);
			}
			AccumulatedLength += GetSegmentLength(SegmentIndex, 1.0f, bClosedLoop, Scale2D);
		}

		ReparamTable.Points.Emplace(AccumulatedLength, static_cast<float>(NumSegments), 0.0f, 0.0f, CIM_Linear);
		++Version;
	}
}


float FUMGSplineCurves::GetSegmentLength(const int32 Index, const float Param, bool bClosedLoop, const FVector2D& Scale2D) const
{
	const int32 NumPoints = Position.Points.Num();
	const int32 LastPoint = NumPoints - 1;

	check(Index >= 0 && ((bClosedLoop && Index < NumPoints) || (!bClosedLoop && Index < LastPoint)));
	check(Param >= 0.0f && Param <= 1.0f);

	// Evaluate the length of a Hermite spline segment.
	// This calculates the integral of |dP/dt| dt, where P(t) is the spline equation with components (x(t), y(t), z(t)).
	// This isn't solvable analytically, so we use a numerical method (Legendre-Gauss quadrature) which performs very well
	// with functions of this type, even with very few samples.  In this case, just 5 samples is sufficient to yield a
	// reasonable result.

	struct FLegendreGaussCoefficient
	{
		float Abscissa;
		float Weight;
	};

	static const FLegendreGaussCoefficient LegendreGaussCoefficients[] =
	{
		{ 0.0f, 0.5688889f },
		{ -0.5384693f, 0.47862867f },
		{ 0.5384693f, 0.47862867f },
		{ -0.90617985f, 0.23692688f },
		{ 0.90617985f, 0.23692688f }
	};

	const auto& StartPoint = Position.Points[Index];
	const auto& EndPoint = Position.Points[Index == LastPoint ? 0 : Index + 1];

	const auto& P0 = StartPoint.OutVal;
	const auto& T0 = StartPoint.LeaveTangent;
	const auto& P1 = EndPoint.OutVal;
	const auto& T1 = EndPoint.ArriveTangent;

	// Special cases for linear or constant segments
	if (StartPoint.InterpMode == CIM_Linear)
	{
		return ((P1 - P0) * Scale2D).Size() * Param;
	}
	else if (StartPoint.InterpMode == CIM_Constant)
	{
		return 0.0f;
	}

	// Cache the coefficients to be fed into the function to calculate the spline derivative at each sample point as they are constant.
	const FVector2D Coeff1 = ((P0 - P1) * 2.0f + T0 + T1) * 3.0f;
	const FVector2D Coeff2 = (P1 - P0) * 6.0f - T0 * 4.0f - T1 * 2.0f;
	const FVector2D Coeff3 = T0;

	const float HalfParam = Param * 0.5f;

	float Length = 0.0f;
	for (const auto& LegendreGaussCoefficient : LegendreGaussCoefficients)
	{
		// Calculate derivative at each Legendre-Gauss sample, and perform a weighted sum
		const float Alpha = HalfParam * (1.0f + LegendreGaussCoefficient.Abscissa);
		const FVector2D Derivative = ((Coeff1 * Alpha + Coeff2) * Alpha + Coeff3) * Scale2D;
		Length += Derivative.Size() * LegendreGaussCoefficient.Weight;
	}
	Length *= HalfParam;

	return Length;
}


float FUMGSplineCurves::GetSplineLength() const
{
	const int32 NumPoints = ReparamTable.Points.Num();

	// This is given by the input of the last entry in the remap table
	if (NumPoints > 0)
	{
		return ReparamTable.Points.Last().InVal;
	}

	return 0.0f;
}


/// Slate Overrides ///
const FInterpCurvePointVector2D USplineWidget::DummyPointPosition(0.0f, FVector2D::ZeroVector, FVector2D::ZeroVector, FVector2D::ZeroVector, CIM_Constant);

void USplineWidget::OnWidgetRebuilt()
{
	// 
	if (!IsDesignTime())
	{
		UpdateSpline();
	}
}

TSharedRef<SWidget> USplineWidget::RebuildWidget()
{
    Spline = SNew(SWSpline)
                .SplineInfo_UObject(this, &USplineWidget::GetSplineInfo)
				.SplineDebugInfo_UObject(this, &USplineWidget::GetSplineDebugInfo);
	
    return Spline.ToSharedRef();
}

void USplineWidget::ReleaseSlateResources(bool bReleaseChildren)
{
    Super::ReleaseSlateResources(bReleaseChildren);

    Spline.Reset();
}

void USplineWidget::UpdateSpline()
{
    SplineCurves.UpdateSpline(SplineInfo);
}

FVector2D USplineWidget::GetLocationAtSplineInputKey(float InKey, EUMGSplineCoordinateSpace::Type CoordinateSpace) const
{
	FVector2D Location = SplineCurves.Position.Eval(InKey, FVector2D::ZeroVector);

	if (CoordinateSpace == EUMGSplineCoordinateSpace::Type::Screen)
	{
		return GetCachedGeometry().LocalToAbsolute(Location);
	}
	else if(CoordinateSpace == EUMGSplineCoordinateSpace::Type::Viewport)
	{
		FVector2D PixelPosition, ViewportPosition;
		USlateBlueprintLibrary::LocalToViewport(GetWorld(), GetCachedGeometry(), Location, PixelPosition, ViewportPosition);
		return ViewportPosition;
	}
	
    return Location;
}

FVector2D USplineWidget::GetTangentAtSplineInputKey(float InKey, EUMGSplineCoordinateSpace::Type CoordinateSpace) const
{
	FVector2D Tangent = SplineCurves.Position.EvalDerivative(InKey, FVector2D::ZeroVector);

	if (CoordinateSpace == EUMGSplineCoordinateSpace::Type::Screen || CoordinateSpace == EUMGSplineCoordinateSpace::Type::Viewport)
	{
		return GetCachedGeometry().GetAccumulatedRenderTransform().TransformVector(Tangent);
	}
    return Tangent;
}

FVector2D USplineWidget::GetDirectionAtSplineInputKey(float InKey, EUMGSplineCoordinateSpace::Type CoordinateSpace) const
{
	FVector2D Direction = SplineCurves.Position.EvalDerivative(InKey, FVector2D::ZeroVector).GetSafeNormal();

	if (CoordinateSpace == EUMGSplineCoordinateSpace::Type::Screen || CoordinateSpace == EUMGSplineCoordinateSpace::Type::Viewport)
	{
		return GetCachedGeometry().GetAccumulatedRenderTransform().TransformVector(Direction);
	}

	return Direction;
}

float USplineWidget::GetRotationAngleAtSplineInputKey(float InKey, EUMGSplineCoordinateSpace::Type CoordinateSpace) const
{
	FVector2D Direction = GetDirectionAtSplineInputKey(InKey, CoordinateSpace);
	float AngleInRadians = Direction.IsNearlyZero() ? 0.0f : FMath::Atan2(Direction.Y, Direction.X);

	return FMath::RadiansToDegrees(AngleInRadians);
}

float USplineWidget::GetDistanceAlongSplineAtSplineInputKey(float InKey) const
{
	const int32 NumPoints = SplineCurves.Position.Points.Num();
	const int32 NumSegments = SplineInfo.bClosedLoop ? NumPoints : NumPoints - 1;

	if ((InKey >= 0) && (InKey < NumSegments))
	{
		const int32 PointIndex = FMath::FloorToInt(InKey);
		const float Fraction = InKey - PointIndex;
		const int32 ReparamPointIndex = PointIndex * SplineCurves.ReparamStepsPerSegment;
		const float Distance = SplineCurves.ReparamTable.Points[ReparamPointIndex].InVal;
		return Distance + SplineCurves.GetSegmentLength(PointIndex, Fraction, SplineInfo.bClosedLoop, FVector2D(1.0f));;
	}
	else if (InKey >= NumSegments)
	{
		return SplineCurves.GetSplineLength();
	}

	return 0.0f;
}

void USplineWidget::AddSplinePoint(const FUMGSplinePoint& SplinePoint, bool bUpdateSpline)
{
	SplineInfo.AddPoint(SplinePoint);

	if (bUpdateSpline)
	{
		UpdateSpline();
	}
}

void USplineWidget::AddSplinePointAtIndex(const FUMGSplinePoint& SplinePoint, int32 Index, bool bUpdateSpline)
{
	SplineInfo.AddSplinePointAtIndex(SplinePoint, Index);
	
	if (bUpdateSpline)
	{
		UpdateSpline();
	}
}

void USplineWidget::ChangeSplinePointAtIndex(const FUMGSplinePoint& SplinePoint, int32 Index, bool bUpdateSpline)
{
	SplineInfo.ChangeSplinePointAtIndex(SplinePoint, Index);

	if (bUpdateSpline)
	{
		UpdateSpline();
	}
}

void USplineWidget::RemoveSplinePoint(int32 Index, bool bUpdateSpline)
{
	SplineInfo.DeletePoint(Index);

	if (bUpdateSpline)
	{
		UpdateSpline();
	}
}

void USplineWidget::RemoveAllSplinePoint(bool bUpdateSpline)
{
	SplineInfo.Points.Empty();

	if (bUpdateSpline)
	{
		UpdateSpline();
	}
}

void USplineWidget::SetSplineThickness(float Thickness)
{
	if (Thickness >= 0.0f)
	{
		SplineInfo.Thickness = Thickness;
	}
}

float USplineWidget::GetSplineThickness() const
{
	return SplineInfo.Thickness;
}

void USplineWidget::SetSplineMaterial(UMaterialInterface* Material)
{
	if (SplineInfo.BuildCustomVerts && Material)
	{
		SplineInfo.CustomVertsBrush.SetResourceObject(Material);
	}
}

UMaterialInterface* USplineWidget::GetSplineMaterial() const
{
	if (SplineInfo.BuildCustomVerts)
	{
		return Cast<UMaterialInterface>(SplineInfo.CustomVertsBrush.GetResourceObject());
	}
	return nullptr;
}

EUMGSplineType USplineWidget::GetSplineType() const
{
    return SplineInfo.SplineType;
}

void USplineWidget::SetSplineType(EUMGSplineType Type)
{
    SplineInfo.SplineType = Type;
}

int32 USplineWidget::GetNumberOfSplinePoints() const
{
    return SplineInfo.Points.Num();
}

FVector2D USplineWidget::GetLocationAtSplinePoint(int32 PointIndex, EUMGSplineCoordinateSpace::Type CoordinateSpace) const
{
	const FInterpCurvePointVector2D& Point = GetPositionPointSafe(PointIndex);
	const FVector2D& Location = Point.OutVal;

	if (CoordinateSpace == EUMGSplineCoordinateSpace::Type::Screen)
	{
		return GetCachedGeometry().LocalToAbsolute(Location);
	}
	else if (CoordinateSpace == EUMGSplineCoordinateSpace::Type::Viewport)
	{
		FVector2D PixelPosition, ViewportPosition;
		USlateBlueprintLibrary::LocalToViewport(GetWorld(), GetCachedGeometry(), Location, PixelPosition, ViewportPosition);
		return ViewportPosition;
	}

    return Location;
}

FVector2D USplineWidget::GetDirectionAtSplinePoint(int32 PointIndex, EUMGSplineCoordinateSpace::Type CoordinateSpace) const
{
	const FInterpCurvePointVector2D& Point = GetPositionPointSafe(PointIndex);
	const FVector2D Direction = Point.LeaveTangent.GetSafeNormal();

	if (CoordinateSpace == EUMGSplineCoordinateSpace::Type::Screen || CoordinateSpace == EUMGSplineCoordinateSpace::Type::Viewport)
	{
		return GetCachedGeometry().GetAccumulatedRenderTransform().TransformVector(Direction);
	}

	return Direction;
}

FVector2D USplineWidget::GetTangentAtSplinePoint(int32 PointIndex, EUMGSplineCoordinateSpace::Type CoordinateSpace) const
{
	const FInterpCurvePointVector2D& Point = GetPositionPointSafe(PointIndex);
	const FVector2D& Direction = Point.LeaveTangent;

	if (CoordinateSpace == EUMGSplineCoordinateSpace::Type::Screen || CoordinateSpace == EUMGSplineCoordinateSpace::Type::Viewport)
	{
		return GetCachedGeometry().GetAccumulatedRenderTransform().TransformVector(Direction);
	}

	return Direction;
}

float USplineWidget::GetSplineLength() const
{
	return SplineCurves.GetSplineLength();
}

float USplineWidget::GetInputKeyAtDistanceAlongSpline(float Distance, EUMGSplineCoordinateSpace::Type CoordinateSpace) const
{
	const int32 NumPoints = SplineCurves.Position.Points.Num();

	if (NumPoints < 2)
	{
		return 0.0f;
	}

	return SplineCurves.ReparamTable.Eval(Distance, 0.0f);
}

FVector2D USplineWidget::GetLocationAtDistanceAlongSpline(float Distance, EUMGSplineCoordinateSpace::Type CoordinateSpace) const
{
	const float Param = SplineCurves.ReparamTable.Eval(Distance, 0.0f);
	return GetLocationAtSplineInputKey(Param, CoordinateSpace);
}

FVector2D USplineWidget::GetDirectionAtDistanceAlongSpline(float Distance, EUMGSplineCoordinateSpace::Type CoordinateSpace) const
{
	const float Param = SplineCurves.ReparamTable.Eval(Distance, 0.0f);
	return GetDirectionAtSplineInputKey(Param, CoordinateSpace);
}

FVector2D USplineWidget::GetTangentAtDistanceAlongSpline(float Distance, EUMGSplineCoordinateSpace::Type CoordinateSpace) const
{
	const float Param = SplineCurves.ReparamTable.Eval(Distance, 0.0f);
	return GetTangentAtSplineInputKey(Param, CoordinateSpace);
}

float USplineWidget::GetRotationAngleAtDistanceAlongSpline(float Distance, EUMGSplineCoordinateSpace::Type CoordinateSpace) const
{
	const float Param = SplineCurves.ReparamTable.Eval(Distance, 0.0f);
    return GetRotationAngleAtSplineInputKey(Param, CoordinateSpace);
}




