// Copyright Qibo Pang 2022. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Dom/JsonObject.h"	// Json
#include "UMGSplineDefine.generated.h"

/** Enumerates extrapolation options. */
UENUM(BlueprintType)
enum class EUMGSplineType : uint8
{
    /** Use a linearly increasing value for extrapolation.*/
    Linear UMETA(DisplayName = "Linear"),
    /** Use a bezier increasing value for extrapolation */
    Curve UMETA(DisplayName = "Curve")
};

/** */
USTRUCT(BlueprintType)
struct UMGSPLINE_API FUMGSplinePoint
{
    GENERATED_BODY()

    FUMGSplinePoint() = default;
    FUMGSplinePoint(FVector2D InLocation, FVector2D InDirection):
        Location(InLocation),
        Direction(InDirection)
    { }

public:

    /**  */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UMGSpline")
        FVector2D Location;

    /**  */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UMGSpline")
        FVector2D Direction;
};

/** */
USTRUCT(BlueprintType)
struct UMGSPLINE_API FUMGSplineInfo
{
    GENERATED_BODY()

        FUMGSplineInfo() = default;

        FUMGSplineInfo(bool Init);

public:

    void AddPoint(const FUMGSplinePoint& SplinePoint)
    {
        Points.Add(SplinePoint);
    }
    
    void ChangeSplinePointAtIndex(const FUMGSplinePoint& SplinePoint, int32 Index)
    {
        if (Index >= 0 && Index < Points.Num()) {
            Points[Index] = SplinePoint;
        }
    }

    void AddSplinePointAtIndex(const FUMGSplinePoint& SplinePoint, int32 Index)
    {
        if (Index >= 0 && Index < Points.Num()) {
            Points.Insert(SplinePoint, Index);
        }
    }

    void DeletePoint(int Index)
    {
        if (Points.IsValidIndex(Index))
        {
            Points.RemoveAt(Index);
        }
    }


    /** Finds a key a the specified time */
    FUMGSplinePoint FindPoint(int Index) const
    {
        if (Points.IsValidIndex(Index))
        {
            return Points[Index];
        }
        return FUMGSplinePoint();
    }

    int GetPointNum() const { return Points.Num(); }

    const TArray<FUMGSplinePoint>& GetPoints() const { return Points; }

public:

    /** 
    * SplineType = EUMGSplineType::Curve, draw bezier splines.
    * SplineType = EUMGSplineType::Linear, draw straight lines.
    */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UMGSpline")
        EUMGSplineType SplineType = EUMGSplineType::Curve;

    /** Line thickness for this spline */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UMGSpline")
        float Thickness = 1.0f;

    /** Points of this spline */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UMGSpline")
        TArray<FUMGSplinePoint> Points;

    /**
     * Whether the spline is to be considered as a closed loop.
     */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UMGSpline")
       bool bClosedLoop = false;

    /** Tint color for this spline */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UMGSpline")
        FLinearColor TintColor = FLinearColor(1.0f, 1.0f, 1.0f, 1.0f);

    /**
    Build custom verts for spline.

    When BuildCustomVerts == true, the spline geometry will be built as custom vertexs. 
    The advantage is that you can set a brush for the spline geometry, and set your own spline materials.
    The disadvantage is that it cost more when drawing a lot of splines.

    Notice: when using custom spline materials, you need call MF_SplineAntiAlisaing in your material for smooth spline. 

    When BuildCustomVerts == false, the spline geometry will be built with unreal engine default functions, which means the spline geometry 
    will be batched with other splines. 
    The advantage is that you will get good performance when drawing a lot of splines. 
    The disadvantage is taht you can not set a brush for the spline geometry. 

    */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UMGSpline")
        bool BuildCustomVerts = true;

    /** The image/material used for the custom verts spline geometry*/
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UMGSpline", meta = (EditCondition = "BuildCustomVerts"))
        FSlateBrush CustomVertsBrush;

    /** The uv.v scale for the custom verts spline geometry*/
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UMGSpline", meta = (EditCondition = "BuildCustomVerts"))
        float CustomVertsVCoordScale = 1.0;
};

/** */
USTRUCT(BlueprintType)
struct UMGSPLINE_API FUMGSplineDebugInfo
{
    GENERATED_BODY()

        FUMGSplineDebugInfo() { }

public:

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UMGSpline")
        bool bDrawWidgetBoarder = false;

    /** Tint color for this spline */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UMGSpline")
        FLinearColor BoarderColor = FLinearColor(1.0f, 1.0f, 1.0f, 1.0f);
};

/** Notification for FUMGSplineInfo value change */
DECLARE_DELEGATE_OneParam(FOnSplineInfoValueChanged, const FUMGSplineInfo&)
