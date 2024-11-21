// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RLCharacterBase.h"
#include "PhysicsEngine/RadialForceComponent.h"
#include "RougelikeProject/AbilitySystem/RLAbilitySystemComponent.h"
#include "RLCharacter.generated.h"

/**
 * 
 */
UCLASS()
class ROUGELIKEPROJECT_API ARLCharacter : public ARLCharacterBase
{
	GENERATED_BODY()
	
protected:
	ARLCharacter();
	
	void Tick(float DeltaSeconds) override;

	void Destroyed() override;
	
	UPROPERTY(VisibleAnywhere)
	URadialForceComponent* RadialForceComp;
	
	virtual void PossessedBy(AController* NewController) override;
	virtual void InitAbilityActorInfo() override;
	
	virtual void GameplayEffectApplied(const FGameplayTagContainer& TagContainer) override;

	UPROPERTY()
	URLAbilitySystemComponent* RLAbilitySystemComponent;

	UPROPERTY()
	APlayerController* PlayerController;

	FDelegateHandle MoveSpeedChangedDelegateHandle;
public:
	APlayerController* GetPlayerController() { return PlayerController; };

	UPROPERTY(EditDefaultsOnly)
	FScalableFloat AbilityLevelCost;

	void AddRadialForce(float Amount, float Radius);
};
