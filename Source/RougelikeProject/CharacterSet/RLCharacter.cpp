// Fill out your copyright notice in the Description page of Project Settings.


#include "RLCharacter.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "RougelikeProject/AbilitySystem/RLAbilitySystemComponent.h"
#include "RougelikeProject/ArributeBaseSet/AttributeSetBase.h"
#include "RougelikeProject/Player/RLPlayerController.h"
#include "RougelikeProject/Player/RLPlayerState.h"
#include "RougelikeProject/UI/HUD/RLHUD.h"

void ARLCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if(RLAbilitySystemComponent)
	{
		RLAbilitySystemComponent->CheckAbilityCoolDown();
	}
}

void ARLCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	
	// 初始化 ASC
	InitAbilityActorInfo();

	PlayerController = Cast<APlayerController>(NewController);
}

void ARLCharacter::InitAbilityActorInfo()
{
	Super::InitAbilityActorInfo();

	ARLPlayerState* RLPlayerState = GetPlayerState<ARLPlayerState>();
	ARLPlayerController* RLPlayerController = Cast<ARLPlayerController>(GetController());

	// 从 PlayerState 中取出 ASC
	AbilitySystemComponent = RLPlayerState->GetAbilitySystemComponent();
	RLAbilitySystemComponent = Cast<URLAbilitySystemComponent>(AbilitySystemComponent);
	AttributeSet = RLPlayerState->GetAttributeSet();
	
	if(!RLPlayerState->HasInit()) // 第一次需要对属性、技能初始化
	{
		AbilitySystemComponent->InitAbilityActorInfo(RLPlayerState, this);
		
		// ASC绑定GE被应用的回调函数
		RLAbilitySystemComponent->BindGameplayEffectAppliedDelegate();

		// 初始属性 
		FCharacterClassDefaultInfo info = CharacterInfo->GetClassDefaultInfo(CharacterType);
		ApplyEffectToSelf(info.Attributes, 1);

		// 初始技能
		AddCharacterAbilities();
		
		RLPlayerState->Init();
	}
	else // 后续只需要绑定新的Pawn
	{
		AbilitySystemComponent->SetAvatarActor(this);
	}
	
	if(ARLHUD* HUD = Cast<ARLHUD>(RLPlayerController->GetHUD()))
	{
		HUD->InitOverlay(RLPlayerController, AbilitySystemComponent, AttributeSet, RLPlayerState);
	}
	
	if(ARLHUD* HUD = Cast<ARLHUD>(RLPlayerController->GetHUD()))
	{
		HUD->InitAbilityMenu(RLPlayerController, AbilitySystemComponent, AttributeSet, RLPlayerState);
	}
	
	RLAbilitySystemComponent->OnGameplayEffectApplied.AddUObject(this, &ARLCharacter::GameplayEffectApplied);
	Cast<UAttributeSetBase>(AttributeSet)->OnCharacterDie.AddDynamic(this, &ARLCharacter::CharacterDie);
	
	// 移速
	const UAttributeSetBase* RLAS = Cast<UAttributeSetBase>(GetAttributeSet());
	GetCharacterMovement()->MaxWalkSpeed = RLAS->GetMoveSpeed();
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(RLAS->GetMoveSpeedAttribute()).AddLambda([this](const FOnAttributeChangeData& Data)
		{
			GetCharacterMovement()->MaxWalkSpeed = Data.NewValue;
		});
}

void ARLCharacter::GameplayEffectApplied(const FGameplayTagContainer& TagContainer)
{
	Super::GameplayEffectApplied(TagContainer);

	// TODO: 角色受到敌人的特殊技能后的效果
}
