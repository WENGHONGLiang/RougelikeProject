// Fill out your copyright notice in the Description page of Project Settings.


#include "RLPlayerState.h"

#include "RougelikeProject/AbilitySystem/RLAbilitySystemComponent.h"
#include "RougelikeProject/ArributeBaseSet/AttributeSetBase.h"

ARLPlayerState::ARLPlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<URLAbilitySystemComponent>("AbilitySystemComp");
	AttributeSet = CreateDefaultSubobject<UAttributeSetBase>("AttributeSet");
}

UAbilitySystemComponent* ARLPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}
