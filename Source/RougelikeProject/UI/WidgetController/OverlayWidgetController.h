// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RLWidgetController.h"
#include "RougelikeProject/Data/MessageConfig.h"
#include "OverlayWidgetController.generated.h"

enum MessageHideMode
{
	Tip,
	Property,
	All
};

// 数值改变事件 // 在 Widget 蓝图中注册
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttributeChangedSignature, float, NewValue);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMessageSignature, FRLMessageInfo, MessageInfo);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FEventSignature);

UCLASS(Blueprintable)
class ROUGELIKEPROJECT_API UOverlayWidgetController : public URLWidgetController
{
	GENERATED_BODY()
public:
	// 调用事件初始化 Overlay 界面数值 // Call in RLHUD
	virtual void BroadcastInitialValues() override;

	// 属性绑定 // 注册ASC属性变化事件，并把变化后的数值再次广播出去，由Widget接收
	virtual void BindCallbacksToDependencies() override;

	UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")
	FOnAttributeChangedSignature OnHealthChanged;
	
	UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")
	FOnAttributeChangedSignature OnMaxHealthChanged;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UMessageConfig> MessageConfig;

	// 由设置 Messge 方调用，如 PropertyActor // 通过 Tag 找到具体消息并传递出去
	void SetMessage(FGameplayTag MessageTag);

	void HideMessage(MessageHideMode mode);

	UPROPERTY(BlueprintAssignable, Category="Message")
	FOnMessageSignature OnTipMessage;

	UPROPERTY(BlueprintAssignable, Category="Message")
	FOnMessageSignature OnPropertyMessage;

	UPROPERTY(BlueprintAssignable, Category="Message")
	FEventSignature OnHideTipMessage;
	
	UPROPERTY(BlueprintAssignable, Category="Message")
	FEventSignature OnHidePropertyMessage;
};
