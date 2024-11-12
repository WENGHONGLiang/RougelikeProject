// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "RougelikeProject/CharacterSet/RLCharacter.h"
#include "RougelikeProject/Player/RLPlayerController.h"
#include "RLInteractableActor.generated.h"

UCLASS()
class ROUGELIKEPROJECT_API ARLInteractableActor : public AActor
{
	GENERATED_BODY()
	
protected:	
	ARLInteractableActor();
	
	UPROPERTY()
	ARLCharacter* PlayerCharacter;
	
	UPROPERTY()
	ARLPlayerController* PlayerController;
	
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag MessageTipTag_Open;
	
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag MessageTipTag_Close;
	
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly)
	UBoxComponent* BoxComp;

	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* StaticMeshComp;
	
	UFUNCTION()
	virtual void BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
	virtual void EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
	UFUNCTION()
	virtual void OnInteract();

	bool bBindInteractEvent = false;
};
