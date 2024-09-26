// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSetBase.h"
#include "HealthSet.generated.h"

/**
 * 生命属性集
 */
UCLASS()
class UHealthSet : public UAttributeSetBase {
	GENERATED_BODY()

public:
	//最大生命值
	UPROPERTY(BlueprintReadOnly)
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UHealthSet, MaxHealth);
	//生命恢复速度
	UPROPERTY(BlueprintReadOnly)
	FGameplayAttributeData HealthRegenRate;
	ATTRIBUTE_ACCESSORS(UHealthSet, HealthRegenRate);
};
