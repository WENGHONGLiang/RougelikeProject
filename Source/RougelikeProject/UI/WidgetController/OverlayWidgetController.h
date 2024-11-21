// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RLWidgetController.h"
#include "RougelikeProject/Data/MessageConfig.h"
#include "OverlayWidgetController.generated.h"

class UEquipmentConfig;
class UAbilityConfig;
enum class EQuality : uint8;

UENUM(BlueprintType, Blueprintable)
enum class EMessageHideMode : uint8
{
	Tip,
	Property,
	All
};

// 数值改变事件 // 在 Widget 蓝图中注册
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttributeChangedSignature, float, NewValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnExpAttributeChangedSignature, int32, CurExp, int32, UpgradeNeedExp);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTipMessageSignature, FRLMessageInfo, MessageInfo);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_SixParams(FOnPropertyMessageSignature, FText, PropertyName, EQuality, PropertyQuality, FText, PropertyDescription, int, PropertyLevel, float, PropertyBaseDamage, float, PropertyCooldown);

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
	
	UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")
	FOnAttributeChangedSignature OnMoneyChanged;
	
	UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")
	FOnAttributeChangedSignature OnSkillCoinChanged;
	
	UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")
	FOnAttributeChangedSignature OnPlayerLevelChanged;
	
	UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")
	FOnExpAttributeChangedSignature OnExpChanged;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UMessageConfig> MessageConfig;
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UAbilityConfig> AbilityConfig;
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UEquipmentConfig> EquipmentConfig;

	void SetPropertyMessage(FText PropertyName, EQuality PropertyQuality, FText PropertyDescription, int PropertyLevel, float PropertyBaseDamage, float PropertyCooldown);

	UFUNCTION(BlueprintCallable)
	void SetAbilityMessage(FGameplayTag AbilityTag);
	
	UFUNCTION(BlueprintCallable)
	void SetEquipmentMessage(FGameplayTag EquipmentTag);
	
	// 由设置 Messge 方调用，如 PropertyActor // 通过 Tag 找到具体消息并传递出去
	void SetTipMessageByTag(FGameplayTag MessageTag);

	UFUNCTION(BlueprintCallable)
	void HideMessage(EMessageHideMode mode);

	UPROPERTY(BlueprintAssignable, Category="Message")
	FOnTipMessageSignature OnTipMessage;

	UPROPERTY(BlueprintAssignable, Category="Message")
	FOnPropertyMessageSignature OnPropertyMessage;

	UPROPERTY(BlueprintAssignable, Category="Message")
	FEventSignature OnHideTipMessage;
	
	UPROPERTY(BlueprintAssignable, Category="Message")
	FEventSignature OnHidePropertyMessage;
};
