// Fill out your copyright notice in the Description page of Project Settings.


#include "RLCloseCombat.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "RougelikeProject/AbilitySystem/RLGameplayTags.h"
#include "AbilitySystemComponent.h"

void URLCloseCombat::Attack(AActor* TargetActor, float AttackDistance)
{
	// 判定目标距离、是否闪避等，再决定攻击是否生效
	AActor* AvatarActor = GetAvatarActorFromActorInfo();
	if(FVector::Distance(AvatarActor->GetActorLocation(), TargetActor->GetActorLocation()) > AttackDistance)
		return;
	
	const UAbilitySystemComponent* SourceASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(AvatarActor);
	const FGameplayEffectSpecHandle SpecHandle = SourceASC->MakeOutgoingSpec(DamageEffectClass, GetAbilityLevel(), SourceASC->MakeEffectContext());
	
	if (UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor))
	{
		const bool bRolling = TargetASC->GetOwnedGameplayTags().HasTag(FRLGameplayTags::Get().HeroAbility_Roll);
		if(!bRolling)
		{
			const float RealDamage = BaseDamage * (1 + 0.1 * (GetAbilityLevel()-1)); // 每级提升 0.1
			UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, FRLGameplayTags::Get().GE_Damage, RealDamage);
			TargetASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
		}
	}
}
