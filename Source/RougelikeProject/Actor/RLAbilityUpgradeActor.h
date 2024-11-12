// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RLInteractableActor.h"
#include "RLAbilityUpgradeActor.generated.h"

/**
 * 
 */
UCLASS()
class ROUGELIKEPROJECT_API ARLAbilityUpgradeActor : public ARLInteractableActor
{
	GENERATED_BODY()

public:
	virtual void OnInteract() override;

	virtual void EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;

	bool bShowAbilityUpgradePanel = false;
};
