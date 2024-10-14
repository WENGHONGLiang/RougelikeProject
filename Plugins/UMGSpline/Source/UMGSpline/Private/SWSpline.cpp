// Copyright Qibo Pang 2022. All Rights Reserved.

#include "SWSpline.h"
#include "UMGSplineBuilder.h"
#include "UMGSplineRenderBatch.h"

/*===========================================================================*\
|                                Construct                                    |
\*===========================================================================*/

void SWSpline::Construct(const FArguments& InArgs)
{
	// Arguments passed onto the slate widget
	SplineInfo = InArgs._SplineInfo;

	SplineDebugInfo = InArgs._SplineDebugInfo;
}

/*===========================================================================*\
|                                Overrides                                    |
\*===========================================================================*/

FVector2D SWSpline::ComputeDesiredSize(float) const
{
	return FVector2D(200.0f, 200.0f);
}


int32 SWSpline::OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
	const ESlateDrawEffect DrawEffects = ShouldBeEnabled(bParentEnabled) ? ESlateDrawEffect::None : ESlateDrawEffect::DisabledEffect;
	
	int32 OutLayerId = LayerId;
	if (SplineDebugInfo.Get().bDrawWidgetBoarder)
	{
		OutLayerId++;
		PaintWidgetBoarder(AllottedGeometry, OutDrawElements, OutLayerId, MyCullingRect, DrawEffects);
	}
	
	OutLayerId++;
	if (SplineInfo.Get().BuildCustomVerts)
	{
		PaintSpline_CustomVerts(SplineInfo.Get(), AllottedGeometry, OutDrawElements, OutLayerId, MyCullingRect, DrawEffects, InWidgetStyle);;
	}
	else
	{
		PaintSpline_Default(SplineInfo.Get(), AllottedGeometry, OutDrawElements, OutLayerId, MyCullingRect, DrawEffects, InWidgetStyle);
	}

	return OutLayerId;
}

void SWSpline::PaintWidgetBoarder(const FGeometry& AllottedGeometry, FSlateWindowElementList& OutDrawElements,
	int32 LayerId, const FSlateRect& MyCullingRect, ESlateDrawEffect DrawEffects) const
{
	const FLinearColor& BoarderColor = SplineDebugInfo.Get().BoarderColor;

	// Boarder
	{
		TArray<FVector2D> LinePoints;
		LinePoints.Add(FVector2D(1.0f, 1.0f));
		LinePoints.Add(FVector2D(AllottedGeometry.GetLocalSize().X - 1.0f, 1.0f));
		LinePoints.Add(FVector2D(AllottedGeometry.GetLocalSize().X - 1.0f, AllottedGeometry.GetLocalSize().Y - 1.0f));
		LinePoints.Add(FVector2D(1.0f, AllottedGeometry.GetLocalSize().Y - 1.0f));
		LinePoints.Add(FVector2D(1.0f, 1.0f));

		FSlateDrawElement::MakeLines(
			OutDrawElements,
			LayerId,
			AllottedGeometry.ToPaintGeometry(),
			LinePoints,
			DrawEffects,
			BoarderColor,
			false);
	}
}

void SWSpline::PaintSpline_Default(const FUMGSplineInfo& InSplineInfo, const FGeometry& AllottedGeometry, FSlateWindowElementList& OutDrawElements,
	int32 LayerId, const FSlateRect& MyCullingRect, ESlateDrawEffect DrawEffects, const FWidgetStyle& InWidgetStyle)const
{
	if (InSplineInfo.GetPointNum() > 1)
	{
		FLinearColor InColor = InWidgetStyle.GetColorAndOpacityTint() * InSplineInfo.TintColor;
		FColor TintColor = InColor.ToFColor(true);

		const TArray<FUMGSplinePoint>& Points = InSplineInfo.GetPoints();

		for (int32 i = 0; i < Points.Num() - 1; ++i)
		{
			FVector2D LocalStart = Points[i].Location;
			FVector2D StartDir = InSplineInfo.SplineType ==EUMGSplineType::Linear ? FVector2D::ZeroVector : Points[i].Direction;

			FVector2D LocalEnd = Points[i+1].Location;
			FVector2D EndDir = InSplineInfo.SplineType == EUMGSplineType::Linear ? FVector2D::ZeroVector : Points[i+1].Direction;
			
			bool bAllValuesValid = (LocalStart.X != -FLT_MAX) && (LocalStart.Y != -FLT_MAX) && (LocalEnd.X != -FLT_MAX) && (LocalEnd.Y != -FLT_MAX);

			if (bAllValuesValid)
			{
				FSlateDrawElement::MakeSpline(
					OutDrawElements,
					LayerId + 1,
					AllottedGeometry.ToPaintGeometry(),
					LocalStart,
					StartDir,
					LocalEnd,
					EndDir,
					InSplineInfo.Thickness,
					DrawEffects,
					TintColor);
			}
		}

		if (InSplineInfo.bClosedLoop)
		{
			FVector2D LocalStart = Points.Last().Location;
			FVector2D StartDir = InSplineInfo.SplineType == EUMGSplineType::Linear ? FVector2D::ZeroVector : Points.Last().Direction;

			FVector2D LocalEnd = Points[0].Location;
			FVector2D EndDir = InSplineInfo.SplineType == EUMGSplineType::Linear ? FVector2D::ZeroVector : Points[0].Direction;

			bool bAllValuesValid = (LocalStart.X != -FLT_MAX) && (LocalStart.Y != -FLT_MAX) && (LocalEnd.X != -FLT_MAX) && (LocalEnd.Y != -FLT_MAX);

			if (bAllValuesValid)
			{
				FSlateDrawElement::MakeSpline(
					OutDrawElements,
					LayerId + 1,
					AllottedGeometry.ToPaintGeometry(),
					LocalStart,
					StartDir,
					LocalEnd,
					EndDir,
					InSplineInfo.Thickness,
					DrawEffects,
					TintColor);
			}
		}
	}
}

