// Fill out your copyright notice in the Description page of Project Settings.


#include "RLAbilitySystemComponent.h"

#include "RLGameplayAbility.h"
#include "RLGameplayTags.h"
#include "Kismet/GameplayStatics.h"
#include "RougelikeProject/GameMode/RLGameMode.h"
#include "RougelikeProject/Player/RLPlayerState.h"

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

void URLAbilitySystemComponent::AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupAbilities, bool bInit)
{
	for(auto AbilityClass : StartupAbilities)
	{
		AddCharacterAbility(AbilityClass, 1, bInit);
	}
}

void URLAbilitySystemComponent::AddCharacterAbility(const TSubclassOf<UGameplayAbility>& AbilityClass, float AbilityLevel, bool bInit)
{
	FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, AbilityLevel);
	
	if(URLGameplayAbility* Ability = Cast<URLGameplayAbility>(AbilitySpec.Ability))
	{
		ARLGameMode* GameMode = Cast<ARLGameMode>(UGameplayStatics::GetGameMode(this));
		const float BaseDamage = GameMode->GetAbilityBaseDamageWithAbilityTag(Ability->AbilityTags.First());
		Ability->InitAbilityBaseDamage(BaseDamage);
		if(bInit)
		{
			AbilitySpec.DynamicAbilityTags.AddTag(Ability->StartupInputTag);
			
			// 由AbilityMenuWidgetController接收使用
			OnGameplayAbilityGive.Broadcast(Ability->StartupInputTag, Ability->AbilityTags.First(), AbilityLevel);
		}
		
		// 只赋予，不激活
		GiveAbility(AbilitySpec);
	}
}

void URLAbilitySystemComponent::AddAbilityLevel(const FGameplayTag& AbilityTag, bool bUseSkillCoin, int Amount)
{
	if(AbilityTag == FRLGameplayTags::Get().HeroAbility_Empty || AbilityTag == FRLGameplayTags::Get().Equipment_Empty)
		return;

	for(auto& AbilitySpec : GetActivatableAbilities())
	{
		if(AbilitySpec.Ability->AbilityTags.First().MatchesTagExact(AbilityTag))
		{
			ARLPlayerState* RLPS = Cast<ARLPlayerState>(GetOwnerActor());
			ARLCharacter* RLCharacter = Cast<ARLCharacter>(GetAvatarActor());

			// 在技能升级点，使用技能币升级
			if(bUseSkillCoin)
			{
				if(AbilitySpec.Level <= 4 && RLPS->CostSkillCoin(RLCharacter->AbilityLevelCost.GetValueAtLevel(AbilitySpec.Level)))
				{
					AbilitySpec.Level += 1;
				}
			}
			// 合成等方式升级
			else
			{
				AbilitySpec.Level += Amount;
				AbilitySpec.Level = AbilitySpec.Level > 5 ? 5 : AbilitySpec.Level; // 最高 5 级
			}

			
			if(AbilityTag.MatchesTag(FRLGameplayTags::Get().Equipment))
				OnEquipmentLevelUpdate.Broadcast(AbilitySpec.DynamicAbilityTags.First(), AbilitySpec.Ability->AbilityTags.First(), AbilitySpec.Level);
			else
				OnAbilityLevelUpdate.Broadcast(AbilitySpec.DynamicAbilityTags.First(), AbilitySpec.Ability->AbilityTags.First(), AbilitySpec.Level);
			
			break;
		}

	}
}

void URLAbilitySystemComponent::RemoveCharacterAblity(const FGameplayTag& AbilityTag)
{
	if(AbilityTag == FRLGameplayTags::Get().HeroAbility_Empty)
		return;

	for(auto& AbilitySpec : GetActivatableAbilities())
	{
		if(AbilitySpec.Ability->AbilityTags.First().MatchesTagExact(AbilityTag))
		{
			ClearAbility(AbilitySpec.Handle);
			break;
		}
	}
}

