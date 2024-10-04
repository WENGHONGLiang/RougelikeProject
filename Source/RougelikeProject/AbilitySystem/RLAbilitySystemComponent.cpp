// Fill out your copyright notice in the Description page of Project Settings.


#include "RLAbilitySystemComponent.h"

#include "RLGameplayAbility.h"

void URLAbilitySystemComponent::BindGameplayEffectAppliedDelegate()
{
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &URLAbilitySystemComponent::OnEffectApplied);
}

void URLAbilitySystemComponent::OnEffectApplied(UAbilitySystemComponent* AbilitySystemComponent,
                                                const FGameplayEffectSpec& GameplayEffectSpec, FActiveGameplayEffectHandle ActiveGameplayEffectHandle)
{
	// 得到GE本身的Tag（即AssetTag）
	FGameplayTagContainer TagContainer;
	GameplayEffectSpec.GetAllAssetTags(TagContainer);

	// 将 Tag 广播
	OnGameplayEffectApplied.Broadcast(TagContainer);
}

void URLAbilitySystemComponent::AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupAbilities)
{
	for(auto AbilityClass : StartupAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);

		if(const URLGameplayAbility* Ability = Cast<URLGameplayAbility>(AbilitySpec.Ability))
		{
			AbilitySpec.DynamicAbilityTags.AddTag(Ability->StartupInputTag);
			// 只赋予，不激活
			GiveAbility(AbilitySpec);
		}
	}
}

void URLAbilitySystemComponent::AbilityInputTagHeld(const FGameplayTag& InputTag)
{
	if(!InputTag.IsValid()) return;

	for(auto& AbilitySpec : GetActivatableAbilities())
	{
		if(AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
		{
			if(!AbilitySpec.IsActive())
			{
				// 启用能力
				TryActivateAbility(AbilitySpec.Handle);
			}
		}
	}
}

void URLAbilitySystemComponent::AbilityInputTagReleased(const FGameplayTag& InputTag)
{
	if(!InputTag.IsValid()) return;

	for(auto& AbilitySpec :  GetActivatableAbilities())
	{
		if(AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
		{
			// 能力被松开 // 做一些事情？
		}
	}
}

void URLAbilitySystemComponent::ActivateAbilityDirectly(const FGameplayTag& InputTag)
{
	if(!InputTag.IsValid()) return;
	
	for(auto& AbilitySpec :  GetActivatableAbilities())
	{
		if(AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
		{
			TryActivateAbility(AbilitySpec.Handle);
		}
	}
}

