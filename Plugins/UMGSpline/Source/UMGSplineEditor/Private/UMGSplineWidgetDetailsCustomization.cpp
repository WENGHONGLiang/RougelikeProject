// Copyright Qibo Pang 2022. All Rights Reserved.

#include "UMGSplineWidgetDetailsCustomization.h"
#include "Layout/Margin.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Widgets/SBoxPanel.h"
#include "Framework/Commands/UICommandList.h"
#include "Widgets/Layout/SWrapBox.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Input/SButton.h"
#include "EditorStyleSet.h"
#include "EditorModeManager.h"

#include "PropertyHandle.h"
#include "DetailLayoutBuilder.h"
#include "DetailWidgetRow.h"
#include "IDetailPropertyRow.h"
#include "DetailCategoryBuilder.h"

#include "IDetailsView.h"
#include "IAssetTools.h"
#include "AssetToolsModule.h"

#include "ScopedTransaction.h"
#include "IPropertyUtilities.h"
#include "Subsystems/AssetEditorSubsystem.h"

#include "UMGSplineEditPanel.h"
#include "UMGSplineEditorSettings.h"

#define LOCTEXT_NAMESPACE "SplineWidgetDetails"

//////////////////////////////////////////////////////////////////////////
// FUMGSplineWidgetDetailsCustomization

TSharedRef<IDetailCustomization> FUMGSplineWidgetDetailsCustomization::MakeInstance()
{
	return MakeShareable(new FUMGSplineWidgetDetailsCustomization);
}

void FUMGSplineWidgetDetailsCustomization::CustomizeDetails(IDetailLayoutBuilder& DetailLayout)
{
	const TArray< TWeakObjectPtr<UObject> >& SelectedObjects = DetailLayout.GetSelectedObjects();
	if (SelectedObjects.Num() != 1)
	{
		return;
	}
	
	USplineWidget* SplineWidget = nullptr;
	for (int32 ObjectIndex = 0; ObjectIndex < SelectedObjects.Num(); ++ObjectIndex)
	{
		UObject* TestObject = SelectedObjects[ObjectIndex].Get();
		if (USplineWidget* TestSplineWidget = Cast<USplineWidget>(TestObject))
		{
			SplineWidget = TestSplineWidget;
			break;
		}
	}

	if (!SplineWidget)
	{
		return;
	}

	SplineWidgetPtr = SplineWidget;

	PropertySplineInfo = DetailLayout.GetProperty(GET_MEMBER_NAME_CHECKED(USplineWidget, SplineInfo), USplineWidget::StaticClass());
	check(PropertySplineInfo->IsValidHandle());

	// Make sure the Appearance category is right below the Transform
	IDetailCategoryBuilder& AppearanceCategory = DetailLayout.EditCategory("Appearance", FText::GetEmpty(), ECategoryPriority::TypeSpecific);

	// Make sure the EditSpline category is right below the Appearance category
	IDetailCategoryBuilder& EditSplineCategory = DetailLayout.EditCategory("EditSpline", FText::GetEmpty(), ECategoryPriority::TypeSpecific);

	FText SplineEditText = LOCTEXT("UMGSplineEditPanel", "Spline Edit Panel");
	EditSplineCategory.AddCustomRow(SplineEditText)
		[
			SNew(SVerticalBox)
			/* + SVerticalBox::Slot()
			.AutoHeight()
			[
				SNew(STextBlock)
				.Font(DetailLayout.GetDetailFont())
				.Text(SplineEditText)
			]*/
			+ SVerticalBox::Slot()
			.AutoHeight()
			[
				SNew(SBox)
				.HeightOverride(this, &FUMGSplineWidgetDetailsCustomization::GetSplineEditPanelHieght)
				[
					SNew(SUMGSplineEditPanel)
					.SplineInfo_UObject(SplineWidget, &USplineWidget::GetSplineInfo)
					.Clipping(EWidgetClipping::ClipToBounds)
					.OnSplineInfoValueChanged(FOnSplineInfoValueChanged::CreateSP(this, &FUMGSplineWidgetDetailsCustomization::OnSplineInfoValueChanged))	
				]
			]
		];


	MyDetailLayout = &DetailLayout;
}

void FUMGSplineWidgetDetailsCustomization::OnSplineInfoValueChanged(const FUMGSplineInfo& NewSplineInfo)
{
	FString TextValue;
	FUMGSplineInfo::StaticStruct()->ExportText(TextValue, &NewSplineInfo, &NewSplineInfo, nullptr, EPropertyPortFlags::PPF_None, nullptr);

	FPropertyAccess::Result Result = PropertySplineInfo->SetValueFromFormattedString(TextValue, EPropertyValueSetFlags::NotTransactable);
	check(Result == FPropertyAccess::Success);
}

FOptionalSize FUMGSplineWidgetDetailsCustomization::GetSplineEditPanelHieght() const
{
	return UUMGSplineEditorSettings::GetPanelHeight();
}

//////////////////////////////////////////////////////////////////////////

#undef LOCTEXT_NAMESPACE
