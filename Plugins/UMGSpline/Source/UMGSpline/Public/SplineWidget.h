// Copyright Qibo Pang 2022. All Rights Reserved.

#pragma once

// Core
#include "CoreMinimal.h"
#include "Components/Widget.h"

// Misc
#include "SWSpline.h"
#include "Styling/CoreStyle.h"
#include "UMGSplineDefine.h"

// Generated.h
#include "SplineWidget.generated.h"

class SWSpline;
class UMaterialInterface;

/** Types of coordinate space accepted by the functions. */
UENUM()
namespace EUMGSplineCoordinateSpace
{
	enum Type
	{
		Local,
		Viewport,
		Screen
	};
}

USTRUCT()
struct UMGSPLINE_API FUMGSplineCurves
{
	GENERATED_BODY()

	/** Spline built from position data. */
	UPROPERTY()
		FInterpCurveVector2D Position;

	/** Input: distance along curve, output: parameter that puts you there. */
	UPROPERTY()
		FInterpCurveFloat ReparamTable;

	UPROPERTY(transient)
		uint32 Version = 0xffffffff;

	UPROPERTY(transient)
		int32 ReparamStepsPerSegment;

	bool operator==(const FUMGSplineCurves& Other) const
	{
		return Position == Other.Position;
	}

	bool operator!=(const FUMGSplineCurves& Other) const
	{
		return !(*this == Other);
	}

	/**
	 * Update the spline's internal data according to the passed-in params
	 * @param	SplineInfo				Spline Information
	 * @param	bStationaryEndpoints	Whether the endpoints of the spline are considered stationary when traversing the spline at non-constant velocity.  Essentially this sets the endpoints' tangents to zero vectors.
	 * @param	ReparamStepsPerSegment	Number of steps per spline segment to place in the reparameterization table
	 * @param	bClosedLoopPositionOverride	Whether to override the loop position with LoopPosition
	 * @param	LoopPosition			The loop position to use instead of the last key
	 * @param	Scale3D					The world scale to override
	 */
	void UpdateSpline(const FUMGSplineInfo& SplineInfo, bool bStationaryEndpoints = false, int32 InReparamStepsPerSegment = 10, bool bClosedLoopPositionOverride = false, float LoopPosition = 0.0f, const FVector2D& Scale2D = FVector2D(1.0f));

	/** Returns the length of the specified spline segment up to the parametric value given */
	float GetSegmentLength(const int32 Index, const float Param, bool bClosedLoop = false, const FVector2D& Scale2D = FVector2D(1.0f)) const;

	/** Returns total length along this spline */
	float GetSplineLength() const;
};

//This plugin is finished during Chinese Lunar New Year. Happy tiger year!

/**
 * A spline wdiget can be: 
 * 1. Easy to edit in UMG editor.
 * 2. Built as custom verts, so that you can set a custom brush for the spline geometry.
 * 3. Used as a rail for other widgets, like USplineComponent in 3D space.
 *
 * * No Children
 */
UCLASS(meta=(DisplayName="Spline"))
class UMGSPLINE_API USplineWidget final : public UWidget
{                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                            
	GENERATED_BODY()

public:

	/*============================================================================\
	|                                Properties                                   |
	\============================================================================*/

	/** Spline Info */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Appearance")
		FUMGSplineInfo SplineInfo = FUMGSplineInfo(/* Init */true);

	/** Debug Info */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debug")
		FUMGSplineDebugInfo SplineDebugInfo;

	/*============================================================================\
	|                            Blueprint Functions                              |
	\============================================================================*/
	
	/** Update the spline tangents and SplineReparamTable */
	UFUNCTION(BlueprintCallable, Category = Spline)
		virtual void UpdateSpline();

	/** Get location along spline at the provided input key value */
	UFUNCTION(BlueprintCallable, Category = Spline)
		FVector2D GetLocationAtSplineInputKey(float InKey, EUMGSplineCoordinateSpace::Type CoordinateSpace) const;

	/** Get tangent along spline at the provided input key value */
	UFUNCTION(BlueprintCallable, Category = Spline)
		FVector2D GetTangentAtSplineInputKey(float InKey, EUMGSplineCoordinateSpace::Type CoordinateSpace) const;

	/** Get unit direction along spline at the provided input key value */
	UFUNCTION(BlueprintCallable, Category = Spline)
		FVector2D GetDirectionAtSplineInputKey(float InKey, EUMGSplineCoordinateSpace::Type CoordinateSpace) const;

	/** Get rotator corresponding to rotation along spline at the provided input key value */
	UFUNCTION(BlueprintCallable, Category = Spline)
		float GetRotationAngleAtSplineInputKey(float InKey, EUMGSplineCoordinateSpace::Type CoordinateSpace) const;

	/** Get distance along the spline at the provided input key value */
	UFUNCTION(BlueprintCallable, Category = Spline)
		float GetDistanceAlongSplineAtSplineInputKey(float InKey) const;

	/** Adds a point to the spline */
	UFUNCTION(BlueprintCallable, Category = Spline)
		void AddSplinePoint(const FUMGSplinePoint& SplinePoint, bool bUpdateSpline);

	/** Adds a point to the spline at the specified index */
	UFUNCTION(BlueprintCallable, Category = Spline)
		void AddSplinePointAtIndex(const FUMGSplinePoint& SplinePoint, int32 Index, bool bUpdateSpline);

	/** Change a point to the spline at the specified index */
	UFUNCTION(BlueprintCallable, Category = Spline)
		void ChangeSplinePointAtIndex(const FUMGSplinePoint& SplinePoint, int32 Index, bool bUpdateSpline);

