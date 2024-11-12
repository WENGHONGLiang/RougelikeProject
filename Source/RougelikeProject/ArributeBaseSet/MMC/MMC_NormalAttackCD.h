// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "MMC_NormalAttackCD.generated.h"

/**
 * 
 */
UCLASS()
class ROUGELIKEPROJECT_API UMMC_NormalAttackCD : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()
public:
	UMMC_NormalAttackCD();
	virtual  float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;

private:
	FGameplayEffectAttributeCaptureDefinition AttackSpeedDef;
};
