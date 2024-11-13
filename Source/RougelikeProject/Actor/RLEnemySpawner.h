// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "RougelikeProject/CharacterSet/RLEnemy.h"
#include "RougelikeProject/Data/CharacterInfo.h"
#include "RLEnemySpawner.generated.h"

USTRUCT(Blueprintable, BlueprintType)
struct FEnemyInfo
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere)
	ECharacterType EnemyType;
	UPROPERTY(EditAnywhere)
	int32 EnemyNumber;
};

UCLASS()
class ROUGELIKEPROJECT_API ARLEnemySpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	ARLEnemySpawner();

protected:
	virtual void BeginPlay() override;
	
	UFUNCTION()
	virtual void BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(EditAnywhere)
	UBoxComponent* BoxComp;
	
	UPROPERTY(EditAnywhere)
	TArray<FEnemyInfo> EnemyConfig;

	UPROPERTY(EditAnywhere)
	int32 SpawnTurn;

	UPROPERTY()
	TArray<ARLEnemy*> CurEnemies;

	FTimerHandle SpawnTimerHandle;
	
	int32 CurTurn;
	bool bStartSpawn;
	
	void SpawnEnemy();
	
};
