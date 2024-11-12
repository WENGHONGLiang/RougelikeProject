// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "MMC_SkillCD.generated.h"

/**
 * 
 */
UCLASS()
class ROUGELIKEPROJECT_API UMMC_SkillCD : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()
	
public:
	UMMC_SkillCD();
	virtual  float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;

private:
	FGameplayEffectAttributeCaptureDefinition SkillSpeedDef;
};
