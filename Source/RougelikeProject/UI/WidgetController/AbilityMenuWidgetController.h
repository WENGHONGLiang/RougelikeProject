// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RLWidgetController.h"
#include "RougelikeProject/Data/AbilityConfig.h"
#include "RougelikeProject/Data/EquipmentConfig.h"
#include "AbilityMenuWidgetController.generated.h"

UENUM()
enum EAddPropertyRes
{
	NewlyAdded,
	AlreadyHasProperty,
	DontAdd
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDragSignature, UTexture2D*, newIcon);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAbilityCoolDownSignature, const FGameplayTag, InputTag, float, Percent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FShowUpgradePanelSignature, bool, bShow);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnUpdateAbilitySignature, FGameplayTag, InputTag, FRLAbilityInfo, AbilityInfo);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FAbilityCanUpgradeSignature, const FGameplayTag, InputTag, int, NeedMoney, bool, bCanUpgrade);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnUpdateEquipmentByInputSlotSignature, FGameplayTag, InputTag, int, Index, FRLEquipmentInfo, EquipmentInfo);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnUpdateEquipmentByEquipmentTagSignature, FGameplayTag, EquipmentTag, FRLEquipmentInfo, EquipmentInfo);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FEquipmentCanUpgradeSignature, const FGameplayTag, EquipmentTag, int, NeedMoney, bool, bCanUpgrade);

UCLASS(Blueprintable)
class ROUGELIKEPROJECT_API UAbilityMenuWidgetController : public URLWidgetController
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere)
	UAbilityConfig* AbilityConfig;

	UPROPERTY(EditAnywhere)
	UEquipmentConfig* EquipmentConfig;

public:
	UAbilityMenuWidgetController();
	
	virtual void BindCallbacksToDependencies() override;

	virtual void BroadcastInitialValues() override;

	UPROPERTY(BlueprintAssignable)
	FOnAbilityCoolDownSignature OnUpdateAbilityCoolDown;
	
	UPROPERTY(BlueprintAssignable)
	FOnUpdateAbilitySignature OnUpdateAbility;
	
	UPROPERTY(BlueprintAssignable)
	FAbilityCanUpgradeSignature OnUpdateAbilityUpgradeInfo;
	
	UPROPERTY(BlueprintAssignable)
	FOnUpdateEquipmentByInputSlotSignature OnUpdateEquipmentByInputSlot;
	
	UPROPERTY(BlueprintAssignable)
	FOnUpdateEquipmentByEquipmentTagSignature OnUpdateEquipmentByEquipmentTag;
	
	UPROPERTY(BlueprintAssignable)
	FEquipmentCanUpgradeSignature OnUpdateEquipmentUpgradeInfo;

	UPROPERTY(BlueprintAssignable)
	FShowUpgradePanelSignature OnShowUpgradePanel;
	
	void OnAbilityButtonClicked(const FGameplayTag& InputTag, const FGameplayTag& AbilityTag);

	// 在 SubAbilityWidget 中调用
	void OnEquipmemtButtonClicked(const FGameplayTag& InputTag, int Index, const FGameplayTag& EquipmentTag);

	void DiscardProperty();

	EAddPropertyRes AddProperty(const FGameplayTag& PropertyTag, float PropertyLevel);

	void AddPropertyLevelWithSkillCoin(FGameplayTag PropertyTag);

	UFUNCTION()
	void OnMouseClick();

	void ShowAbilityUpgradePanel(bool bShow);

protected:

	bool bDraggingAbility;
	bool bDraggingEquipment;
	
	bool bJustPickUp;
	bool bUpgradeMode;
	
	FGameplayTag SourceInputTag;
	int SourcePropertyIndex; // 装备使用
	FGameplayTag SourceAbilityTag;
	FGameplayTag SourceEquipmentTag;

	UPROPERTY(BlueprintAssignable)
	FOnDragSignature OnStartDrag;
	
	UPROPERTY(BlueprintAssignable)
	FOnDragSignature OnStopDrag;

	FTimerHandle DelayReleaseDragTimerHandle;
	FTimerHandle PickUpTimerHandle;
};
