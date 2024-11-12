// Fill out your copyright notice in the Description page of Project Settings.


#include "RLSubAbilityWidget.h"

#include "RougelikeProject/AbilitySystem/RLGameplayTags.h"
#include "RougelikeProject/UI/WidgetController/AbilityMenuWidgetController.h"

URLSubAbilityWidget::URLSubAbilityWidget()
{
	AbilityInfo.AbilityTag = FRLGameplayTags::Get().HeroAbility_Empty;
}

void URLSubAbilityWidget::StoreAbilityInfo(FRLAbilityInfo info)
{
	AbilityInfo = info;

	if(info.AbilityTag.MatchesTagExact(FRLGameplayTags::Get().HeroAbility_Empty))
		HasAbility = false;
	else
		HasAbility = true;
}

void URLSubAbilityWidget::AddAblityLevel()
{
	UAbilityMenuWidgetController* AbilityMenuWidgetController = Cast<UAbilityMenuWidgetController>(WidgetController);
	AbilityMenuWidgetController->AddAbilityLevel(AbilityInfo.AbilityTag);
}

