// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "AbilityConfig.generated.h"

class UGameplayAbility;

UENUM(Blueprintable, BlueprintType)
enum class EQuality : uint8
{
	White,
	Blue,
	Purple,
	Red
};

USTRUCT(BlueprintType, Blueprintable)
struct FRLAbilityInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag AbilityTag = FGameplayTag();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameplayAbility> AbilityClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText AbilityName = FText();
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (MultiLine = true))
	FText AbilityDescription = FText();
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EQuality AbilityQuality;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UTexture2D* AbilityImage = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float AbilityBaseDamage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float AbilityBaseCooldown;

	// 出售技能获得的金币
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float AbilityMoneyValue;

	// 粉碎技能获得的技能币
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float AbilitySkillCoinValue;
	
	UPROPERTY(BlueprintReadOnly)
	float AbilityLevel;
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
