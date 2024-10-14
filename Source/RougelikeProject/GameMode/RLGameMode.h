// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
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
private:
	int CurrentLevelNodeIndex;
	int LastLevelNodeIndex;
	FName CurrentLevelName;
	ARLHUD* RLHUD;
	
public:
	void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void StartLevel(int NodeIndex, FName LevelName);

	UFUNCTION(BlueprintCallable)
	void EndLevel();

private:
	void LoadLevel(FName LevelName);
	void UnLoadCurrentLevel();
};