	/** Removes point at specified index from the spline */
	UFUNCTION(BlueprintCallable, Category = Spline)
		void RemoveSplinePoint(int32 Index, bool bUpdateSpline);

	/** Removes all points from the spline */
	UFUNCTION(BlueprintCallable, Category = Spline)
		void RemoveAllSplinePoint(bool bUpdateSpline);

	/** Set spline thickness */
	UFUNCTION(BlueprintCallable, Category = Spline)
		void SetSplineThickness(float Thickness);

	/** Get spline thickness */
	UFUNCTION(BlueprintCallable, Category = Spline)
		float GetSplineThickness() const;

	/** Set spline material */
	UFUNCTION(BlueprintCallable, Category = Spline)
		void SetSplineMaterial(UMaterialInterface* Material);

	/** Get spline material */
	UFUNCTION(BlueprintCallable, Category = Spline)
		UMaterialInterface* GetSplineMaterial() const;

	/** Get the type of spline */
	UFUNCTION(BlueprintCallable, Category = Spline)
		EUMGSplineType GetSplineType() const;

	/** Specify the type of spline */
	UFUNCTION(BlueprintCallable, Category = Spline)
		void SetSplineType(EUMGSplineType Type);

	/** Get the number of points that make up this spline */
	UFUNCTION(BlueprintCallable, Category = Spline)
		int32 GetNumberOfSplinePoints() const;

	/** Get the location at spline point */
	UFUNCTION(BlueprintCallable, Category = Spline)
		FVector2D GetLocationAtSplinePoint(int32 PointIndex, EUMGSplineCoordinateSpace::Type CoordinateSpace) const;

	/** Get the direction at spline point */
	UFUNCTION(BlueprintCallable, Category = Spline)
		FVector2D GetDirectionAtSplinePoint(int32 PointIndex, EUMGSplineCoordinateSpace::Type CoordinateSpace) const;

	/** Get the tangent at spline point. This fetches the Leave tangent of the point. */
	UFUNCTION(BlueprintCallable, Category = Spline)
		FVector2D GetTangentAtSplinePoint(int32 PointIndex, EUMGSplineCoordinateSpace::Type CoordinateSpace) const;

	/** Returns total length along this spline */
	UFUNCTION(BlueprintCallable, Category = Spline)
		float GetSplineLength() const;

	/** Given a distance along the length of this spline, return the corresponding input key at that point */
	UFUNCTION(BlueprintCallable, Category = Spline)
		float GetInputKeyAtDistanceAlongSpline(float Distance, EUMGSplineCoordinateSpace::Type CoordinateSpace) const;

	/** Given a distance along the length of this spline, return the point in space where this puts you */
	UFUNCTION(BlueprintCallable, Category = Spline)
		FVector2D GetLocationAtDistanceAlongSpline(float Distance, EUMGSplineCoordinateSpace::Type CoordinateSpace) const;

	/** Given a distance along the length of this spline, return a unit direction vector of the spline tangent there. */
	UFUNCTION(BlueprintCallable, Category = Spline)
		FVector2D GetDirectionAtDistanceAlongSpline(float Distance, EUMGSplineCoordinateSpace::Type CoordinateSpace) const;

	/** Given a distance along the length of this spline, return the tangent vector of the spline there. */
	UFUNCTION(BlueprintCallable, Category = Spline)
		FVector2D GetTangentAtDistanceAlongSpline(float Distance, EUMGSplineCoordinateSpace::Type CoordinateSpace) const;

	/** Given a distance along the length of this spline, return a rotation corresponding to the spline's rotation there. */
	UFUNCTION(BlueprintCallable, Category = Spline)
		float GetRotationAngleAtDistanceAlongSpline(float Distance, EUMGSplineCoordinateSpace::Type CoordinateSpace) const;

	
public:

	inline FUMGSplineInfo GetSplineInfo() const { return SplineInfo; }
	inline FUMGSplineDebugInfo GetSplineDebugInfo() const { return SplineDebugInfo; }

protected:
	
	virtual void OnWidgetRebuilt() override;

	// Construct the widget
	virtual TSharedRef<SWidget> RebuildWidget() override;
	
	// Destroy the widget
	virtual void ReleaseSlateResources(bool bReleaseChildren) override;
	
#if WITH_EDITOR
	
	// For the editor to set the name category in the Widget Explorer.
	virtual inline const FText GetPaletteCategory() override { return NSLOCTEXT("Spline", "Spline", "Spline"); };

#endif
	
private:

	/** The dummy value used for queries when there are no point in a spline */
	static const FInterpCurvePointVector2D DummyPointPosition;

	/** Returns a const reference to the specified position point, but gives back a dummy point if there are no points */
	inline const FInterpCurvePointVector2D& GetPositionPointSafe(int32 PointIndex) const
	{
		const TArray<FInterpCurvePointVector2D>& Points = SplineCurves.Position.Points;
		const int32 NumPoints = Points.Num();
		if (NumPoints > 0)
		{
			const int32 ClampedIndex = (SplineInfo.bClosedLoop && PointIndex >= NumPoints) ? 0 : FMath::Clamp(PointIndex, 0, NumPoints - 1);
			return Points[ClampedIndex];
		}
		else
		{
			return DummyPointPosition;
		}
	}

	
private:

	/*============================================================================\
	|                              Private Variables                              |
	\============================================================================*/
	
	// Shared Ptr to the Spline.
	TSharedPtr<SWSpline> Spline;

	UPROPERTY(transient)
		FUMGSplineCurves SplineCurves;

};
