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

void ARLPlayerState::AddMoney(int32 Amount)
{
	Money += Amount;
	OnMoneyChanged.Broadcast(Money);
}

bool ARLPlayerState::CostMoney(int32 Amount)
{
	if(Money < Amount)
		return false;
	
	Money -= Amount;
	OnMoneyChanged.Broadcast(Money);

	return true;
}
