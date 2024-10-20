// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityConfig.h"

FRLAbilityInfo& UAbilityConfig::FindAbilityInfo(const FGameplayTag& AbilityTag, bool bLogNotFound)
{
	for(FRLAbilityInfo& Info : AbilityInfos)
	{
		// 精确查找
		if(Info.AbilityTag.MatchesTagExact(AbilityTag))
		{
			return Info;
		}
	}

	if(bLogNotFound)
	{
		UE_LOG(LogTemp, Error, TEXT("Can't find Info for AbilityTag [%s] on AbilityTag [%s]"), *AbilityTag.ToString(), *GetNameSafe(this));
	}
	
	return AbilityInfos.Top(); // 默认第一个为空技能
}
