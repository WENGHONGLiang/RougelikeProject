// Fill out your copyright notice in the Description page of Project Settings.


#include "OverlayWidgetController.h"

#include "RougelikeProject/AbilitySystem/RLAbilitySystemComponent.h"
#include "RougelikeProject/AbilitySystem/RLAbilitySystemLibrary.h"
#include "RougelikeProject/AbilitySystem/RLGameplayTags.h"
#include "RougelikeProject/ArributeBaseSet/AttributeSetBase.h"
#include "RougelikeProject/Player/RLPlayerState.h"

void UOverlayWidgetController::BroadcastInitialValues()
{
	const UAttributeSetBase* WAttributeSet = Cast<UAttributeSetBase>(AttributeSet);
	ARLPlayerState* RLPS = Cast<ARLPlayerState>(PlayerState);

	OnHealthChanged.Broadcast(WAttributeSet->GetHealth());
	OnMaxHealthChanged.Broadcast(WAttributeSet->GetMaxHealth());
	OnMoneyChanged.Broadcast(RLPS->GetMoney());
	OnSkillCoinChanged.Broadcast(RLPS->GetSkillCoin());
	OnPlayerLevelChanged.Broadcast(RLPS->GetPlayerLevel());
	OnExpChanged.Broadcast(RLPS->GetExp(), RLPS->GetUpgradeNeedExp());
}

void UOverlayWidgetController::BindCallbacksToDependencies()
{
	const UAttributeSetBase* WAttributeSet = Cast<UAttributeSetBase>(AttributeSet);

	// 注册AS数值变化回调函数
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		WAttributeSet->GetHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnHealthChanged.Broadcast(Data.NewValue);
			}
		);
	
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		WAttributeSet->GetMaxHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnMaxHealthChanged.Broadcast(Data.NewValue);
			}
		);

	ARLPlayerState* RLPS = Cast<ARLPlayerState>(PlayerState);
	RLPS->OnMoneyChanged.AddLambda([this](int32 NewMoney)
		{
			OnMoneyChanged.Broadcast(NewMoney);
		});
	RLPS->OnSkillCoinChanged.AddLambda([this](int32 NewSkillCoin)
		{
			OnSkillCoinChanged.Broadcast(NewSkillCoin);
		});
	RLPS->OnPlayerLevelChanged.AddLambda([this](int32 NewLevel)
		{
			OnPlayerLevelChanged.Broadcast(NewLevel);
		});
	RLPS->OnExpChanged.AddLambda([this](int32 Exp, int32 UpgradeNeedExp)
		{
			OnExpChanged.Broadcast(Exp, UpgradeNeedExp);
		});
}

void UOverlayWidgetController::SetPropertyMessage(FText PropertyName, EQuality PropertyQuality, FText PropertyDescription, int PropertyLevel, float PropertyBaseDamage, float PropertyCooldown)
{
	OnPropertyMessage.Broadcast(PropertyName, PropertyQuality, PropertyDescription, PropertyLevel, PropertyBaseDamage, PropertyCooldown);
}

void UOverlayWidgetController::SetAbilityMessage(FGameplayTag AbilityTag)
{
	FRLAbilityInfo AbilityInfo = AbilityConfig->FindAbilityInfo(AbilityTag);
	AbilityInfo.AbilityLevel = Cast<URLAbilitySystemComponent>(AbilitySystemComponent)->GetAbilityLevel(AbilityTag);
	SetPropertyMessage(AbilityInfo.AbilityName, AbilityInfo.AbilityQuality, AbilityInfo.AbilityDescription, AbilityInfo.AbilityLevel, AbilityInfo.AbilityBaseDamage, AbilityInfo.AbilityBaseCooldown);
}

void UOverlayWidgetController::SetEquipmentMessage(FGameplayTag EquipmentTag)
{
	FRLEquipmentInfo EquipmentInfo = EquipmentConfig->FindEquipmentInfo(EquipmentTag);
	EquipmentInfo.EquipmentLevel = Cast<URLAbilitySystemComponent>(AbilitySystemComponent)->GetAbilityLevel(EquipmentTag);
	SetPropertyMessage(EquipmentInfo.EquipmentName, EquipmentInfo.EquipmentQuality, EquipmentInfo.EquipmentDescription, EquipmentInfo.EquipmentLevel, 0, EquipmentInfo.EquipmentBaseCooldown);
}

void UOverlayWidgetController::SetTipMessageByTag(FGameplayTag MessageTag)
{
	const FRLMessageInfo info = MessageConfig->FindMessageTag(MessageTag);
	OnTipMessage.Broadcast(info);
}

void UOverlayWidgetController::HideMessage(EMessageHideMode mode)
{
	if(mode == EMessageHideMode::Tip)
		OnHideTipMessage.Broadcast();
	
	else if(mode == EMessageHideMode::Property)
		OnHidePropertyMessage.Broadcast();
	
	else if(mode == EMessageHideMode::All)
	{
		OnHideTipMessage.Broadcast();
		OnHidePropertyMessage.Broadcast();
	}
}
