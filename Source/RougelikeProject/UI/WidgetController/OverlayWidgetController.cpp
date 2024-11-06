// Fill out your copyright notice in the Description page of Project Settings.


#include "OverlayWidgetController.h"

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
}

void UOverlayWidgetController::SetPropertyMessage(FText Message, UTexture2D* Image)
{
	if(Message.IsEmpty())
		return;
	
	FRLMessageInfo info;
	info.Message = Message;
	info.Image = Image;

	OnPropertyMessage.Broadcast(info);
}

void UOverlayWidgetController::SetTipMessageByTag(FGameplayTag MessageTag)
{
	const FRLMessageInfo info = MessageConfig->FindMessageTag(MessageTag);
	OnTipMessage.Broadcast(info);
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
