// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "RLAbilitySystemComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FEffectAssetTags, const FGameplayTagContainer&)

UCLASS()
class ROUGELIKEPROJECT_API URLAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	FEffectAssetTags OnGameplayEffectApplied;

	// 添加一系列的能力 // 不激活
	void AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupAbilities);

	// 通过输入绑定启用技能 // 用于玩家
	void AbilityInputTagReleased(const FGameplayTag& InputTag);
	void AbilityInputTagHeld(const FGameplayTag& InputTag);
	
	// 在 ActorInfo 被设置后调用一次 // 绑定GE被应用的回调
	void BindGameplayEffectAppliedDelegate();

	// 通过传入Tag直接启用技能 // 用于敌人
	void ActivateAbilityDirectly(const FGameplayTag& InputTag);

	UFUNCTION()
	void OnEffectApplied(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& GameplayEffectSpec, FActiveGameplayEffectHandle ActiveGameplayEffectHandle);
};
