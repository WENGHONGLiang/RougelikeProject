// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "RougelikeProject/Actor/PropertyActor.h"
#include "RougelikeProject/UI/HUD/RLHUD.h"
#include "RougelikeProject/UI/Widget/LevelMapWidget.h"
#include "RougelikeProject/UI/WidgetController/LevelMapWidgetController.h"
#include "RLGameMode.generated.h"

/**
 * 
 */
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

	/* --- 技能 --- */
	void SpawnAblityActorAroundPlayer(FGameplayTag AbilityTag);
	
	void SpawnAbilityActorAtLocation(FGameplayTag AbilityTag, FVector3d Location);
	
private:
	
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
};
