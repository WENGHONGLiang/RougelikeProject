// Fill out your copyright notice in the Description page of Project Settings.


#include "RLGameplayAbility.h"

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

