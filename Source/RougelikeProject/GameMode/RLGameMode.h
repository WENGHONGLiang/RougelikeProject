// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "RougelikeProject/Actor/PropertyActor.h"
#include "RougelikeProject/CharacterSet/RLEnemy.h"
#include "RougelikeProject/UI/HUD/RLHUD.h"
#include "RougelikeProject/UI/Widget/LevelMapWidget.h"
#include "RougelikeProject/UI/WidgetController/LevelMapWidgetController.h"
#include "RLGameMode.generated.h"

DECLARE_MULTICAST_DELEGATE(FEventDelegate)

UCLASS()
class ROUGELIKEPROJECT_API ARLGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	void BeginPlay() override;
	
	/* --- Level --- */
	UFUNCTION(BlueprintCallable)
	void StartLevel(int NodeIndex, FName LevelName);

	UFUNCTION(BlueprintCallable)
	void EndLevel();
	
	FEventDelegate OnEnemyClear;

	/* --- 技能 --- */
	UFUNCTION(BlueprintCallable) // TEST
	void SpawnAblityActorAroundPlayer(FGameplayTag AbilityTag, float AbilityLevel = 1);
	
	void SpawnAbilityActorAtLocation(FGameplayTag AbilityTag, FVector3d Location, float AbilityLevel);

	float GetAbilityBaseDamageWithAbilityTag(FGameplayTag AbilityTag);

	/* --- 装备 --- */
	UFUNCTION(BlueprintCallable) // TEST
	void SpawnEquipmentActorAroundPlayer(FGameplayTag EquipmentTag, float EquipmentLevel = 1);
	
	void SpawnEquipmentActorAtLocation(FGameplayTag EquipmentTag, FVector3d Location, float EquipmentLevel);
	

	/* --- 敌人生成 --- */
	ARLEnemy* SpawnEnemyAtLocation(ECharacterType EnemyType, FVector Location);

	void CheckEnemyAliveState();

	TSet<ARLEnemy*> GetEnemiesInRange(const FVector TargetPosition, float Distance);
	
	void AddEnemySpawnerNumber() { EnemySpawnerNumber++; }; // 初始化时添加，敌人死光时减少。减为0时代表本关结束，可开启地图Actor。

	void ReduceEnmeySpawnerNumber();
	
private:
	UPROPERTY()
	ARLHUD* RLHUD;
	
	/* --- Level --- */
	int CurrentLevelNodeIndex;
	int LastLevelNodeIndex;
	FName CurrentLevelName;
	
	void LoadLevel(FName LevelName);
	void UnLoadCurrentLevel();
	
	/* --- 技能 --- */
	UPROPERTY(EditAnywhere)
	UAbilityConfig* AbilityConfig;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<APropertyActor> AbilityActorClass;
	
	/* --- 装备 --- */
	UPROPERTY(EditAnywhere)
	UEquipmentConfig* EquipmentConfig;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<APropertyActor> EquipmentActorClass;

	/* --- 敌人生成 --- */
	UPROPERTY(EditAnywhere)
	UCharacterClassInfo* CharacterInfos;

	int EnemySpawnerNumber = 0;

	UPROPERTY()
	TSet<ARLEnemy*> Enemies;


	// 
	UPROPERTY(EditAnywhere)
	TSubclassOf<ARLCharacter> PlayerCharacterClass;
	
	bool bInit = false;
};
