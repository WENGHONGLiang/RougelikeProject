// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RLInteractableActor.h"
#include "RLRecoverActor.generated.h"

/**
 * 
 */
UCLASS()
class ROUGELIKEPROJECT_API ARLRecoverActor : public ARLInteractableActor
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere)
	TSubclassOf<UGameplayEffect> GE_Recover;
	
	virtual void BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	
	virtual void OnInteract() override;

	bool bAlreadyUsed = false;
};