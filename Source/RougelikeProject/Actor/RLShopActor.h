// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "RougelikeProject/UI/HUD/RLHUD.h"
#include "RLShopActor.generated.h"

class ARLCharacter;
class ARLPlayerController;

UCLASS()
class ROUGELIKEPROJECT_API ARLShopActor : public AActor
{
	GENERATED_BODY()
	
public:	
	ARLShopActor();

protected:
	UPROPERTY()
	ARLHUD* HUD;
	
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
	void BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
	void EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
	UFUNCTION()
	void OnInteract();

	bool bShowShopWidget = false;
	bool bBindInteractEvent = false;
};
