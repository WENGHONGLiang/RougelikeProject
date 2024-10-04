// Fill out your copyright notice in the Description page of Project Settings.


#include "RLCloseCombat.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "RougelikeProject/AbilitySystem/RLGameplayTags.h"
#include "AbilitySystemComponent.h"

void URLCloseCombat::Attack(AActor* TargetActor)
{
	// TODO: 判定目标距离、是否闪避等，再决定攻击是否生效
	
	const UAbilitySystemComponent* SourceASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetAvatarActorFromActorInfo());
	const FGameplayEffectSpecHandle SpecHandle = SourceASC->MakeOutgoingSpec(DamageEffectClass, GetAbilityLevel(), SourceASC->MakeEffectContext());
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, FRLGameplayTags::Get().GE_Damage, BaseDamage);

	if (UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor))
	{
		TargetASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
	}
}
