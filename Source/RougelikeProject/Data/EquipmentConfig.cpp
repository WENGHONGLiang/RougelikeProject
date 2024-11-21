// Fill out your copyright notice in the Description page of Project Settings.


#include "EquipmentConfig.h"

FRLEquipmentInfo& UEquipmentConfig::FindEquipmentInfo(const FGameplayTag& EquipmentTag, bool bLogNotFound)
{
	for(FRLEquipmentInfo& Info : EquipmentInfos)
	{
		// 精确查找
		if(Info.EquipmentTag.MatchesTagExact(EquipmentTag))
		{
			return Info;
		}
	}

	if(bLogNotFound)
	{
		UE_LOG(LogTemp, Error, TEXT("Can't find Info for EquipmentTag [%s] on EquipmentTag [%s]"), *EquipmentTag.ToString(), *GetNameSafe(this));
	}
	
	return EquipmentInfos.Top(); // 默认第一个为空
}
