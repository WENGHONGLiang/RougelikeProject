// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RLCharacterBase.h"
#include "RLCharacter.generated.h"

/**
 * 
 */
UCLASS()
class ROUGELIKEPROJECT_API ARLCharacter : public ARLCharacterBase
{
	GENERATED_BODY()
	
protected:
	virtual void PossessedBy(AController* NewController) override;
	virtual void InitAbilityActorInfo() override;
	
	virtual void GameplayEffectApplied(const FGameplayTagContainer& TagContainer) override;
};