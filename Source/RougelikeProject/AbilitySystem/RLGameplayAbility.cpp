// Fill out your copyright notice in the Description page of Project Settings.


#include "RLGameplayAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "RLGameplayTags.h"
#include "Kismet/GameplayStatics.h"
#include "RougelikeProject/GameMode/RLGameMode.h"

URLGameplayAbility::URLGameplayAbility()
{
	StartupInputTag = FRLGameplayTags::Get().InputTag_Empty;
}

void URLGameplayAbility::InitAbilityBaseDamage(float Amount)
{
	BaseDamage = Amount;
}

ARLCharacterBase* URLGameplayAbility::GetCharacterBase()
{
	ARLCharacterBase* Ret = Cast<ARLCharacterBase>(GetAvatarActorFromActorInfo());
	if(Ret)
		return Ret;
	return nullptr;
}

void URLGameplayAbility::BeatBackEnemyAroundPlayer(float Distance, float ImpulseAmount, bool bCauseDamage)
{
	ARLCharacter* RLCharacter =  Cast<ARLCharacter>(GetAvatarActorFromActorInfo());
	FVector PlayerLocation = GetAvatarActorFromActorInfo()->GetActorLocation();
	TSet<ARLEnemy*> Enemies = Cast<ARLGameMode>(UGameplayStatics::GetGameMode(this))->GetEnemiesInRange(PlayerLocation, Distance);
	
	FGameplayEffectSpecHandle SpecHandle;
	const float RealDamage = BaseDamage * (1 + 0.1 * (GetAbilityLevel()-1)); // 每级提升 0.1
	if(bCauseDamage)
	{
		const UAbilitySystemComponent* SourceASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(RLCharacter);
		SpecHandle = SourceASC->MakeOutgoingSpec(DamageEffectClass, GetAbilityLevel(), SourceASC->MakeEffectContext());
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, FRLGameplayTags::Get().GE_Damage, RealDamage);
	}
	
	for(auto Enemy : Enemies)
	{
		if(Enemy->GetCharacterAlive())
		{
			Enemy->OnBeatBack();

			if(bCauseDamage)
				Enemy->GetAbilitySystemComponent()->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
		}
	}

	RLCharacter->AddRadialForce(ImpulseAmount, Distance);
}

