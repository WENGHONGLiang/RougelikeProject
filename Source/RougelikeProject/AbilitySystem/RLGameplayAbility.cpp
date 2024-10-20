// Fill out your copyright notice in the Description page of Project Settings.


#include "RLGameplayAbility.h"

URLGameplayAbility::URLGameplayAbility()
{
	BaseDamage = 1.f;
}

ARLCharacterBase* URLGameplayAbility::GetCharacterBase()
{
	ARLCharacterBase* Ret = Cast<ARLCharacterBase>(GetAvatarActorFromActorInfo());
	if(Ret)
		return Ret;
	return nullptr;
}

