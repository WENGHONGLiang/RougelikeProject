// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "RLAbilitySystemComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FEffectAssetTags, const FGameplayTagContainer&)
DECLARE_MULTICAST_DELEGATE_TwoParams(FCoolDownUpdateDelegate, const FGameplayTag, float)
DECLARE_MULTICAST_DELEGATE_ThreeParams(FAbilityEvent, const FGameplayTag, const FGameplayTag, float)

UCLASS()
class ROUGELIKEPROJECT_API URLAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	FEffectAssetTags OnGameplayEffectApplied;

	FAbilityEvent OnGameplayAbilityGive;
	
	FAbilityEvent OnAbilityLevelUpdate;
	
	FCoolDownUpdateDelegate OnCoolDownTimeUpdate;
	

	// 添加一系列的能力 // 不激活
	void AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupAbilities, bool bInit = true);
	
	void AddCharacterAbility(const TSubclassOf<UGameplayAbility>& AbilityClass, float AbilityLevel = 1, bool bInit = true);

	void AddAbilityLevel(const FGameplayTag& AbilityTag);

	void RemoveCharacterAblity(const FGameplayTag& AbilityTag);
	
	void UpdateCharacterAbilitiesInput(const FGameplayTag& AbilityTag, const FGameplayTag& NewInputTag);

	float GetAbilityLevel(const FGameplayTag AbilityTag);

	bool HasAbility(const FGameplayTag AbilityTag);

	// 通过输入绑定启用技能 // 用于玩家
	void AbilityInputTagReleased(const FGameplayTag& InputTag);
	void AbilityInputTagHeld(const FGameplayTag& InputTag);
	
	// 在 ActorInfo 被设置后调用一次 // 绑定GE被应用的回调
	void BindGameplayEffectAppliedDelegate();

	// 通过传入Tag直接启用技能
	bool ActivateAbilityDirectly(const FGameplayTag& InputTag);

	UFUNCTION()
	void OnEffectApplied(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& GameplayEffectSpec, FActiveGameplayEffectHandle ActiveGameplayEffectHandle);

	UFUNCTION(BlueprintCallable)
	void CheckAbilityCoolDown();
};
