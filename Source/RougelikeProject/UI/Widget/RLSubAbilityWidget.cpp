// Fill out your copyright notice in the Description page of Project Settings.


#include "RLSubAbilityWidget.h"

#include "RougelikeProject/AbilitySystem/RLAbilitySystemLibrary.h"
#include "RougelikeProject/AbilitySystem/RLGameplayTags.h"
#include "RougelikeProject/UI/WidgetController/AbilityMenuWidgetController.h"

URLSubAbilityWidget::URLSubAbilityWidget()
{
	EquipmentInfos.SetNum(3);
	EquipmentInfos[0].EquipmentTag = FRLGameplayTags::Get().Equipment_Empty;
	EquipmentInfos[1].EquipmentTag = FRLGameplayTags::Get().Equipment_Empty;
	EquipmentInfos[2].EquipmentTag = FRLGameplayTags::Get().Equipment_Empty;
	HasEquipments.SetNum(3);
	HasEquipments[0] = false;
	HasEquipments[1] = false;
	HasEquipments[2] = false;
	AbilityInfo.AbilityTag = FRLGameplayTags::Get().HeroAbility_Empty;
}

void URLSubAbilityWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	EquipmentButton_1->OnClicked.AddDynamic(this, &URLSubAbilityWidget::OnEquipmentButtonClick_1);
	EquipmentButton_2->OnClicked.AddDynamic(this, &URLSubAbilityWidget::OnEquipmentButtonClick_2);
	EquipmentButton_3->OnClicked.AddDynamic(this, &URLSubAbilityWidget::OnEquipmentButtonClick_3);
}

void URLSubAbilityWidget::StoreAbilityInfo(FRLAbilityInfo info)
{
	AbilityInfo = info;

	if(info.AbilityTag.MatchesTagExact(FRLGameplayTags::Get().HeroAbility_Empty))
		HasAbility = false;
	else
		HasAbility = true;
}

void URLSubAbilityWidget::StoreEquipmentInfo(FRLEquipmentInfo info, int Index)
{
	EquipmentInfos[Index] = info;
	if(info.EquipmentTag.MatchesTagExact(FRLGameplayTags::Get().Equipment_Empty))
		HasEquipments[Index] = false;
	else
		HasEquipments[Index] = true;
}

void URLSubAbilityWidget::AddAblityLevel()
{
	UAbilityMenuWidgetController* AbilityMenuWidgetController = Cast<UAbilityMenuWidgetController>(WidgetController);
	AbilityMenuWidgetController->AddPropertyLevelWithSkillCoin(AbilityInfo.AbilityTag);
}

void URLSubAbilityWidget::AddEquipmentLevel(int Index)
{
	UAbilityMenuWidgetController* AbilityMenuWidgetController = Cast<UAbilityMenuWidgetController>(WidgetController);
	AbilityMenuWidgetController->AddPropertyLevelWithSkillCoin(EquipmentInfos[Index].EquipmentTag);
}

void URLSubAbilityWidget::OnEquipmentButtonClick_1()
{
	URLAbilitySystemLibrary::GetAbilityMenuWidgetController(this)->OnEquipmemtButtonClicked(InputTag, 0, EquipmentInfos[0].EquipmentTag);
}

void URLSubAbilityWidget::OnEquipmentButtonClick_2()
{
	URLAbilitySystemLibrary::GetAbilityMenuWidgetController(this)->OnEquipmemtButtonClicked(InputTag, 1, EquipmentInfos[1].EquipmentTag);
}

void URLSubAbilityWidget::OnEquipmentButtonClick_3()
{
	URLAbilitySystemLibrary::GetAbilityMenuWidgetController(this)->OnEquipmemtButtonClicked(InputTag, 2, EquipmentInfos[2].EquipmentTag);
}