void URLAbilitySystemComponent::UpdateCharacterAbilitiesInput(const FGameplayTag& AbilityTag, const FGameplayTag& NewInputTag)
{
	if(AbilityTag == FRLGameplayTags::Get().HeroAbility_Empty || AbilityTag == FRLGameplayTags::Get().Equipment_Empty)
		return;
	if(NewInputTag.MatchesTagExact(FRLGameplayTags::Get().InputTag_Empty))
		return;
	
	for(auto& AbilitySpec : GetActivatableAbilities())
	{
		if(AbilitySpec.Ability->AbilityTags.First().MatchesTagExact(AbilityTag))
		{
			AbilitySpec.DynamicAbilityTags.Reset();
			AbilitySpec.DynamicAbilityTags.AddTag(NewInputTag);

			break;
		}
	}
}

float URLAbilitySystemComponent::GetAbilityLevel(const FGameplayTag AbilityTag)
{
	if(AbilityTag == FRLGameplayTags::Get().HeroAbility_Empty)
		return 0;
	
	for(auto& AbilitySpec : GetActivatableAbilities())
	{
		if(AbilitySpec.Ability->AbilityTags.First().MatchesTagExact(AbilityTag))
		{
			return AbilitySpec.Level;
		}
	}
	return 0;
}

bool URLAbilitySystemComponent::HasAbility(const FGameplayTag AbilityTag)
{
	if(AbilityTag == FRLGameplayTags::Get().HeroAbility_Empty)
		return false;
	
	for(auto& AbilitySpec : GetActivatableAbilities())
	{
		if(AbilitySpec.Ability->AbilityTags.First().MatchesTagExact(AbilityTag))
		{
			return true;
		}
	}
	return false;
}

void URLAbilitySystemComponent::AbilityInputTagHeld(const FGameplayTag& InputTag)
{
	if(!InputTag.IsValid()) return;

	bool bHasAbility = false; // 先检查是否有技能 // 没有技能但是有装备碎片的情况，装备碎片不能被启用
	
	for(auto& AbilitySpec : GetActivatableAbilities())
	{
		if(AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
		{
			if(!AbilitySpec.IsActive() && !AbilitySpec.Ability->AbilityTags.First().MatchesTag(FRLGameplayTags::Get().Equipment))
			{
				bHasAbility = true;
				break;
			}
		}
	}

	if(!bHasAbility) return;
	
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

bool URLAbilitySystemComponent::ActivateAbilityDirectly(const FGameplayTag& InputTag)
{
	if(!InputTag.IsValid()) return false;

	bool bActivate = false;
	for(auto& AbilitySpec :  GetActivatableAbilities())
	{
		if(AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
		{
			if(!AbilitySpec.IsActive())
			{
				TryActivateAbility(AbilitySpec.Handle);
				bActivate = true;
			}
		}
	}
	return bActivate;
}

void URLAbilitySystemComponent::CheckAbilityCoolDown()
{
	for(auto& AbilitySpec : GetActivatableAbilities())
	{
		// 不显示装备的Cooldown
		if(AbilitySpec.Ability->AbilityTags.First().MatchesTag(FRLGameplayTags::Get().Equipment))
			continue;
		
		UGameplayAbility* Ability = AbilitySpec.GetPrimaryInstance();
		if(Ability)
		{
			float CooldownRemain;
			float CooldownDuration;
			FGameplayAbilityActorInfo ActorInfo = Ability->GetActorInfo();
			Ability->GetCooldownTimeRemainingAndDuration(AbilitySpec.Handle, &ActorInfo, CooldownRemain, CooldownDuration);
			
			if(CooldownRemain > 0.001f)
			{
				float Percent = CooldownRemain / CooldownDuration;
				OnCoolDownTimeUpdate.Broadcast(AbilitySpec.DynamicAbilityTags.First(), Percent);
			}
		}
	}
}
