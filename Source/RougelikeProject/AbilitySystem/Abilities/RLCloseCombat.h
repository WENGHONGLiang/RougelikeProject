// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "../RLGameplayAbility.h"
#include "RLCloseCombat.generated.h"

// 近战 Ability
UCLASS()
class ROUGELIKEPROJECT_API URLCloseCombat : public URLGameplayAbility
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void Attack(AActor* TargetActor, float AttackDistance);
};
