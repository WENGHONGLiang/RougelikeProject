// Copyright Qibo Pang 2022. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "UMGSplineEditorSettings.generated.h"


/**
 * Rhino 客户端应用工程全局配置
 */
UCLASS(config = UMGSplineEditor, defaultconfig)
class UUMGSplineEditorSettings : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, config, Category = UMGSplineEditor)
		float PanelHeight = 600.0f;	

	static float GetPanelHeight()
	{
		UUMGSplineEditorSettings* Settings = GetMutableDefault<UUMGSplineEditorSettings>();
		if (Settings)
		{
			return Settings->PanelHeight;
		}
		return 600.0f;
	}

	static void SetPanelHeight(float PanelHeight)
	{
		UUMGSplineEditorSettings* Settings = GetMutableDefault<UUMGSplineEditorSettings>();
		if (Settings)
		{
			Settings->PanelHeight = PanelHeight;
		}
	}
};
