// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "ExecCal_NormalAttackDamage.generated.h"


// 普通攻击的数值计算
UCLASS()
class ROUGELIKEPROJECT_API UExecCal_NormalAttackDamage : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()
public:
	UExecCal_NormalAttackDamage();

	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
};
