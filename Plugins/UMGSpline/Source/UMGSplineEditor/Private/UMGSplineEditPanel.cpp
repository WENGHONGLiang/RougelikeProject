// Copyright Qibo Pang 2022. All Rights Reserved.


#include "UMGSplineEditPanel.h"
#include "EditorStyleSet.h"
#include "Widgets/Input/SSlider.h"
#include "UMGSplineBuilder.h"
#include "UMGSplineRenderBatch.h"
#include "UMGSplineEditorSettings.h"
#include "Styling/ToolBarStyle.h"

#define LOCTEXT_NAMESPACE "SUMGSplineEditPanel"

const static FVector2D	CONST_KeySize = FVector2D(11, 11);
const static FVector2D	CONST_TangentSize = FVector2D(7, 7);
const static FVector2D	CONST_CurveSize = FVector2D(12, 12);

const static float		CONST_FitMargin = 0.25f;
const static float		CONST_MinViewRange = 100.0f;
const static float		CONST_DefaultZoomRange = 200.0f;
const static float      CONST_KeyTangentOffsetMin = 30.0f;
const static float      CONST_KeyTangentOffsetMax = 150.0f;
const static float      CONST_TangentMaxStrength = 2000.0f;
const static float      CONST_TangentSelectIconOffset = 15.0f;

const static float      CONST_PanelHeightMin = 200.0f;
const static float      CONST_PanelHeightMax = 1000.0f;

const static float      CONST_SplineThicknessMin = 1.0f;
const static float      CONST_SplineThicknessMax = 100.0f;
/*===========================================================================*\
|                                Construct                                    |
\*===========================================================================*/

void SUMGSplineEditPanel::Construct(const FArguments& InArgs)
{
	// Arguments passed onto the slate widget
	SplineInfo = InArgs._SplineInfo;

	OnSplineInfoValueChanged = InArgs._OnSplineInfoValueChanged;

	DragState = EDragState::None;
	DragThreshold = 4;

	SelectedPointIndex = 0;

	const FToolBarStyle& ToolBarStyle = FAppStyle::Get().GetWidgetStyle<FToolBarStyle>("EditorViewportToolBar");

	ChildSlot
		[
			SNew(SHorizontalBox)

			+ SHorizontalBox::Slot()
			.FillWidth(1.0f)
			[
				SNew(SVerticalBox)

				+ SVerticalBox::Slot()
				.FillHeight(1.0f)
				[
					SNew(SHorizontalBox)

					+ SHorizontalBox::Slot()
						.AutoWidth()
						[
							SNew(SBorder)
							.VAlign(VAlign_Top)
							.HAlign(HAlign_Left)
							.BorderImage(FAppStyle::GetBrush("NoBorder"))
							.DesiredSizeScale(FVector2D(256.0f, 32.0f))
							.Padding(FMargin(2, 2, 0, 0))
							[
								SNew(SHorizontalBox)
								+ SHorizontalBox::Slot()
								.AutoWidth()
								.VAlign(VAlign_Center)
								[
									SNew(SComboButton)
									.ButtonStyle(&ToolBarStyle.ButtonStyle)
									.OnGetMenuContent(this, &SUMGSplineEditPanel::GetPanelHeightMenu)
									.ContentPadding(ToolBarStyle.ButtonPadding)
									.ButtonContent()
									[
										SNew(STextBlock)
										.Text(LOCTEXT("AdjustPanelHeight", "Panel Height"))
										.TextStyle(&ToolBarStyle.LabelStyle)
										.ColorAndOpacity(FSlateColor::UseForeground())
									]
								]

								+ SHorizontalBox::Slot()
								.AutoWidth()
								[
									SNew(SButton)
									.ButtonStyle(&ToolBarStyle.ButtonStyle)
									.ToolTipText(LOCTEXT("ZoomToFit", "Zoom To Fit"))
									.OnClicked(this, &SUMGSplineEditPanel::ZoomToFitClicked)
									.ContentPadding(ToolBarStyle.ButtonPadding)
									.ContentPadding(1)
									[
										SNew(SImage)
										.Image(FAppStyle::GetBrush("UMGEditor.ZoomToFit"))
										.ColorAndOpacity(FSlateColor::UseForeground())
									]
								]
								
								// Screen Fill Size Rule
								+ SHorizontalBox::Slot()
									.AutoWidth()
									.VAlign(VAlign_Center)
									[
										SNew(SComboButton)
										.ButtonStyle(&ToolBarStyle.ButtonStyle)
										.OnGetMenuContent(this, &SUMGSplineEditPanel::GetThicknessFillMenu)
										.ContentPadding(ToolBarStyle.ButtonPadding)
										.ButtonContent()
										[
											SNew(STextBlock)
											.Text(LOCTEXT("Thickness", "Thickness"))
											.TextStyle(&ToolBarStyle.LabelStyle)
											.ColorAndOpacity(FSlateColor::UseForeground())
										]
									]
							]
						]
				]
			]
			// Help tips
			+ SHorizontalBox::Slot()
				.AutoWidth()
				.Padding(FMargin(8.0f, 8.0f))
				.HAlign(HAlign_Right)
				.VAlign(VAlign_Top)
				[
					SAssignNew(HelpIconImage, SImage)
					.Image(FAppStyle::GetBrush("Icons.Help"))
					.ToolTipText(this, &SUMGSplineEditPanel::GetHelpText)
				]
		];

	DeferredZoomToFit();
}

/*===========================================================================*\
|                                Overrides                                    |
\*===========================================================================*/

FVector2D SUMGSplineEditPanel::ComputeDesiredSize(float) const
{
	return FVector2D(300.0f, 310.0f);
}

void SUMGSplineEditPanel::Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime)
{
	// Zoom to extents
	if (bDeferredZoomToFit)
	{
		bDeferredZoomToFit = false;
		
		RegisterActiveTimer(0.f, FWidgetActiveTimerDelegate::CreateSP(this, &SUMGSplineEditPanel::HandleZoomToFit));
	}
}


