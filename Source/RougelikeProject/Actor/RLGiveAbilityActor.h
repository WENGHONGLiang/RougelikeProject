// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RLInteractableActor.h"
#include "RougelikeProject/GameMode/RLGameMode.h"
#include "RLGiveAbilityActor.generated.h"

/**
 * 
 */
UCLASS()
class ROUGELIKEPROJECT_API ARLGiveAbilityActor : public ARLInteractableActor
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditAnywhere)
	FGameplayTag AbilityTag;
	
	UPROPERTY(EditAnywhere)
	int AbilityLevel = 1;
	
	UPROPERTY()
	ARLGameMode* GM;

	bool bEnemyClear;

	virtual void BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	void BeginPlay() override;
	
	virtual void OnInteract() override;
};
