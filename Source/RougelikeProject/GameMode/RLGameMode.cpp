// Fill out your copyright notice in the Description page of Project Settings.


#include "RLGameMode.h"

#include "Kismet/GameplayStatics.h"
#include "RougelikeProject/UI/HUD/RLHUD.h"

void ARLGameMode::BeginPlay()
{
	Super::BeginPlay();
	CurrentLevelNodeIndex = -1;
	LastLevelNodeIndex = -1;
}

void ARLGameMode::StartLevel(int NodeIndex, FName LevelName)
{
	if(RLHUD == nullptr)
		RLHUD = Cast<ARLHUD>(UGameplayStatics::GetPlayerController(this, 0)->GetHUD());
	RLHUD->ShowLevelMap(false);

	CurrentLevelName = LevelName;
	LastLevelNodeIndex = CurrentLevelNodeIndex;
	CurrentLevelNodeIndex = NodeIndex;
	
	LoadLevel(LevelName);
	RestartPlayer(UGameplayStatics::GetPlayerController(this, 0));
}

void ARLGameMode::EndLevel()
{
	if(RLHUD == nullptr)
		RLHUD = Cast<ARLHUD>(UGameplayStatics::GetPlayerController(this, 0)->GetHUD());
	RLHUD->ShowLevelMap(true);
	RLHUD->ShowOverlay(false);
	
	RLHUD->UpdateLevel(CurrentLevelNodeIndex, LastLevelNodeIndex); // 更新地图

	APawn* Pawn = UGameplayStatics::GetPlayerPawn(this, 0);
	if(Pawn)
		Pawn->Destroy();
	UnLoadCurrentLevel();
}

void ARLGameMode::LoadLevel(FName LevelName)
{
	FLatentActionInfo LatentActionInfo;
	UGameplayStatics::LoadStreamLevel(GetWorld(), LevelName, true, false, LatentActionInfo);
}

void ARLGameMode::UnLoadCurrentLevel()
{
	FLatentActionInfo LatentActionInfo;
	UGameplayStatics::UnloadStreamLevel(GetWorld(), CurrentLevelName, LatentActionInfo, false);
}