FReply SUMGSplineEditPanel::OnMouseButtonDown(const FGeometry& InMyGeometry, const FPointerEvent& InMouseEvent)
{
	const bool bLeftMouseButton = InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton;
	const bool bMiddleMouseButton = InMouseEvent.GetEffectingButton() == EKeys::MiddleMouseButton;
	const bool bRightMouseButton = InMouseEvent.GetEffectingButton() == EKeys::RightMouseButton;

	DragState = EDragState::PreDrag;

	if (bLeftMouseButton || bMiddleMouseButton || bRightMouseButton)
	{
		MouseDownLocation = InMyGeometry.AbsoluteToLocal(InMouseEvent.GetScreenSpacePosition());

		// Set keyboard focus to this so that selected text box doesn't try to apply to newly selected keys
		if (!HasKeyboardFocus())
		{
			FSlateApplication::Get().SetKeyboardFocus(SharedThis(this), EFocusCause::SetDirectly);
		}

		bEditPanelFocus = true;

		// Always capture mouse if we left or right click on the widget
		return FReply::Handled().CaptureMouse(SharedThis(this));
	}

	return FReply::Unhandled();
}

FReply SUMGSplineEditPanel::OnMouseButtonUp(const FGeometry& InMyGeometry, const FPointerEvent& InMouseEvent)
{
	if (this->HasMouseCapture())
	{
		if (DragState == EDragState::PreDrag)
		{
			// If the user didn't start dragging, handle the mouse operation as a click.
			ProcessClick(InMyGeometry, InMouseEvent);
		}
		else
		{
			EndDrag(InMyGeometry, InMouseEvent);
		}
		return FReply::Handled().ReleaseMouseCapture();
	}

	return FReply::Unhandled();
}

FReply SUMGSplineEditPanel::OnMouseMove(const FGeometry& InMyGeometry, const FPointerEvent& InMouseEvent)
{
	if (this->HasMouseCapture())
	{
		if (DragState == EDragState::PreDrag)
		{
			TryStartDrag(InMyGeometry, InMouseEvent);
		}
		if (DragState != EDragState::None)
		{
			ProcessDrag(InMyGeometry, InMouseEvent);
		}
		MouseMoveLocation = InMyGeometry.AbsoluteToLocal(InMouseEvent.GetScreenSpacePosition());
		return FReply::Handled();
	}

	return FReply::Unhandled();
}

void SUMGSplineEditPanel::OnMouseCaptureLost(const FCaptureLostEvent& CaptureLostEvent)
{
	DragState = EDragState::None;
}


FReply SUMGSplineEditPanel::OnMouseWheel(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	if (bEditPanelFocus/*HasKeyboardFocus()*/)
	{
		ZoomView(MyGeometry, MouseEvent);
		return FReply::Handled();
	}
	return FReply::Unhandled();
}

void SUMGSplineEditPanel::OnMouseLeave(const FPointerEvent& MouseEvent)
{
	bEditPanelFocus = false;
}

void SUMGSplineEditPanel::ZoomView(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	const float CONST_Scale_Min = 0.1f;
	const float CONST_Scale_Max = 10.0f;

	float Delta = MouseEvent.GetWheelDelta();
	float NewScale = TransformInfo.Scale + Delta * 0.1f;
	NewScale = FMath::Clamp(NewScale, CONST_Scale_Min, CONST_Scale_Max);
	
	// Mouse position is zoom center
	FVector2D MousePosition = MyGeometry.AbsoluteToLocal(MouseEvent.GetScreenSpacePosition());
	FVector2D InPosition = TransformInfo.LocalToInput(MousePosition);
	FVector2D NewOffset = InPosition - (MousePosition / NewScale);


	TransformInfo.Scale = NewScale;
	TransformInfo.Offset = NewOffset;
}


FReply SUMGSplineEditPanel::OnKeyDown(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent)
{
	if (InKeyEvent.GetKey() == EKeys::Platform_Delete && SelectedPointIndex != -1)
	{
		DeleteSelectedSplinePoint();
		return FReply::Handled();
	}
	
	return FReply::Unhandled();
}

int32 SUMGSplineEditPanel::OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
	const ESlateDrawEffect DrawEffects = ShouldBeEnabled(bParentEnabled) ? ESlateDrawEffect::None : ESlateDrawEffect::DisabledEffect;
	
	int32 BackgroundLayerId = LayerId;

	// grid lines.
	int32 GridLineLayerId = BackgroundLayerId + 1;
	PaintGridLines(AllottedGeometry, OutDrawElements, GridLineLayerId, MyCullingRect, DrawEffects);
	
	int32 SplineLayerId = GridLineLayerId + 1;
	if (SplineInfo.Get().BuildCustomVerts)
	{
		PaintSpline_CustomVerts(SplineInfo.Get(), AllottedGeometry, OutDrawElements, SplineLayerId, MyCullingRect, DrawEffects, InWidgetStyle);;
	}
	else
	{
		PaintSpline_Default(SplineInfo.Get(), AllottedGeometry, OutDrawElements, SplineLayerId, MyCullingRect, DrawEffects, InWidgetStyle);
	}

	int32 KeyLayerId = SplineLayerId + 1;
	int32 SelectedKeyLayerId = KeyLayerId + 1;
	PaintSplinePoints(SplineInfo.Get(), OutDrawElements, KeyLayerId, SelectedKeyLayerId, AllottedGeometry, MyCullingRect, DrawEffects, InWidgetStyle);
	
	// Paint children
	int32 ChildrenLayerId = SelectedKeyLayerId + 1;
	int32 MarqueeLayerId = SCompoundWidget::OnPaint(Args, AllottedGeometry, MyCullingRect, OutDrawElements, ChildrenLayerId, InWidgetStyle, bParentEnabled);

	return ChildrenLayerId + 1;
}

/*===========================================================================*\
|                            Drag                                             |
\*===========================================================================*/

