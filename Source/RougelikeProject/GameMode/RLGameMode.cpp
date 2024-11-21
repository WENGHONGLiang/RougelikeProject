// Fill out your copyright notice in the Description page of Project Settings.


#include "RLGameMode.h"

#include "Kismet/GameplayStatics.h"
#include "RougelikeProject/Actor/RLAbilityActor.h"
#include "RougelikeProject/Actor/RLEquipmentActor.h"
#include "RougelikeProject/UI/HUD/RLHUD.h"
#include "UniversalObjectLocators/UniversalObjectLocatorUtils.h"

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
	if(!bInit)
	{
		DefaultPawnClass = PlayerCharacterClass;
		bInit = true;
	}
	else
	{
		APawn* Pawn = UGameplayStatics::GetPlayerPawn(this, 0);
		if(Pawn)
			Pawn->Destroy();
	}
	
	RestartPlayer(UGameplayStatics::GetPlayerController(this, 0));
}

void ARLGameMode::EndLevel()
{
	if(RLHUD == nullptr)
		RLHUD = Cast<ARLHUD>(UGameplayStatics::GetPlayerController(this, 0)->GetHUD());
	RLHUD->ShowLevelMap(true);
	RLHUD->ShowBattleWidget(false);
	
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

void ARLGameMode::SpawnAbilityActorAtLocation(FGameplayTag AbilityTag, FVector Location, float AbilityLevel)
{
	ARLAbilityActor* AbilityActor = Cast<ARLAbilityActor>(GetWorld()->SpawnActor(AbilityActorClass, &Location));
	FRLAbilityInfo Info = AbilityConfig->FindAbilityInfo(AbilityTag);
	Info.AbilityLevel = AbilityLevel;
	
	AbilityActor->InitAbilityActor(Info);
}

void ARLGameMode::SpawnAblityActorAroundPlayer(FGameplayTag AbilityTag, float AbilityLevel)
{
	FVector PlayerLocation = UGameplayStatics::GetPlayerPawn(this, 0)->GetActorLocation();
	SpawnAbilityActorAtLocation(AbilityTag, FVector(PlayerLocation.X + FMath::RandRange(-90, 90), PlayerLocation.Y + FMath::RandRange(-90, 90), 0), AbilityLevel);
}

float ARLGameMode::GetAbilityBaseDamageWithAbilityTag(FGameplayTag AbilityTag)
{
	FRLAbilityInfo& Info = AbilityConfig->FindAbilityInfo(AbilityTag);
	
	return Info.AbilityBaseDamage;
}

void ARLGameMode::SpawnEquipmentActorAtLocation(FGameplayTag EquipmentTag, FVector3d Location, float EquipmentLevel)
{
	ARLEquipmentActor* EquipmentActor = Cast<ARLEquipmentActor>(GetWorld()->SpawnActor(EquipmentActorClass, &Location));
	FRLEquipmentInfo Info = EquipmentConfig->FindEquipmentInfo(EquipmentTag);
	Info.EquipmentLevel = EquipmentLevel;
	
	EquipmentActor->InitEquipmentActor(Info);
}

void ARLGameMode::SpawnEquipmentActorAroundPlayer(FGameplayTag EquipmentTag, float EquipmentLevel)
{
	FVector PlayerLocation = UGameplayStatics::GetPlayerPawn(this, 0)->GetActorLocation();
	SpawnEquipmentActorAtLocation(EquipmentTag, FVector(PlayerLocation.X + FMath::RandRange(-90, 90), PlayerLocation.Y + FMath::RandRange(-90, 90), 0), EquipmentLevel);
}

ARLEnemy* ARLGameMode::SpawnEnemyAtLocation(ECharacterType EnemyType, FVector Location)
{
	FCharacterClassDefaultInfo Info = CharacterInfos->GetClassDefaultInfo(EnemyType);
	ARLEnemy* Enemy = Cast<ARLEnemy>(GetWorld()->SpawnActor(Info.CharacterClass, &Location));
	Enemies.Add(Enemy);
	return Enemy;
}

void ARLGameMode::CheckEnemyAliveState()
{
	for(auto Enemy : Enemies)
	{
		if(!Enemy || !Enemy->GetCharacterAlive())
			Enemies.Remove(Enemy);
	}
}

TSet<ARLEnemy*> ARLGameMode::GetEnemiesInRange(const FVector TargetPosition, float Distance)
{
	CheckEnemyAliveState();
	TSet<ARLEnemy*> RetEnemies;
	for(auto Enemy : Enemies)
	{
		if(FVector::Distance(Enemy->GetActorLocation(), TargetPosition) <= Distance)
			RetEnemies.Add(Enemy);
	}
	return RetEnemies;
}

void ARLGameMode::ReduceEnmeySpawnerNumber()
{
	EnemySpawnerNumber--;
	CheckEnemyAliveState();
	if(EnemySpawnerNumber == 0)
		OnEnemyClear.Broadcast();
}

