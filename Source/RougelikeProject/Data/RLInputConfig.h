// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "InputAction.h"
#include "Engine/DataAsset.h"
#include "RLInputConfig.generated.h"

/* 编辑器中 Data/InputConfig， 记录 GamplayTag 和 IA 的映射 */
USTRUCT(BlueprintType)
struct FRLInputAction
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	const UInputAction* InputAction = nullptr;

	UPROPERTY(EditDefaultsOnly)
	FGameplayTag InputTag = FGameplayTag();
};

UCLASS()
class ROUGELIKEPROJECT_API URLInputConfig : public UDataAsset
{
	GENERATED_BODY()
public:
	// 根据 Tag 查找 IA
	const UInputAction* FindAbilityInputActionFromTag(const FGameplayTag& Tag) const;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FRLInputAction> AbilityInputActions;
};
