// Fill out your copyright notice in the Description page of Project Settings.


#include "RLCharacter.h"

#include "RougelikeProject/AbilitySystem/RLAbilitySystemComponent.h"
#include "RougelikeProject/Player/RLPlayerController.h"
#include "RougelikeProject/Player/RLPlayerState.h"
#include "RougelikeProject/UI/HUD/RLHUD.h"

void ARLCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	
	// 初始化 ASC
	InitAbilityActorInfo();
}

void ARLCharacter::InitAbilityActorInfo()
{
	Super::InitAbilityActorInfo();
	
	ARLPlayerController* RLPlayerController = Cast<ARLPlayerController>(GetController());

	// Set OverlayWidget/WidgetController in HUD
	if(ARLHUD* HUD = Cast<ARLHUD>(RLPlayerController->GetHUD()))
	{
		HUD->InitOverlay(RLPlayerController, AbilitySystemComponent, AttributeSet);
	}
}

void ARLCharacter::GameplayEffectApplied(const FGameplayTagContainer& TagContainer)
{
	Super::GameplayEffectApplied(TagContainer);

	// TODO: 角色受到敌人的特殊技能后的效果
}
