// Fill out your copyright notice in the Description page of Project Settings.


#include "RLPlayerState.h"

#include "RougelikeProject/AbilitySystem/RLAbilitySystemComponent.h"
#include "RougelikeProject/ArributeBaseSet/AttributeSetBase.h"

ARLPlayerState::ARLPlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<URLAbilitySystemComponent>("AbilitySystemComp");
	AttributeSet = CreateDefaultSubobject<UAttributeSetBase>("AttributeSet");
	Money = 8;
	SkillCoin = 5;
	Exp = 0;
	PlayerLevel = 1;
	PlayerUpgradeNeededBaseExp = 150;
	PlayerUpgradeNeededExpMag = 1.5f;
}

UAbilitySystemComponent* ARLPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void ARLPlayerState::AddLevel()
{
	PlayerLevel += 1;
	
	// 属性变化
	FGameplayEffectContextHandle ContextHandle = GetAbilitySystemComponent()->MakeEffectContext();
	ContextHandle.AddSourceObject(this);
	const FGameplayEffectSpecHandle SpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(GE_PlayerUpgradeAttributeClass, 1, ContextHandle);
	GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), GetAbilitySystemComponent());

	OnPlayerLevelChanged.Broadcast(PlayerLevel);
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

void ARLPlayerState::AddSkillCoin(int32 Amount)
{
	SkillCoin += Amount;
	OnSkillCoinChanged.Broadcast(SkillCoin);
}

bool ARLPlayerState::CostSkillCoin(int32 Amount)
{
	if(SkillCoin < Amount)
		return false;

	SkillCoin -= Amount;
	OnSkillCoinChanged.Broadcast(SkillCoin);

	return true;
}

void ARLPlayerState::AddExp(int32 Amount)
{
	Exp += Amount;
	
	// 检查升级
	int32 UpgradeNeededLevel = GetUpgradeNeedExp();
	if(Exp >= UpgradeNeededLevel)
	{
		Exp -= UpgradeNeededLevel;
		AddLevel();
	}

	UpgradeNeededLevel = GetUpgradeNeedExp();
	OnExpChanged.Broadcast(Exp, UpgradeNeededLevel);
}

int32 ARLPlayerState::GetUpgradeNeedExp()
{
	return FMath::Pow(PlayerUpgradeNeededExpMag, static_cast<float>(GetPlayerLevel() - 1)) * PlayerUpgradeNeededBaseExp;
}
