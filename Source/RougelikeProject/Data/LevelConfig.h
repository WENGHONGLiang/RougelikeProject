// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "LevelConfig.generated.h"

enum class ECharacterType : uint8;
// 关卡类型配置
UENUM(BlueprintType)
enum class ELevelType : uint8
{
	// 起始关卡 // 暂时单独列
	Level_Start,
	
	// 对战关卡 // 难度 1 2 3
	Level_Battle_1,
	Level_Battle_2,
	Level_Battle_3,

	// 商店关卡 // 品级 1 2 3
	Level_Shop_1,
	Level_Shop_2,
	Level_Shop_3,

	// Boss关卡 // 暂定只有终点
	Level_Boss,

	// 其它关卡类型...
};

USTRUCT(BlueprintType)
struct FLevelInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	TArray<FName> LevelMapName;

	UPROPERTY(EditDefaultsOnly)
	TMap<ECharacterType, int> EnemyConfig;
	
	// 其它配置 // 如商店配置...
};

UCLASS()
class ROUGELIKEPROJECT_API ULevelConfig : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, Category = "Character Class Defaults")
	TMap<ELevelType, FLevelInfo> LevelInformation;

	FLevelInfo GetLevelInfo(ELevelType LevelType);
};