void SUMGSplineEditPanel::TryStartDrag(const FGeometry& InMyGeometry, const FPointerEvent& InMouseEvent)
{
	const bool bLeftMouseButton = InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton);
	const bool bMiddleMouseButton = InMouseEvent.IsMouseButtonDown(EKeys::MiddleMouseButton);
	const bool bRightMouseButton = InMouseEvent.IsMouseButtonDown(EKeys::RightMouseButton);
	const bool bControlDown = InMouseEvent.IsControlDown();
	const bool bShiftDown = InMouseEvent.IsShiftDown();
	const bool bAltDown = InMouseEvent.IsAltDown();

	FVector2D MousePosition = InMyGeometry.AbsoluteToLocal(InMouseEvent.GetScreenSpacePosition());
	FVector2D DragVector = MousePosition - MouseDownLocation;
	if (DragVector.SizeSquared() >= FMath::Square(DragThreshold))
	{
		if (bLeftMouseButton)
		{
			// Check if we should start dragging keys.
			int HitPointIndex = HitTestSplinePoints(InMyGeometry, InMyGeometry.LocalToAbsolute(MouseDownLocation));
			if (HitPointIndex != -1)
			{
				SelectedPointIndex = HitPointIndex;
				DragState = EDragState::DragKey;
				DraggedPointIndex = HitPointIndex;
				PreDragPointLocations.Empty();
				PreDragPointLocations.Add(DraggedPointIndex, MousePosition);
			}
			else
			{
				// Check if we should start dragging a tangent.
				FSelectedSplineTangent HitTangent = HitTestCubicTangents(InMyGeometry, InMyGeometry.LocalToAbsolute(MouseDownLocation));
				if (HitTangent.IsValid())
				{
					SelectedTangent = HitTangent;
					SelectedPointIndex = HitTangent.PointIndex;
					DragState = EDragState::DragTangent;
					PreDragTangents.Empty();
					PreDragTangents.Add(HitTangent.PointIndex, SplineInfo.Get().Points[HitTangent.PointIndex].Direction);
				}
			}
		}
		else if (bRightMouseButton)
		{
			if (bAltDown)
			{
				DragState = EDragState::Zoom;
			}
			else
			{
				DragState = EDragState::Pan;
			}
		}
		else
		{
			DragState = EDragState::None;
		}
	}
}

void SUMGSplineEditPanel::ProcessDrag(const FGeometry& InMyGeometry, const FPointerEvent& InMouseEvent)
{
	FVector2D ScreenDelta = InMouseEvent.GetCursorDelta();
	FVector2D InputDelta = ScreenDelta / TransformInfo.Scale;

	if (DragState == EDragState::DragKey)
	{
		FVector2D MousePosition = InMyGeometry.AbsoluteToLocal(InMouseEvent.GetScreenSpacePosition());
		FVector2D Delta = MousePosition - PreDragPointLocations[DraggedPointIndex];

		FUMGSplineInfo NewSplineInfo = SplineInfo.Get();
		NewSplineInfo.Points[DraggedPointIndex].Location = TransformInfo.LocalToInput(MousePosition); 
		OnSplineInfoValueChanged.ExecuteIfBound(NewSplineInfo);
	}
	else if (DragState == EDragState::DragTangent)
	{
		FVector2D MousePosition = InMyGeometry.AbsoluteToLocal(InMouseEvent.GetScreenSpacePosition());

		FVector2D KeyLocation = SplineInfo.Get().Points[SelectedTangent.PointIndex].Location;
		FVector2D KeyLocalLocation = TransformInfo.InputToLocal(KeyLocation);
		
		float Distance = FVector2D::Distance(MousePosition, KeyLocalLocation);
		Distance = FMath::Max(CONST_KeyTangentOffsetMin, Distance);
		float Alpha = (Distance - CONST_KeyTangentOffsetMin) / (CONST_KeyTangentOffsetMax - CONST_KeyTangentOffsetMin);
		Alpha = FMath::Max(0.01f, Alpha);

		float Strength = (CONST_TangentMaxStrength * Alpha) / TransformInfo.Scale;

		FVector2D NewDirection = SelectedTangent.bIsArrial ? (KeyLocalLocation - MousePosition) : (MousePosition - KeyLocalLocation);
		NewDirection.Normalize();
		NewDirection = NewDirection * Strength;

		FUMGSplineInfo NewSplineInfo = SplineInfo.Get();
		NewSplineInfo.Points[SelectedTangent.PointIndex].Direction = NewDirection;
		OnSplineInfoValueChanged.ExecuteIfBound(NewSplineInfo);

	}
	else if (DragState == EDragState::Pan)
	{
		TransformInfo.Offset -= InputDelta;
	}
}

void SUMGSplineEditPanel::EndDrag(const FGeometry& InMyGeometry, const FPointerEvent& InMouseEvent)
{
	DragState = EDragState::None;
}


int SUMGSplineEditPanel::HitTestSplinePoints(const FGeometry& InMyGeometry, const FVector2D& HitScreenPosition)
{
	const FVector2D HitPosition = InMyGeometry.AbsoluteToLocal(HitScreenPosition);

	const TArray<FUMGSplinePoint>& Points = SplineInfo.Get().GetPoints();

	// Iterate over each key
	for (int32 i = 0; i < Points.Num(); ++i)
	{
		FVector2D PointLocalLocation = TransformInfo.InputToLocal(Points[i].Location);

		if (HitPosition.X > (PointLocalLocation.X - (0.5f * CONST_KeySize.X)) &&
			HitPosition.X < (PointLocalLocation.X + (0.5f * CONST_KeySize.X)) &&
			HitPosition.Y >(PointLocalLocation.Y - (0.5f * CONST_KeySize.Y)) &&
			HitPosition.Y < (PointLocalLocation.Y + (0.5f * CONST_KeySize.Y)))
		{
			return i;
		}
	}


	return -1;
}

FSelectedSplineTangent SUMGSplineEditPanel::HitTestCubicTangents(const FGeometry& InMyGeometry, const FVector2D& HitScreenPosition)
{
	FSelectedSplineTangent HitTangent;

	const FVector2D HitPosition = InMyGeometry.AbsoluteToLocal(HitScreenPosition);

	const TArray<FUMGSplinePoint>& Points = SplineInfo.Get().GetPoints();

	// Iterate over each key
	for (int32 i = 0; i < Points.Num(); ++i)
	{
		FVector2D LocalLocation = TransformInfo.InputToLocal(Points[i].Location);
		FVector2D LocalDir = Points[i].Direction * TransformInfo.Scale;
		FVector2D Arrive, Leave;
		GetTangentPoints(LocalLocation, LocalDir, Arrive, Leave);

		if (HitPosition.Y > (Arrive.Y - (0.5f * CONST_CurveSize.Y)) &&
			HitPosition.Y < (Arrive.Y + (0.5f * CONST_CurveSize.Y)) &&
			HitPosition.X >(Arrive.X - (0.5f * CONST_TangentSize.X)) &&
			HitPosition.X < (Arrive.X + (0.5f * CONST_TangentSize.X)))
		{
			HitTangent.PointIndex = i;
			HitTangent.bIsArrial = true;
			break;
		}
		if (HitPosition.Y > (Leave.Y - (0.5f * CONST_CurveSize.Y)) &&
			HitPosition.Y < (Leave.Y + (0.5f * CONST_CurveSize.Y)) &&
			HitPosition.X >(Leave.X - (0.5f * CONST_TangentSize.X)) &&
			HitPosition.X < (Leave.X + (0.5f * CONST_TangentSize.X)))
		{
			HitTangent.PointIndex = i;
			HitTangent.bIsArrial = false;
			break;
		}
	}

	return HitTangent;
}
	
