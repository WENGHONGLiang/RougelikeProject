// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "AbilityConfig.generated.h"

class UGameplayAbility;

USTRUCT(BlueprintType, Blueprintable)
struct FRLAbilityInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag AbilityTag = FGameplayTag();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameplayAbility> AbilityClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText AbilityDescription = FText();
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UTexture2D* AbilityImage = nullptr;

	UPROPERTY(BlueprintReadOnly)
	float AbilityBaseDamage;
};

UCLASS()
class ROUGELIKEPROJECT_API UAbilityConfig : public UDataAsset
{
	GENERATED_BODY()
public:
	FRLAbilityInfo& FindAbilityInfo(const FGameplayTag& AbilityTag, bool bLogNotFound = false);
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FRLAbilityInfo> AbilityInfos;
};
