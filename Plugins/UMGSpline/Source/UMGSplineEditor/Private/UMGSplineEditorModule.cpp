// Copyright Qibo Pang 2022. All Rights Reserved.


#include "UMGSplineEditorModule.h"
#include "UMGSplineWidgetDetailsCustomization.h"


#define LOCTEXT_NAMESPACE "FUMGSplineEditorModule"

void FUMGSplineEditorModule::StartupModule()
{
	FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	PropertyModule.RegisterCustomClassLayout(USplineWidget::StaticClass()->GetFName(), FOnGetDetailCustomizationInstance::CreateStatic(&FUMGSplineWidgetDetailsCustomization::MakeInstance));
}

void FUMGSplineEditorModule::ShutdownModule()
{
	if (!UObjectInitialized())
		return;

	FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	PropertyModule.UnregisterCustomClassLayout(USplineWidget::StaticClass()->GetFName());

}


#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FUMGSplineEditorModule, UMGSplineEditor)
