// Copyright Qibo Pang 2022. All Rights Reserved.

#pragma once
#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Modules/ModuleManager.h"

DECLARE_LOG_CATEGORY_EXTERN(UMGSplineEditor, Log, All);

class  FUMGSplineEditorModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */


	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