void SWSpline::PaintSpline_CustomVerts(const FUMGSplineInfo& InSplineInfo, const FGeometry& AllottedGeometry, FSlateWindowElementList& OutDrawElements,
	int32 LayerId, const FSlateRect& MyCullingRect, ESlateDrawEffect DrawEffects, const FWidgetStyle& InWidgetStyle)const
{
	if (InSplineInfo.GetPointNum() > 1)
	{
		const TArray<FUMGSplinePoint>& Points = InSplineInfo.GetPoints();
		const FPaintGeometry& PaintGeometry = AllottedGeometry.ToPaintGeometry();
		PaintGeometry.CommitTransformsIfUsingLegacyConstructor();
		const FSlateRenderTransform& SlateRenderTransform = PaintGeometry.GetAccumulatedRenderTransform();

		// 1 is the minimum thickness we support for generating geometry.
		// The shader takes care of sub-pixel line widths.
		// Thickness is given in screenspace, so convert it to local space before proceeding.
		FSlateLayoutTransform LayoutTransform = FSlateLayoutTransform(PaintGeometry.DrawScale, FVector2D(AllottedGeometry.ToPaintGeometry().DrawPosition));

		float InThickness = FMath::Max(1.0f, Inverse(LayoutTransform).GetScale() * InSplineInfo.Thickness);

		static const float TwoRootTwo = 2 * UE_SQRT_2;
		// Compute the actual size of the line we need based on thickness.
		// Each line segment will be a bit thicker than the line to account
		// for the size of the filter.
		const float LineThickness = (TwoRootTwo + InThickness);

		// Width of the filter size to use for anti-aliasing.
		// Increasing this value will increase the fuzziness of line edges.
		const float FilterScale = 1.0f; 
		const float HalfThickness = LineThickness * 0.5f + FilterScale;

		TArray<FSlateVertex> SlateVerts;
		TArray<SlateIndex> Indexes;
		FUMGSplineRenderBatch RenderBatch = FUMGSplineRenderBatch(&SlateVerts, &Indexes);

		FLinearColor InColor = InWidgetStyle.GetColorAndOpacityTint() * InSplineInfo.TintColor;
		FColor TintColor = InColor.ToFColor(true);
		
		FUMGLineBuilder SplineBuilder = FUMGLineBuilder(RenderBatch, Points[0].Location, HalfThickness, InSplineInfo.Thickness, FilterScale, InSplineInfo.CustomVertsVCoordScale, SlateRenderTransform, TintColor);

		for (int32 i = 0; i < Points.Num() - 1; ++i)
		{
			FVector2D LocalStart = Points[i].Location;
			FVector2D StartDir = InSplineInfo.SplineType == EUMGSplineType::Linear ? FVector2D::ZeroVector : Points[i].Direction;

			FVector2D LocalEnd = Points[i + 1].Location;
			FVector2D EndDir = InSplineInfo.SplineType == EUMGSplineType::Linear ? FVector2D::ZeroVector : Points[i + 1].Direction;

			bool bAllValuesValid = (LocalStart.X != -FLT_MAX) && (LocalStart.Y != -FLT_MAX) && (LocalEnd.X != -FLT_MAX) && (LocalEnd.Y != -FLT_MAX);

			if (bAllValuesValid)
			{
				FVector2D P1 = LocalStart + StartDir / 3.0f;
				FVector2D P2 = LocalEnd - EndDir / 3.0f;
				SplineBuilder.BuildBezierGeometry(LocalStart, P1, P2, LocalEnd);
			}

		}

		if (InSplineInfo.bClosedLoop)
		{
			FVector2D LocalStart = Points.Last().Location;
			FVector2D StartDir = InSplineInfo.SplineType == EUMGSplineType::Linear ? FVector2D::ZeroVector : Points.Last().Direction;

			FVector2D LocalEnd = Points[0].Location;
			FVector2D EndDir = InSplineInfo.SplineType == EUMGSplineType::Linear ? FVector2D::ZeroVector : Points[0].Direction;

			bool bAllValuesValid = (LocalStart.X != -FLT_MAX) && (LocalStart.Y != -FLT_MAX) && (LocalEnd.X != -FLT_MAX) && (LocalEnd.Y != -FLT_MAX);

			if (bAllValuesValid)
			{
				FVector2D P1 = LocalStart + StartDir / 3.0f;
				FVector2D P2 = LocalEnd - EndDir / 3.0f;
				SplineBuilder.BuildBezierGeometry(LocalStart, P1, P2, LocalEnd);
			}

			SplineBuilder.Finish(Points[0].Location, TintColor, true);
		}
		else
		{
			SplineBuilder.Finish(Points.Last().Location, TintColor);
		}
		

		// Get a brush
		FSlateResourceHandle Handle = FSlateApplication::Get().GetRenderer()->GetResourceHandle(InSplineInfo.CustomVertsBrush);

		// Draw
		FSlateDrawElement::MakeCustomVerts(OutDrawElements, LayerId, Handle, SlateVerts, Indexes, nullptr, 0, 0);
	}
}