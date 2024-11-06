// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PropertyActor.h"
#include "Components/WidgetComponent.h"
#include "RougelikeProject/Data/AbilityConfig.h"
#include "RLAbilityActor.generated.h"

/**
 * 
 */
UCLASS()
class ROUGELIKEPROJECT_API ARLAbilityActor : public APropertyActor
{
	GENERATED_BODY()
	ARLAbilityActor();
	
protected:
	void OnPickUp() override;

	FRLAbilityInfo AbilityInfo;
public:
	UFUNCTION(BlueprintCallable)
	void SetAbilityInfo(FRLAbilityInfo Info) { AbilityInfo = Info; };
	
	UFUNCTION(BlueprintImplementableEvent)
	void InitAbilityActor(FRLAbilityInfo Info);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FRLAbilityInfo GetAbilityInfo() { return AbilityInfo; } ;

	void BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	void EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;
};
