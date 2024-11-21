// Fill out your copyright notice in the Description page of Project Settings.


#include "RLAbilityMenuWidget.h"

#include "Blueprint/WidgetLayoutLibrary.h"
#include "RougelikeProject/AbilitySystem/RLGameplayTags.h"
#include "RougelikeProject/UI/WidgetController/AbilityMenuWidgetController.h"

void URLAbilityMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();
	BP_SubAbility_LMB->AbilityButton->OnClicked.AddDynamic(this, &URLAbilityMenuWidget::OnClicked_LMB);
	BP_SubAbility_1->AbilityButton->OnClicked.AddDynamic(this, &URLAbilityMenuWidget::OnClicked_1);
	BP_SubAbility_2->AbilityButton->OnClicked.AddDynamic(this, &URLAbilityMenuWidget::OnClicked_2);
	BP_SubAbility_3->AbilityButton->OnClicked.AddDynamic(this, &URLAbilityMenuWidget::OnClicked_3);
	BP_SubAbility_4->AbilityButton->OnClicked.AddDynamic(this, &URLAbilityMenuWidget::OnClicked_4);
}

void URLAbilityMenuWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	if(bDraggingAbility && WidgetController)
	{
		if(!AbilityMenuWidgetController)
			AbilityMenuWidgetController = Cast<UAbilityMenuWidgetController>(WidgetController);
		const FVector2D CursorPosition = AbilityMenuWidgetController->GetCursorPosition();
		float Scale = UWidgetLayoutLibrary::GetViewportScale(this);
		AbilityIcon->SetRenderTranslation(CursorPosition / Scale);
	}
}

void URLAbilityMenuWidget::OnClicked_LMB()
{
	// 鼠标左键固定
	//if(!AbilityMenuWidgetController)
	//	AbilityMenuWidgetController = Cast<UAbilityMenuWidgetController>(WidgetController);
	//
	//AbilityMenuWidgetController->OnAbilityButtonClicked(FRLGameplayTags::Get().InputTag_LMB, BP_SubAbility_LMB->AbilityInfo.AbilityTag);
}

void URLAbilityMenuWidget::OnClicked_1()
{
	if(!AbilityMenuWidgetController)
		AbilityMenuWidgetController = Cast<UAbilityMenuWidgetController>(WidgetController);
	
	AbilityMenuWidgetController->OnAbilityButtonClicked(FRLGameplayTags::Get().InputTag_1, BP_SubAbility_1->AbilityInfo.AbilityTag);
}

void URLAbilityMenuWidget::OnClicked_2()
{
	if(!AbilityMenuWidgetController)
		AbilityMenuWidgetController = Cast<UAbilityMenuWidgetController>(WidgetController);
	
	AbilityMenuWidgetController->OnAbilityButtonClicked(FRLGameplayTags::Get().InputTag_2, BP_SubAbility_2->AbilityInfo.AbilityTag);
}

void URLAbilityMenuWidget::OnClicked_3()
{
	if(!AbilityMenuWidgetController)
		AbilityMenuWidgetController = Cast<UAbilityMenuWidgetController>(WidgetController);
	
	AbilityMenuWidgetController->OnAbilityButtonClicked(FRLGameplayTags::Get().InputTag_3, BP_SubAbility_3->AbilityInfo.AbilityTag);
}

void URLAbilityMenuWidget::OnClicked_4()
{
	// 键盘 4 固定
	//if(!AbilityMenuWidgetController)
	//	AbilityMenuWidgetController = Cast<UAbilityMenuWidgetController>(WidgetController);
	//
	//AbilityMenuWidgetController->OnAbilityButtonClicked(FRLGameplayTags::Get().InputTag_4, BP_SubAbility_4->AbilityInfo.AbilityTag);
}

void URLAbilityMenuWidget::SetDragging(bool bDrag)
{
	bDraggingAbility = bDrag;
}
