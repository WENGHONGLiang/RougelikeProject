// Copyright Qibo Pang 2022. All Rights Reserved.


#pragma once

// Core Include
#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

// Misc Includes
#include "Misc/Attribute.h"
#include "Input/Reply.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Framework/SlateDelegates.h"
#include "Rendering/DrawElements.h"
#include "Styling/CoreStyle.h"
#include "UMGSplineDefine.h"

class FPaintArgs;
class FSlateWindowElementList;
struct FSlateBrush;

struct FSelectedSplineTangent
{
    FSelectedSplineTangent() {}

    bool IsValid() { return PointIndex != -1; }

    int32 PointIndex = -1;

    bool bIsArrial = false;
};

struct FSplineEditPanelTransform
{
    FVector2D  Offset;
    float      Scale;

    FSplineEditPanelTransform()
    {
        Offset = FVector2D::ZeroVector;
        Scale = 1.0;
    }

    /** Local Widget Space -> Spline Input domain. */
    FVector2D LocalToInput(FVector2D Local) const
    {
        return (Scale != 0.0f) ? (Local / Scale + Offset) : Offset;
    }

    /** Spline Input domain. -> Local Widget Space*/
    FVector2D InputToLocal(FVector2D Input) const
    {
        return (Input - Offset) * Scale;
    }
};

/**
 * Edit panel for a spline
 */
class SUMGSplineEditPanel : public SCompoundWidget
{
public:

    SLATE_BEGIN_ARGS(SUMGSplineEditPanel)
        : _SplineInfo()
        , _OnSplineInfoValueChanged()
        { }

        /// Attributes ///
    
        SLATE_ATTRIBUTE(FUMGSplineInfo, SplineInfo)

        /// Arguments ///
   

        /// Events ///
   
        /** Invoked when a new value is selected on the spline. */
        SLATE_EVENT(FOnSplineInfoValueChanged, OnSplineInfoValueChanged)

    SLATE_END_ARGS()

protected:

    /** Represents the different states of a drag operation. Copy from SCurveEditor.h */
    enum class EDragState
    {
        /** The user has clicked a mouse button, but hasn't moved more then the drag threshold. */
        PreDrag,
        /** The user is dragging the selected keys. */
        DragKey,
        /** The user is free dragging the selected keys. */
        FreeDrag,
        /** The user is dragging a selected tangent handle. */
        DragTangent,
        /** The user is panning the curve view. */
        Pan,
        /** The user is zooming the curve view. */
        Zoom,
        /** There is no active drag operation. */
        None
    };


public:

    /**
     * Construct this widget.
     *
     * @param InArgs The declaration data for this widget.
     */
    void Construct(const FArguments& InArgs);
	
    
#pragma region Overrides
    
