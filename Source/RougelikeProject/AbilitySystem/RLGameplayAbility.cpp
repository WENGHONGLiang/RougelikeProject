// Fill out your copyright notice in the Description page of Project Settings.


#include "RLGameplayAbility.h"

#include "RLGameplayTags.h"

URLGameplayAbility::URLGameplayAbility()
{
	BaseDamage = 1.f;
	StartupInputTag = FRLGameplayTags::Get().InputTag_Empty;
}

ARLCharacterBase* URLGameplayAbility::GetCharacterBase()
{
	ARLCharacterBase* Ret = Cast<ARLCharacterBase>(GetAvatarActorFromActorInfo());
	if(Ret)
		return Ret;
	return nullptr;
}