/*===========================================================================*\
|                                Paint functions                              |
\*===========================================================================*/

void SUMGSplineEditPanel::PaintSpline_Default(const FUMGSplineInfo& InSplineInfo, const FGeometry& AllottedGeometry, FSlateWindowElementList& OutDrawElements,
	int32 LayerId, const FSlateRect& MyCullingRect, ESlateDrawEffect DrawEffects, const FWidgetStyle& InWidgetStyle)const
{
	if (InSplineInfo.GetPointNum() > 1)
	{
		FLinearColor InColor = InWidgetStyle.GetColorAndOpacityTint() * InSplineInfo.TintColor;
		FColor TintColor = InColor.ToFColor(true);

		const TArray<FUMGSplinePoint>& Points = InSplineInfo.GetPoints();

		for (int32 i = 0; i < Points.Num() - 1; ++i)
		{
			FVector2D LocalStart = TransformInfo.InputToLocal(Points[i].Location);
			FVector2D StartDir = InSplineInfo.SplineType == EUMGSplineType::Linear ? FVector2D::ZeroVector : Points[i].Direction;
			StartDir = StartDir * TransformInfo.Scale;

			FVector2D LocalEnd = TransformInfo.InputToLocal(Points[i + 1].Location);
			FVector2D EndDir = InSplineInfo.SplineType == EUMGSplineType::Linear ? FVector2D::ZeroVector : Points[i + 1].Direction;
			EndDir = EndDir * TransformInfo.Scale;

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
					InSplineInfo.Thickness,// * TransformInfo.Scale,
					DrawEffects,
					TintColor);
			}
		}

		if (InSplineInfo.bClosedLoop)
		{
			FVector2D LocalStart = TransformInfo.InputToLocal(Points.Last().Location);
			FVector2D StartDir = InSplineInfo.SplineType == EUMGSplineType::Linear ? FVector2D::ZeroVector : Points.Last().Direction;
			StartDir = StartDir * TransformInfo.Scale;

			FVector2D LocalEnd = TransformInfo.InputToLocal(Points[0].Location);
			FVector2D EndDir = InSplineInfo.SplineType == EUMGSplineType::Linear ? FVector2D::ZeroVector : Points[0].Direction;
			EndDir = EndDir * TransformInfo.Scale;

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

void SUMGSplineEditPanel::PaintSpline_CustomVerts(const FUMGSplineInfo& InSplineInfo, const FGeometry& AllottedGeometry, FSlateWindowElementList& OutDrawElements,
	int32 LayerId, const FSlateRect& MyCullingRect, ESlateDrawEffect DrawEffects, const FWidgetStyle& InWidgetStyle)const
{
	if (InSplineInfo.GetPointNum() > 1)
	{
		const TArray<FUMGSplinePoint>& Points = InSplineInfo.GetPoints();

		// 1 is the minimum thickness we support for generating geometry.
		// The shader takes care of sub-pixel line widths.
		// Thickness is given in screenspace, so convert it to local space before proceeding.
		//FSlateLayoutTransform LayoutTransform = FSlateLayoutTransform(AllottedGeometry.ToPaintGeometry().DrawScale, AllottedGeometry.ToPaintGeometry().DrawPosition);
		float InThickness = InSplineInfo.Thickness;// FMath::Max(1.0f, Inverse(LayoutTransform).GetScale() * InSplineInfo.Thickness);

		static const float TwoRootTwo = 2 * UE_SQRT_2;
		// Compute the actual size of the line we need based on thickness.
		// Each line segment will be a bit thicker than the line to account
		// for the size of the filter.
		const float LineThickness = (TwoRootTwo + InThickness);

		// Width of the filter size to use for anti-aliasing.
		// Increasing this value will increase the fuzziness of line edges.
		const float FilterScale = 1.0f;
		const float HalfThickness = (LineThickness * 0.5f + FilterScale);// *TransformInfo.Scale;

		TArray<FSlateVertex> SlateVerts;
		TArray<SlateIndex> Indexes;
		FUMGSplineRenderBatch RenderBatch = FUMGSplineRenderBatch(&SlateVerts, &Indexes);

		FLinearColor InColor = InWidgetStyle.GetColorAndOpacityTint() * InSplineInfo.TintColor;
		FColor TintColor = InColor.ToFColor(true);
		FVector2D StartPosition = TransformInfo.InputToLocal(Points[0].Location);
		float CustomVertsVCoordScale = InSplineInfo.CustomVertsVCoordScale / TransformInfo.Scale;
		FUMGLineBuilder SplineBuilder = FUMGLineBuilder(RenderBatch, StartPosition, HalfThickness, InSplineInfo.Thickness, FilterScale, CustomVertsVCoordScale, AllottedGeometry.ToPaintGeometry().GetAccumulatedRenderTransform(), TintColor);

		for (int32 i = 0; i < Points.Num() - 1; ++i)
		{
			FVector2D LocalStart = TransformInfo.InputToLocal(Points[i].Location);
			FVector2D StartDir = InSplineInfo.SplineType == EUMGSplineType::Linear ? FVector2D::ZeroVector : Points[i].Direction;
			StartDir = StartDir * TransformInfo.Scale;

			FVector2D LocalEnd = TransformInfo.InputToLocal(Points[i + 1].Location);
			FVector2D EndDir = InSplineInfo.SplineType == EUMGSplineType::Linear ? FVector2D::ZeroVector : Points[i + 1].Direction;
			EndDir = EndDir * TransformInfo.Scale;

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
			FVector2D LocalStart = TransformInfo.InputToLocal(Points.Last().Location);
			FVector2D StartDir = InSplineInfo.SplineType == EUMGSplineType::Linear ? FVector2D::ZeroVector : Points.Last().Direction;
			StartDir = StartDir * TransformInfo.Scale;

			FVector2D LocalEnd = TransformInfo.InputToLocal(Points[0].Location);
			FVector2D EndDir = InSplineInfo.SplineType == EUMGSplineType::Linear ? FVector2D::ZeroVector : Points[0].Direction;
			EndDir = EndDir * TransformInfo.Scale;

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

void SUMGSplineEditPanel::PaintSplinePoints(const FUMGSplineInfo& InSplineInfo, FSlateWindowElementList& OutDrawElements, int32 LayerId, int32 SelectedLayerId, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, ESlateDrawEffect DrawEffects, const FWidgetStyle& InWidgetStyle) const
{
	const FSlateBrush* KeyBrush = FAppStyle::GetBrush("CurveEd.CurveKey");
	const FLinearColor KeyColor = FAppStyle::GetColor("CurveEd.TangentColor");
	const FLinearColor KeySelectionColor = FAppStyle::GetBrush("CurveEd.CurveKeySelected")->GetTint(InWidgetStyle);

	const TArray<FUMGSplinePoint>& Points = InSplineInfo.GetPoints();

	// Iterate over each key
	for (int32 i = 0; i < Points.Num(); ++i)
	{

		// Work out where it is
		FVector2D KeyLocation = TransformInfo.InputToLocal(Points[i].Location);
		FVector2D KeyIconLocation = KeyLocation - (CONST_KeySize / 2);

		// Get brush
		bool IsSelected = IsSplinePointSelected(i);
		
		int32 LayerToUse = IsSelected ? SelectedLayerId : LayerId;

		// Fade out keys that are not selected and whose curve is not selected as well.
		FLinearColor TintColor = IsSelected ? KeySelectionColor : KeyColor;

		FSlateDrawElement::MakeBox(
			OutDrawElements,
			LayerToUse,
			AllottedGeometry.ToPaintGeometry(KeyIconLocation, CONST_KeySize),
			KeyBrush,
			DrawEffects,
			TintColor
		);

		if ((IsSelected || !HideUnselectedTangents) && InSplineInfo.SplineType == EUMGSplineType::Curve)
		{
			PaintTangent(Points[i], OutDrawElements, LayerId, AllottedGeometry, MyCullingRect, DrawEffects, LayerToUse, InWidgetStyle, IsSelected);
		}
	}
}


void SUMGSplineEditPanel::PaintTangent(const FUMGSplinePoint& SplinePoint, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, ESlateDrawEffect DrawEffects, int32 LayerToUse, const FWidgetStyle& InWidgetStyle, bool bTangentSelected) const
{
	FVector2D KeyIconLocation = TransformInfo.InputToLocal(SplinePoint.Location);
	FVector2D LocalDir = SplinePoint.Direction * TransformInfo.Scale;

	FVector2D ArriveTangentLocation, LeaveTangentLocation;
	GetTangentPoints(KeyIconLocation, LocalDir, ArriveTangentLocation, LeaveTangentLocation);

	FVector2D ArriveTangentIconLocation = ArriveTangentLocation - (CONST_TangentSize / 2);
	FVector2D LeaveTangentIconLocation = LeaveTangentLocation - (CONST_TangentSize / 2);

	const FSlateBrush* TangentBrush = FAppStyle::GetBrush("CurveEd.Tangent");
	const FSlateBrush* TangentBrushSelected = FAppStyle::GetBrush("CurveEd.TangentSelected");
	const FSlateBrush* ArrowImage = FAppStyle::GetBrush(TEXT("Graph.Arrow"));
	const FLinearColor TangentColor = FAppStyle::GetColor("CurveEd.TangentColor");
	const FLinearColor TangentColorSelected = FAppStyle::GetColor("CurveEd.TangentColorSelected");

	bool LeaveTangentSelected = bTangentSelected;
	bool ArriveTangentSelected = bTangentSelected;

	//Add lines from tangent control point to 'key'
	TArray<FVector2D> LinePoints;
	LinePoints.Add(KeyIconLocation);
	LinePoints.Add(ArriveTangentLocation);
	FSlateDrawElement::MakeLines(
		OutDrawElements,
		LayerId,
		AllottedGeometry.ToPaintGeometry(),
		LinePoints,
		DrawEffects,
		ArriveTangentSelected ? TangentColorSelected : TangentColor
	);

	LinePoints.Empty();
	LinePoints.Add(KeyIconLocation);
	LinePoints.Add(LeaveTangentLocation);
	FSlateDrawElement::MakeLines(
		OutDrawElements,
		LayerId,
		AllottedGeometry.ToPaintGeometry(),
		LinePoints,
		DrawEffects,
		LeaveTangentSelected ? TangentColorSelected : TangentColor
	);

	//Arrive tangent control
	FSlateDrawElement::MakeBox(
		OutDrawElements,
		LayerToUse,
		AllottedGeometry.ToPaintGeometry(ArriveTangentIconLocation, CONST_TangentSize),
		ArriveTangentSelected ? TangentBrushSelected : TangentBrush,
		DrawEffects,
		ArriveTangentSelected ? TangentBrushSelected->GetTint(InWidgetStyle) : TangentBrush->GetTint(InWidgetStyle) * InWidgetStyle.GetColorAndOpacityTint()
	);


	//Leave tangent control arrow
	const FVector2D DirectionUnnormalized = LeaveTangentLocation - ArriveTangentLocation;
	const float AngleInRadians = DirectionUnnormalized.IsNearlyZero() ? 0.0f : FMath::Atan2(DirectionUnnormalized.Y, DirectionUnnormalized.X);
	const float ZoomFactor = 1.3 * CONST_TangentSize.X / ArrowImage->ImageSize.X;
	const FVector2D ArrowRadius = ArrowImage->ImageSize * ZoomFactor * 0.5f;
	const FVector2D ArrowDrawLocation = LeaveTangentLocation - ArrowRadius;

	FSlateDrawElement::MakeRotatedBox(
		OutDrawElements,
		LayerToUse,
		AllottedGeometry.ToPaintGeometry(ArrowDrawLocation, ArrowImage->ImageSize * ZoomFactor),
		ArrowImage,
		DrawEffects,
		AngleInRadians,
		TOptional<FVector2D>(),
		FSlateDrawElement::RelativeToElement,
		LeaveTangentSelected ? TangentBrushSelected->GetTint(InWidgetStyle) : TangentBrush->GetTint(InWidgetStyle) * InWidgetStyle.GetColorAndOpacityTint()
	);
}

float SUMGSplineEditPanel::CalculateGridPixelSpacing() const 
{
	const float MinGridPixelSpacing = 10.0f;
	const float MaxGridPixelSpacing = 20.0f;

	const float ScaleStep = 0.20f;
	float Alpha = FMath::Frac(TransformInfo.Scale / ScaleStep);
	return FMath::Lerp(MinGridPixelSpacing, MaxGridPixelSpacing, Alpha);
}

void SUMGSplineEditPanel::PaintGridLines(const FGeometry& AllottedGeometry, FSlateWindowElementList& OutDrawElements,
	int32 LayerId, const FSlateRect& MyCullingRect, ESlateDrawEffect DrawEffects) const
{
	float GridPixelSpacing = CalculateGridPixelSpacing();

	const FLinearColor GridColor(0.35f, 0.35f, 0.35f, 0.2f);
	const FLinearColor BoarderColor(0.35f, 0.35f, 0.35f, 1.0f);

	//Vertical grid
	{
		TArray<FVector2D> LinePoints;

		//draw vertical grid lines
		float StartOffset = FMath::Fmod(-TransformInfo.Offset.X, GridPixelSpacing);
		for (float X = StartOffset; X < AllottedGeometry.GetLocalSize().X; X += GridPixelSpacing)
		{
			if (SMALL_NUMBER < FMath::Abs(X)) //don't show at 0 to avoid overlapping with center axis 
			{
				LinePoints.Add(FVector2D(X, 0.0));
				LinePoints.Add(FVector2D(X, AllottedGeometry.GetLocalSize().Y));
				FSlateDrawElement::MakeLines(
					OutDrawElements,
					LayerId,
					AllottedGeometry.ToPaintGeometry(),
					LinePoints,
					DrawEffects,
					GridColor,
					false);

				LinePoints.Empty();
			}
		}
	}

	//Horizontal grid
	{
		TArray<FVector2D> LinePoints;

		float StartOffset = FMath::Fmod(-TransformInfo.Offset.Y, GridPixelSpacing);;

		for (float Y = StartOffset; Y < AllottedGeometry.GetLocalSize().Y; Y += GridPixelSpacing)
		{
			if (SMALL_NUMBER < FMath::Abs(Y)) //don't show at 0 to avoid overlapping with center axis 
			{
				LinePoints.Add(FVector2D(0.0f, Y));
				LinePoints.Add(FVector2D(AllottedGeometry.GetLocalSize().X, Y));
				FSlateDrawElement::MakeLines(
					OutDrawElements,
					LayerId,
					AllottedGeometry.ToPaintGeometry(),
					LinePoints,
					DrawEffects,
					GridColor,
					false);

				LinePoints.Empty();
			}
		}
	}

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

bool SUMGSplineEditPanel::IsSplinePointSelected(int PointIndex) const
{
	return PointIndex == SelectedPointIndex;
}

void SUMGSplineEditPanel::GetTangentPoints(const FVector2D& Location, const FVector2D& Direction, FVector2D& Arrive, FVector2D& Leave) const
{
	FVector2D ArriveDirection = Direction;
	ArriveDirection.Normalize();

	FVector2D LeaveDirection = Direction;
	LeaveDirection.Normalize();

	float Alpha = FVector2D::Distance(Direction, FVector2D::ZeroVector) / CONST_TangentMaxStrength;
	Alpha = FMath::Clamp(Alpha, 0.01f, 1.0f);

	float KeyTangentOffset = FMath::Lerp(CONST_KeyTangentOffsetMin, CONST_KeyTangentOffsetMax, Alpha);
	Arrive = -ArriveDirection * KeyTangentOffset + Location;

	Leave = LeaveDirection * KeyTangentOffset + Location;
}

/*===========================================================================*\
|                                Click                                        |
\*===========================================================================*/

void SUMGSplineEditPanel::ProcessClick(const FGeometry& InMyGeometry, const FPointerEvent& InMouseEvent)
{
	const bool bLeftMouseButton = InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton;
	const bool bRightMouseButton = InMouseEvent.GetEffectingButton() == EKeys::RightMouseButton;

	int HitPointIndex = HitTestSplinePoints(InMyGeometry, InMouseEvent.GetScreenSpacePosition());
	FSelectedSplineTangent HitTangent = HitTestCubicTangents(InMyGeometry, InMouseEvent.GetScreenSpacePosition());

	if (bLeftMouseButton)
	{
		// If the user left clicked a key, update selection based on modifier key state.
		if (HitPointIndex != -1)
		{
			SelectedPointIndex = HitPointIndex;
		}
		else if (HitTangent.IsValid())
		{
			SelectedTangent = HitTangent;
			SelectedPointIndex = HitTangent.PointIndex;
		}
	}
	else if (bRightMouseButton)
	{
		bool bShowAddNewSplinePoint = true;

		// If the user right clicked, handle opening context menus.
		if (HitPointIndex != -1)
		{
			SelectedPointIndex = HitPointIndex;
			bShowAddNewSplinePoint = false;
		}
		/*else if (HitTangent.IsValid())
		{
			SelectedTangent = HitTangent;
			SelectedPointIndex = HitTangent.PointIndex;
		}*/

		CreateContextMenu(InMyGeometry, InMouseEvent, bShowAddNewSplinePoint);
	}
}

void SUMGSplineEditPanel::CreateContextMenu(const FGeometry& InMyGeometry, const FPointerEvent& InMouseEvent, bool bShowAddNewSplinePoint)
{
	const FVector2D& ScreenPosition = InMouseEvent.GetScreenSpacePosition();

	const bool CloseAfterSelection = true;
	FMenuBuilder MenuBuilder(CloseAfterSelection, NULL);

	MenuBuilder.BeginSection("EditUMGSplineActions", LOCTEXT("Actions", "Actions"));
	{
		if (bShowAddNewSplinePoint)
		{
			FText MenuItemLabel = LOCTEXT("AddPointToSplineLabel", "Add point to spline");
			FText MenuItemToolTip = LOCTEXT("AddPointToSplineToolTip", "Add a new spline point at the hovered place to the spline.");

			FVector2D Position = InMouseEvent.GetScreenSpacePosition();

			FUIAction Action = FUIAction(FExecuteAction::CreateSP(this, &SUMGSplineEditPanel::AddNewSplinePoint, InMyGeometry, Position));
			MenuBuilder.AddMenuEntry(MenuItemLabel, MenuItemToolTip, FSlateIcon(), Action);
		}
		
		if (SelectedPointIndex != -1)
		{
			FText MenuItemLabel = LOCTEXT("DeleteSelectedPointFromSplineLabel", "Delete selected point from spline");
			FText MenuItemToolTip = LOCTEXT("DeleteSelectedPointFromSplineToolTip", "Delete selected point from the spline.");

			FVector2D Position = InMouseEvent.GetScreenSpacePosition();

			FUIAction Action = FUIAction(FExecuteAction::CreateSP(this, &SUMGSplineEditPanel::DeleteSelectedSplinePoint));
			MenuBuilder.AddMenuEntry(MenuItemLabel, MenuItemToolTip, FSlateIcon(), Action);
		}
	}
	MenuBuilder.EndSection();

	FWidgetPath WidgetPath = InMouseEvent.GetEventPath() != nullptr ? *InMouseEvent.GetEventPath() : FWidgetPath();
	FSlateApplication::Get().PushMenu(SharedThis(this), WidgetPath, MenuBuilder.MakeWidget(), FSlateApplication::Get().GetCursorPos(), FPopupTransitionEffect(FPopupTransitionEffect::ContextMenu));
}

void SUMGSplineEditPanel::AddNewSplinePoint(FGeometry InMyGeometry, FVector2D ScreenPosition)
{
	const FScopedTransaction Transaction(LOCTEXT("EditUMGSpline_AddNewSplinePoint", "Add New Spline Point"));

	const FVector2D MousePosition = InMyGeometry.AbsoluteToLocal(ScreenPosition);

	FUMGSplinePoint NewPoint;
	NewPoint.Location = TransformInfo.LocalToInput(MousePosition);
	NewPoint.Direction = FVector2D(1.0f, 0.0f);

	FUMGSplineInfo NewSplineInfo = SplineInfo.Get();
	NewSplineInfo.AddPoint(NewPoint);
	OnSplineInfoValueChanged.ExecuteIfBound(NewSplineInfo);

	SelectedPointIndex = NewSplineInfo.Points.Num() - 1;
}

void SUMGSplineEditPanel::DeleteSelectedSplinePoint()
{
	const FScopedTransaction Transaction(LOCTEXT("EditUMGSpline_RemoveSplinePoint", "Delete Spline Point"));
	
	if (SelectedPointIndex != -1 && SplineInfo.Get().Points.Num() > SelectedPointIndex)
	{
		FUMGSplineInfo NewSplineInfo = SplineInfo.Get();
		NewSplineInfo.DeletePoint(SelectedPointIndex);
		OnSplineInfoValueChanged.ExecuteIfBound(NewSplineInfo);

		SelectedPointIndex = -1;
	}
}

/*===========================================================================*\
|                                Zoom to fit                                  |
\*===========================================================================*/

void SUMGSplineEditPanel::DeferredZoomToFit()
{
	bDeferredZoomToFit = true;
}

EActiveTimerReturnType SUMGSplineEditPanel::HandleZoomToFit(double InCurrentTime, float InDeltaTime)
{
	bool bDoneZooming = ZoomToFit(true, true);

	if (bDoneZooming)
	{
		return EActiveTimerReturnType::Stop;
	}

	return EActiveTimerReturnType::Continue;
}

bool SUMGSplineEditPanel::ZoomToFit(bool bFitHorizontal, bool bFitVertical)
{
	float InMinX = FLT_MAX;
	float InMaxX = -FLT_MAX;
	float InMinY = FLT_MAX;
	float InMaxY = -FLT_MAX;

	const TArray<FUMGSplinePoint>& Points = SplineInfo.Get().GetPoints();

	// Iterate over each key
	for (int32 i = 0; i < Points.Num(); ++i)
	{
		InMinX = FMath::Min(Points[i].Location.X, InMinX);
		InMaxX = FMath::Max(Points[i].Location.X, InMaxX);
		InMinY = FMath::Min(Points[i].Location.Y, InMinY);
		InMaxY = FMath::Max(Points[i].Location.Y, InMaxY);
	}

	//if (bFitHorizontal)
	{
		if (Points.Num() > 0)
		{
			// Clamp the minimum size
			float SizeX = InMaxX - InMinX;
			if (SizeX < CONST_MinViewRange)
			{
				InMinX -= (0.5f * CONST_MinViewRange);
				InMaxX += (0.5f * CONST_MinViewRange);
				SizeX = InMaxX - InMinX;
			}

			// add margin
			InMinX -= CONST_FitMargin * SizeX;
			InMaxX += CONST_FitMargin * SizeX;
		}
		else
		{
			InMinX = -CONST_FitMargin * 2.0f;
			InMaxX = (CONST_DefaultZoomRange + CONST_FitMargin) * 2.0;
		}
	}
	
	//if (bFitVertical)
	{
		if (Points.Num() > 0)
		{
			// Clamp the minimum size
			float SizeY = InMaxY - InMinY;
			if (SizeY < CONST_MinViewRange)
			{
				InMinY -= (0.5f * CONST_MinViewRange);
				InMaxY += (0.5f * CONST_MinViewRange);
				SizeY = InMaxY - InMinY;
			}

			// add margin
			InMinY -= CONST_FitMargin * SizeY;
			InMaxY += CONST_FitMargin * SizeY;
		}  
		else
		{
			InMinY = -CONST_FitMargin * 2.0f;
			InMaxY = (CONST_DefaultZoomRange + CONST_FitMargin) * 2.0;
		}
	}

	FVector2D LocalSize = GetCachedGeometry().GetLocalSize();
	if (LocalSize == FVector2D::ZeroVector)
	{
		return false;
	}

	if (bFitHorizontal && bFitVertical)
	{
		float ScaleX = LocalSize.X / (InMaxX - InMinX);
		float ScaleY = LocalSize.Y / (InMaxY - InMinY);
		float Scale = FMath::Min(ScaleX, ScaleY);

		FVector2D LocalCenter = LocalSize / 2;
		FVector2D InputCenter = FVector2D(InMaxX + InMinX, InMaxY + InMinY) / 2;
		FVector2D Offset = InputCenter - LocalCenter / Scale;

		TransformInfo.Scale = Scale;
		TransformInfo.Offset = Offset;
	}
	else if (bFitHorizontal)
	{
		float Scale = LocalSize.X / (InMaxX - InMinX);
		
		FVector2D LocalCenter = LocalSize / 2;
		FVector2D InputCenter = FVector2D(InMaxX + InMinX, InMaxY + InMinY) / 2;
		FVector2D Offset = InputCenter - LocalCenter / Scale;

		TransformInfo.Scale = Scale;
		TransformInfo.Offset = Offset;
	}
	else if (bFitVertical)
	{
		float Scale = LocalSize.Y / (InMaxY - InMinY);

		FVector2D LocalCenter = LocalSize / 2;
		FVector2D InputCenter = FVector2D(InMaxX + InMinX, InMaxY + InMinY) / 2;
		FVector2D Offset = InputCenter - LocalCenter / Scale;

		TransformInfo.Scale = Scale;
		TransformInfo.Offset = Offset;
	}

	return true;
}

FReply SUMGSplineEditPanel::ZoomToFitClicked()
{
	ZoomToFit(true, true);
	return FReply::Handled();
}

/*===========================================================================*\
|                                Adjust panel height                          |
\*===========================================================================*/

TSharedRef<SWidget> SUMGSplineEditPanel::GetPanelHeightMenu()
{
	TSharedRef<SWidget> FillWidget = SNew(SBorder)
		.BorderImage(FAppStyle::GetBrush(TEXT("Menu.Background")))
		[
			SNew(SVerticalBox)
			+ SVerticalBox::Slot()
			.AutoHeight()
			.Padding(FMargin(8.0f, 2.0f, 60.0f, 2.0f))
			.HAlign(HAlign_Left)
			[
				SNew(STextBlock)
				.Text(LOCTEXT("UMGSplineEditPanelHeight", "Panel Height"))
				.Font(FAppStyle::GetFontStyle(TEXT("MenuItem.Font")))
			]
			+ SVerticalBox::Slot()
			.AutoHeight()
			.Padding(FMargin(8.0f, 4.0f))
			[
				SNew(SHorizontalBox)
				+ SHorizontalBox::Slot()
				.FillWidth(1)
				.Padding(FMargin(0.0f, 2.0f))
				[
					SNew(SSlider)
					.Value(this, &SUMGSplineEditPanel::GetPanelHeightSliderPosition)
					.OnValueChanged(this, &SUMGSplineEditPanel::OnSetPanelHeight)
				]
				+ SHorizontalBox::Slot()
				.AutoWidth()
				.Padding(8.0f, 2.0f, 0.0f, 2.0f)
				[
					SNew(STextBlock)
					.Text(this, &SUMGSplineEditPanel::GetPanelHeightLabel)
					.Font(FAppStyle::GetFontStyle(TEXT("MenuItem.Font")))
				]
			]

		];

	return FillWidget;
}

FText SUMGSplineEditPanel::GetPanelHeightLabel() const
{
	FNumberFormattingOptions Options;
	Options.SetMaximumFractionalDigits(0);
	return FText::AsNumber(UUMGSplineEditorSettings::GetPanelHeight(), &Options);
}


float SUMGSplineEditPanel::GetPanelHeightSliderPosition() const
{
	float Value = (UUMGSplineEditorSettings::GetPanelHeight() - CONST_PanelHeightMin) / (CONST_PanelHeightMax - CONST_PanelHeightMin);
	return Value;
}


void SUMGSplineEditPanel::OnSetPanelHeight(float NewValue)
{
	float DesiredHeight = FMath::Lerp(CONST_PanelHeightMin, CONST_PanelHeightMax, NewValue);
	UUMGSplineEditorSettings::SetPanelHeight(DesiredHeight);
}

/*===========================================================================*\
|                                Adjust spline thickness                      |
\*===========================================================================*/

TSharedRef<SWidget>  SUMGSplineEditPanel::GetThicknessFillMenu()
{
	TSharedRef<SWidget> FillWidget = SNew(SBorder)
		.BorderImage(FAppStyle::GetBrush(TEXT("Menu.Background")))
		[
			SNew(SVerticalBox)
			+ SVerticalBox::Slot()
				.AutoHeight()
				.Padding(FMargin(8.0f, 2.0f, 60.0f, 2.0f))
				.HAlign(HAlign_Left)
				[
					SNew(STextBlock)
					.Text(LOCTEXT("SplineEditSplineThickness", "Spline Thickness"))
					.Font(FAppStyle::GetFontStyle(TEXT("MenuItem.Font")))
				]
			+ SVerticalBox::Slot()
				.AutoHeight()
				.Padding(FMargin(8.0f, 4.0f))
				[
					SNew(SHorizontalBox)
					+ SHorizontalBox::Slot()
						.FillWidth(1)
						.Padding(FMargin(0.0f, 2.0f))
						[
							SNew(SSlider)
							.Value(this, &SUMGSplineEditPanel::GetSplineThicknessSliderPosition)
							.OnValueChanged(this, &SUMGSplineEditPanel::OnSetSplineThickness)
						]
					+ SHorizontalBox::Slot()
						.AutoWidth()
						.Padding(8.0f, 2.0f, 0.0f, 2.0f)
						[
							SNew(STextBlock)
							.Text(this, &SUMGSplineEditPanel::GetSplineThicknessLabel)
							.Font(FAppStyle::GetFontStyle(TEXT("MenuItem.Font")))
						]
				] 
	
		];

	return FillWidget;
}

FText SUMGSplineEditPanel::GetSplineThicknessLabel() const
{
	FNumberFormattingOptions Options;
	Options.SetMaximumFractionalDigits(0);
	return FText::AsNumber(SplineInfo.Get().Thickness, &Options);
}


float SUMGSplineEditPanel::GetSplineThicknessSliderPosition() const
{
	float Value = (SplineInfo.Get().Thickness - CONST_SplineThicknessMin) / (CONST_SplineThicknessMax - CONST_SplineThicknessMin);
	return Value;
}


void SUMGSplineEditPanel::OnSetSplineThickness(float NewValue)
{
	float NewThickness = FMath::Lerp(CONST_SplineThicknessMin, CONST_SplineThicknessMax, NewValue);

	FUMGSplineInfo NewSplineInfo = SplineInfo.Get();
	NewSplineInfo.Thickness = NewThickness;
	OnSplineInfoValueChanged.ExecuteIfBound(NewSplineInfo);
}


/*===========================================================================*\
|                                Help text                                    |
\*===========================================================================*/

FText SUMGSplineEditPanel::GetHelpText() const
{
	FText HelpText = LOCTEXT("SplineEditHelpText", "\
How to edit a spline?\n\n\
Left mouse click:\n   Pick a spline point or tangent.\n\n\
Right mouse click:\n   Add a spline point at mouse location or delete selected spline point.\n\n\
Mouse wheel:\n   Zoom the editor view. Before zooming you need to click on the edit panel to focus on it.\n\n\
Left mouse drag:\n   Adjust spline point location, or adjust spline tangent direction and strength.\n\n\
Right mouse drag:\n   Pan the editor view.");

	return HelpText;
}

#undef LOCTEXT_NAMESPACE