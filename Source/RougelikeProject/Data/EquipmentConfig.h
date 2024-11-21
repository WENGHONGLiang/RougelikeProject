// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "Engine/DataAsset.h"
#include "RougelikeProject/AbilitySystem/Equipment/RLEquipmentAbility.h"
#include "EquipmentConfig.generated.h"

enum class EQuality : uint8;

USTRUCT(BlueprintType, Blueprintable)
struct FRLEquipmentInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag EquipmentTag = FGameplayTag();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<URLEquipmentAbility> EquipmentAbilityClass; // TODO：继承 GA 创建一个 EquipmentAbility // 激活时应用 GE
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText EquipmentName = FText();
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (MultiLine = true))
	FText EquipmentDescription = FText();
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EQuality EquipmentQuality;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UTexture2D* EquipmentImage = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float EquipmentBaseCooldown;
	
	// 出售获得的金币
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float EquipmentMoneyValue;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float EquipmentSkillCoinValue;
	
	UPROPERTY(BlueprintReadOnly)
	float EquipmentLevel;
};

UCLASS()
class ROUGELIKEPROJECT_API UEquipmentConfig : public UDataAsset
{
	GENERATED_BODY()

public:
	FRLEquipmentInfo& FindEquipmentInfo(const FGameplayTag& EquipmentTag, bool bLogNotFound = false);
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FRLEquipmentInfo> EquipmentInfos;
};
