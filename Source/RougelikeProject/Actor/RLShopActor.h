// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "RLInteractableActor.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "RougelikeProject/UI/HUD/RLHUD.h"
#include "RLShopActor.generated.h"

class ARLCharacter;
class ARLPlayerController;

UCLASS()
class ROUGELIKEPROJECT_API ARLShopActor : public ARLInteractableActor
{
	GENERATED_BODY()
	
public:	
	virtual void BeginPlay() override;
	
protected:
	UPROPERTY()
	ARLHUD* HUD;

	virtual void BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	virtual void EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;

	virtual void OnInteract() override;
	
	bool bShowShopWidget = false;
};