    // SWidget overrides
    virtual void Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime) override;

    virtual FVector2D ComputeDesiredSize(float) const override;
    
    virtual FReply OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
    
    virtual FReply OnMouseButtonUp(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
    
    virtual FReply OnMouseMove(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
    
    virtual void   OnMouseCaptureLost(const FCaptureLostEvent& CaptureLostEvent) override;

    virtual FReply OnMouseWheel(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;

    virtual void OnMouseLeave(const FPointerEvent& MouseEvent) override;

    virtual FReply OnKeyDown(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent) override;

    virtual int32 OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect,
                          FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle,
                          bool bParentEnabled) const override;

#pragma endregion Overrides


protected:

    /** Paint a spline with engine default draw function*/
    void PaintSpline_Default(const FUMGSplineInfo& InSplineInfo, const FGeometry& AllottedGeometry, FSlateWindowElementList& OutDrawElements,
        int32 LayerId, const FSlateRect& MyCullingRect, ESlateDrawEffect DrawEffects, const FWidgetStyle& InWidgetStyle) const;

    /** Paint a spline with custom verts draw function*/
    void PaintSpline_CustomVerts(const FUMGSplineInfo& InSplineInfo, const FGeometry& AllottedGeometry, FSlateWindowElementList& OutDrawElements,
        int32 LayerId, const FSlateRect& MyCullingRect, ESlateDrawEffect DrawEffects, const FWidgetStyle& InWidgetStyle) const;

    /** Paint the keys that make up a curve */
    void PaintSplinePoints(const FUMGSplineInfo& InSplineInfo, FSlateWindowElementList& OutDrawElements, int32 LayerId, int32 SelectedLayerId, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, ESlateDrawEffect DrawEffects, const FWidgetStyle& InWidgetStyle) const;

    /** Paint the tangent for a key with cubic curves */
    void PaintTangent(const FUMGSplinePoint& SplinePoint, FSlateWindowElementList& OutDrawElements, int32 LayerId,
        const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, ESlateDrawEffect DrawEffects, int32 LayerToUse, const FWidgetStyle& InWidgetStyle, bool bTangentSelected) const;

    /** Paint Grid lines, these make it easier to visualize relative distance */
    void PaintGridLines(const FGeometry& AllottedGeometry, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FSlateRect& MyCullingRect, ESlateDrawEffect DrawEffects)const;

    bool IsSplinePointSelected(int PointIndex) const;

    void GetTangentPoints(const FVector2D& Location, const FVector2D& Direction, FVector2D& Arrive, FVector2D& Leave) const;

    /** Attempts to start a drag operation when the mouse moves. */
    void TryStartDrag(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent);

    /** Processes an ongoing drag operation when the mouse moves. */
    void ProcessDrag(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent);

    /** Completes an ongoing drag operation on mouse up. */
    void EndDrag(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent);

    /** Zoom the view. */
    void ZoomView(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent);

    /** Test a screen space location to find which key was clicked on */
    int32 HitTestSplinePoints(const FGeometry& InMyGeometry, const FVector2D& HitScreenPosition);

    /** Test a screen space location to find if any cubic tangents were clicked on */
    FSelectedSplineTangent HitTestCubicTangents(const FGeometry& InMyGeometry, const FVector2D& HitScreenPosition);


    /** Handles a mouse click operation on mouse up */
    void ProcessClick(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent);

    /** Function to create context menu on mouse right click*/
    void CreateContextMenu(const FGeometry& InMyGeometry, const FPointerEvent& InMouseEvent, bool bShowAddNewSplinePoint);

    /** Adds a new spline point to the spline. */
    void AddNewSplinePoint(FGeometry InMyGeometry, FVector2D ScreenPosition);

    /** Called by delete command */
    void DeleteSelectedSplinePoint();

    float CalculateGridPixelSpacing() const;

    /** Defered Zoom to fit */
    void DeferredZoomToFit();

    /** Active timer that handles deferred zooming until the target zoom is reached */
    EActiveTimerReturnType HandleZoomToFit(double InCurrentTime, float InDeltaTime);

    /** Zoom to fit */
    bool ZoomToFit(bool bFitHorizontal, bool bFitVertical);

    /** Zoom to fit button clicked*/
    FReply ZoomToFitClicked();

    /** Function to create menu on adjust panel height button click*/
    TSharedRef<SWidget> GetPanelHeightMenu();

    /** Panel height Label callback */
    FText GetPanelHeightLabel() const;

    /** Returns the current panel height setting */
    float GetPanelHeightSliderPosition() const;

    /**
     * Sets new panel height
     *
     * @Param	NewValue	Value to set panel height too
     */
    void OnSetPanelHeight(float NewValue);

    /** Function to create menu on spline thickness button click*/
    TSharedRef<SWidget> GetThicknessFillMenu();

    /** Spline Thickness Label callback */
    FText GetSplineThicknessLabel() const;

    /** Returns  the spline thickness */
    float GetSplineThicknessSliderPosition() const;

    /**
     * Sets new panel height
     *
     * @Param	NewValue	Value to set the spline thickness
     */
    void OnSetSplineThickness(float NewValue);

    /** Returns the help text */
    FText GetHelpText() const;

private:

    TAttribute<FUMGSplineInfo> SplineInfo;

private:

    /** The location of mouse during the last OnMouseButtonDown callback in widget local coordinates. */
    FVector2D MouseDownLocation;

    /** The location of the mouse during the last OnMouseMove callback in widget local coordinates. */
    FVector2D MouseMoveLocation;

    /** The state of the current drag operation happening with the widget, if any. */
    EDragState DragState;

    /** The number of pixels which the mouse must move before a drag operation starts. */
    float DragThreshold;

    /** A map of selected key handle to their starting locations at the beginning of a drag operation. */
    TMap<int32, FVector2D> PreDragPointLocations;

    /** A map of selected key handles to their tangent values at the beginning of a drag operation. */
    TMap<int32, FVector2D> PreDragTangents;

    FSelectedSplineTangent SelectedTangent;

    int32 DraggedPointIndex;

    int32 SelectedPointIndex;

    FOnSplineInfoValueChanged OnSplineInfoValueChanged;

    FSplineEditPanelTransform TransformInfo;

    bool HideUnselectedTangents = false;

    bool bDeferredZoomToFit = false;

    bool bEditPanelFocus = false;

    TSharedPtr< SImage > HelpIconImage;
};
