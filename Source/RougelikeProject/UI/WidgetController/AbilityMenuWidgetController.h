// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RLWidgetController.h"
#include "RougelikeProject/Data/AbilityConfig.h"
#include "AbilityMenuWidgetController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnUpdateAbilitySignature, FGameplayTag, InputTag, FRLAbilityInfo, AbilityInfo);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDragSignature, UTexture2D*, newIcon);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCoolDownSignature, const FGameplayTag, InputTag, float, Percent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FShowAbilityUpgradePanelSignature, bool, bShow);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FAbilityCanUpgradeSignature, const FGameplayTag, InputTag, int, NeedMoney, bool, bCanUpgrade);

UCLASS(Blueprintable)
class ROUGELIKEPROJECT_API UAbilityMenuWidgetController : public URLWidgetController
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere)
	UAbilityConfig* AbilityConfig;

public:
	UAbilityMenuWidgetController();
	
	virtual void BindCallbacksToDependencies() override;

	virtual void BroadcastInitialValues() override;
	
	UPROPERTY(BlueprintAssignable)
	FOnUpdateAbilitySignature OnUpdateAbility;

	UPROPERTY(BlueprintAssignable)
	FOnCoolDownSignature OnUpdateCoolDown;
	
	UPROPERTY(BlueprintAssignable)
	FAbilityCanUpgradeSignature OnUpdateAbilityUpgradeInfo;

	UPROPERTY(BlueprintAssignable)
	FShowAbilityUpgradePanelSignature OnShowAbilityUpgradePanel;
	
	void OnAbilityButtonClicked(const FGameplayTag& InputTag, const FGameplayTag& AbilityTag);

	void DiscardAbility();

	bool AddAbility(const FGameplayTag& AbilityTag, float AbilityLevel);

	void AddAbilityLevel(const FGameplayTag& AbilityTag);

	UFUNCTION()
	void OnMouseClick();

	void ShowAbilityUpgradePanel(bool bShow);

protected:

	bool bDraggingAbility;
	bool bJustPickUp;
	bool bUpgradeMode;
	
	FGameplayTag SourceInputTag;
	FGameplayTag SourceAbilityTag;

	UPROPERTY(BlueprintAssignable)
	FOnDragSignature OnStartDrag;
	
	UPROPERTY(BlueprintAssignable)
	FOnDragSignature OnStopDrag;

	FTimerHandle DelayReleaseDragTimerHandle;
	FTimerHandle PickUpTimerHandle;
};
