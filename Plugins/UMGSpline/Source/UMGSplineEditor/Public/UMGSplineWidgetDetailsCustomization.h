// Copyright Qibo Pang 2022. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Layout/Visibility.h"
#include "Input/Reply.h"

#include "SplineWidget.h"
#include "IDetailCustomization.h"

class IDetailLayoutBuilder;

//////////////////////////////////////////////////////////////////////////
// FUMGSplineWidgetDetailsCustomization

class FUMGSplineWidgetDetailsCustomization: public IDetailCustomization
{
public:
	// Makes a new instance of this detail layout class for a specific detail view requesting it
	static TSharedRef<IDetailCustomization> MakeInstance();

	// IDetailCustomization interface
	virtual void CustomizeDetails(IDetailLayoutBuilder& DetailLayout) override;
	// End of IDetailCustomization interface

	void OnSplineInfoValueChanged(const FUMGSplineInfo& NewSplineInfo);

	FOptionalSize GetSplineEditPanelHieght() const;

private:

	TWeakObjectPtr<USplineWidget> SplineWidgetPtr;

	IDetailLayoutBuilder* MyDetailLayout;

	TSharedPtr<IPropertyHandle>     PropertySplineInfo;

};
