// Fill out your copyright notice in the Description page of Project Settings.


#include "RLInputConfig.h"

#include "InputAction.h"

const UInputAction* URLInputConfig::FindAbilityInputActionFromTag(const FGameplayTag& Tag) const
{
	for(const FRLInputAction AbilityAction : AbilityInputActions)
	{
		if(AbilityAction.InputAction && AbilityAction.InputTag.MatchesTagExact(Tag))
		{
			return AbilityAction.InputAction;
		}
	}
	return nullptr;
}
