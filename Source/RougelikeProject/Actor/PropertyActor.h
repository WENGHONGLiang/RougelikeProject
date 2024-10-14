// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "PropertyActor.generated.h"

/* 装备 和 技能 的基类 */
UCLASS()
class ROUGELIKEPROJECT_API APropertyActor : public AActor
{
	GENERATED_BODY()
	
public:	
	APropertyActor();

protected:
	virtual void BeginPlay() override;
	void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:	
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(VisibleAnywhere)
	USphereComponent* SphereComp; // 碰撞体

	UPROPERTY(VisibleAnywhere)
	UParticleSystemComponent* EffectComp; // 特效
	
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* MeshComp; // 网格

	UPROPERTY(EditAnywhere)
	FGameplayTag MessageTag;
	
	UPROPERTY(EditAnywhere)
	FGameplayTag TipTag;

	UFUNCTION()
	void BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
	void EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
