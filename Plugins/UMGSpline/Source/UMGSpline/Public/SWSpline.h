// Copyright Qibo Pang 2022. All Rights Reserved.

#pragma once

// Core Include
#include "CoreMinimal.h"
#include "Widgets/SLeafWidget.h"

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


/**
 * 
 * @Note: Because someone thought hardcoding it is reasonable?
 */
class UMGSPLINE_API SWSpline : public SLeafWidget
{
public:

    SLATE_BEGIN_ARGS(SWSpline)
        : _SplineInfo()
        , _SplineDebugInfo()
        { }

        /// Attributes ///
    
        SLATE_ATTRIBUTE(FUMGSplineInfo, SplineInfo)
        SLATE_ATTRIBUTE(FUMGSplineDebugInfo, SplineDebugInfo)

        /// Arguments ///
   

        /// Events ///

        /** Invoked when a new value is seted on the spline. */
        SLATE_EVENT(FOnLinearColorValueChanged, OnValueChanged)

    SLATE_END_ARGS()

public:

    /**
     * Construct this widget.
     *
     * @param InArgs The declaration data for this widget.
     */
    void Construct(const FArguments& InArgs);
	
    
#pragma region Overrides
    
    // SWidget overrides
    virtual FVector2D ComputeDesiredSize(float) const override;
    
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

    /** Paint widget boarder for debug*/
    void PaintWidgetBoarder(const FGeometry& AllottedGeometry, FSlateWindowElementList& OutDrawElements,
        int32 LayerId, const FSlateRect& MyCullingRect, ESlateDrawEffect DrawEffects) const;

private:

    TAttribute<FUMGSplineInfo> SplineInfo;
    TAttribute<FUMGSplineDebugInfo> SplineDebugInfo;
};
