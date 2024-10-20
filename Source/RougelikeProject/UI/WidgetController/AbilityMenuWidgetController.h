// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RLWidgetController.h"
#include "RougelikeProject/Data/AbilityConfig.h"
#include "AbilityMenuWidgetController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnUpdateAbilitySignature, FGameplayTag, InputTag, FRLAbilityInfo, AbilityInfo);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDragSignature, UTexture2D*, newIcon);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCoolDownSignature, const FGameplayTag, InputTag, float, Percent);

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
	
	void OnAbilityButtonClicked(const FGameplayTag& InputTag, const FGameplayTag& AbilityTag);

	void CancelDrag();

	UFUNCTION()
	void OnMouseClick();

protected:

	bool bDraggingAbility;
	
	FGameplayTag SourceInputTag;
	FGameplayTag SourceAbilityTag;

	UPROPERTY(BlueprintAssignable)
	FOnDragSignature OnStartDrag;
	
	UPROPERTY(BlueprintAssignable)
	FOnDragSignature OnStopDrag;

	FTimerHandle DelayReleaseDragTimerHandle;
};
