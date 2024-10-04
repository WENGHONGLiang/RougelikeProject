// Fill out your copyright notice in the Description page of Project Settings.


#include "MessageConfig.h"

FRLMessageInfo UMessageConfig::FindMessageTag(const FGameplayTag& MessageTag, bool bLogNotFound)
{
	for(const FRLMessageInfo& Info : Messages)
	{
		// 精确查找
		if(Info.MessageTag.MatchesTagExact(MessageTag))
		{
			return Info;
		}
	}

	if(bLogNotFound)
	{
		UE_LOG(LogTemp, Error, TEXT("Can't find Info for MessageTag [%s] on MessageTag [%s]"), *MessageTag.ToString(), *GetNameSafe(this));
	}
	
	return FRLMessageInfo();
}
