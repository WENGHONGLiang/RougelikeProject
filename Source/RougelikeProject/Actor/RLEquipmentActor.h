// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PropertyActor.h"
#include "RougelikeProject/Data/EquipmentConfig.h"
#include "RLEquipmentActor.generated.h"

/**
 * 
 */
UCLASS()
class ROUGELIKEPROJECT_API ARLEquipmentActor : public APropertyActor
{
	GENERATED_BODY()
	ARLEquipmentActor();
protected:
	void OnPickUp(bool bClick) override;

	void OnCrush() override;
	
	FRLEquipmentInfo EquipmentInfo;
public:
	UFUNCTION(BlueprintCallable)
	void SetEquipmentInfo(FRLEquipmentInfo Info) { EquipmentInfo = Info; };
	
	UFUNCTION(BlueprintNativeEvent)
	void InitEquipmentActor(FRLEquipmentInfo Info);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FRLEquipmentInfo GetEquipmentInfo() { return EquipmentInfo; } ;

	void BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
};
