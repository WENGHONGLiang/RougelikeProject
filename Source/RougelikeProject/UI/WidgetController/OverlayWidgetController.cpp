// Fill out your copyright notice in the Description page of Project Settings.


#include "OverlayWidgetController.h"

#include "RougelikeProject/AbilitySystem/RLGameplayTags.h"
#include "RougelikeProject/ArributeBaseSet/AttributeSetBase.h"

void UOverlayWidgetController::BroadcastInitialValues()
{
	const UAttributeSetBase* WAttributeSet = Cast<UAttributeSetBase>(AttributeSet);

	OnHealthChanged.Broadcast(WAttributeSet->GetHealth());
	OnMaxHealthChanged.Broadcast(WAttributeSet->GetMaxHealth());
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
}

void UOverlayWidgetController::SetMessage(FGameplayTag MessageTag)
{
	FRLMessageInfo info = MessageConfig->FindMessageTag(MessageTag);
	if(MessageTag.MatchesTag(FRLGameplayTags::Get().Message_Tip))
	{
		OnTipMessage.Broadcast(info);
	}
	else if(MessageTag.MatchesTag(FRLGameplayTags::Get().Message_Property))
	{
		OnPropertyMessage.Broadcast(info);
	}
}

void UOverlayWidgetController::HideMessage(MessageHideMode mode)
{
	if(mode == MessageHideMode::Tip)
		OnHideTipMessage.Broadcast();
	
	else if(mode == MessageHideMode::Property)
		OnHidePropertyMessage.Broadcast();
	
	else if(mode == MessageHideMode::All)
	{
		OnHideTipMessage.Broadcast();
		OnHidePropertyMessage.Broadcast();
	}
}
