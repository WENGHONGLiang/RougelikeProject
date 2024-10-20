// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "RLUserWidget.h"
#include "Components/Button.h"
#include "RougelikeProject/Data/AbilityConfig.h"
#include "RLSubAbilityWidget.generated.h"

UCLASS()
class ROUGELIKEPROJECT_API URLSubAbilityWidget : public URLUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta = (BindWidget), BlueprintReadOnly)
	UButton* AbilityButton;

	UPROPERTY(BlueprintReadOnly)
	FRLAbilityInfo AbilityInfo;
	
	UPROPERTY(BlueprintReadOnly)
	bool HasAbility = false;

	// 蓝图中收到更新事件时，存储更新的技能Info
	UFUNCTION(BlueprintCallable)
	void StoreAbilityInfo(FRLAbilityInfo info);
};
