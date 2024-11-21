// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../RLGameplayAbility.h"
#include "RLEquipmentAbility.generated.h"

/**
 * 
 */
UCLASS()
class ROUGELIKEPROJECT_API URLEquipmentAbility : public URLGameplayAbility
{
	GENERATED_BODY()
protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	
	UFUNCTION(BlueprintCallable)
	void ApplyEquipmentEffect(TSubclassOf<UGameplayEffect> EquipmentEffectClass);
};
