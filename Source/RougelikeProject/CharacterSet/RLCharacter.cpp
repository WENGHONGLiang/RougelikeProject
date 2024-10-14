// Fill out your copyright notice in the Description page of Project Settings.


#include "RLCharacter.h"

#include "RougelikeProject/AbilitySystem/RLAbilitySystemComponent.h"
#include "RougelikeProject/ArributeBaseSet/AttributeSetBase.h"
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

	ARLPlayerState* RLPlayerState = GetPlayerState<ARLPlayerState>();
	ARLPlayerController* RLPlayerController = Cast<ARLPlayerController>(GetController());

	// 从 PlayerState 中取出 ASC
	AbilitySystemComponent = RLPlayerState->GetAbilitySystemComponent();
	AttributeSet = RLPlayerState->GetAttributeSet();
	CharacterLevel = RLPlayerState->GetPlayerLevel();
	URLAbilitySystemComponent* RLASC = Cast<URLAbilitySystemComponent>(AbilitySystemComponent);
	
	if(!RLPlayerState->HasInit()) // 第一次需要对属性、技能初始化
	{
		AbilitySystemComponent->InitAbilityActorInfo(RLPlayerState, this);
		
		// ASC绑定GE被应用的回调函数
		RLASC->BindGameplayEffectAppliedDelegate();

		// 初始属性 
		FCharacterClassDefaultInfo info = CharacterInfo->GetClassDefaultInfo(CharacterType);
		ApplyEffectToSelf(info.Attributes, GetCharacterLevel());

		// 初始技能
		AddCharacterAbilities();
		RLPlayerState->Init();
	}
	else // 后续只需要绑定新的Pawn
	{
		AbilitySystemComponent->SetAvatarActor(this);
	}
	
	RLASC->OnGameplayEffectApplied.AddUObject(this, &ARLCharacter::GameplayEffectApplied);
	Cast<UAttributeSetBase>(AttributeSet)->OnCharacterDie.AddDynamic(this, &ARLCharacter::CharacterDie);
	
	// Set OverlayWidget/WidgetController in HUD
	if(ARLHUD* HUD = Cast<ARLHUD>(RLPlayerController->GetHUD()))
	{
		HUD->InitOverlay(RLPlayerController, AbilitySystemComponent, AttributeSet, RLPlayerState);
	}
}

void ARLCharacter::GameplayEffectApplied(const FGameplayTagContainer& TagContainer)
{
	Super::GameplayEffectApplied(TagContainer);

	// TODO: 角色受到敌人的特殊技能后的效果
}
