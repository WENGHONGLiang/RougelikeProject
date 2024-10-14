// Copyright Qibo Pang 2022. All Rights Reserved.

#include "UMGSplineDefine.h"
#include "Styling/SlateBrush.h"
#include "Materials/MaterialInterface.h"

FUMGSplineInfo::FUMGSplineInfo(bool Init)
{
    if (Init)
    {
        Points.Add(FUMGSplinePoint(FVector2D(0.0f, 0.0f), FVector2D(200.0f, 0.0f)));
        Points.Add(FUMGSplinePoint(FVector2D(200.0f, 200.0f), FVector2D(200.0f, 0.0f)));

        UMaterialInterface* SplineMaterial = LoadObject<UMaterialInterface>(nullptr, TEXT("/UMGSpline/Materials/M_Spline_AntiAliasing"));
        if (SplineMaterial)
        {
            CustomVertsBrush.SetResourceObject(SplineMaterial);
        }

        SplineType = EUMGSplineType::Curve;
    }
}