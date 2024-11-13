// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CharacterInfo.generated.h"

class ARLCharacterBase;
class UGameplayEffect;

// 角色 / 敌人类型配置
UENUM(BlueprintType)
enum class ECharacterType : uint8
{
	Hero_1,
	Hero_2,
	Enemy_1,
	Enemy_2,
	Enemy_3,

	// 其它角色或敌人类型...
};

USTRUCT(BlueprintType)
struct FCharacterClassDefaultInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ARLCharacterBase> CharacterClass;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayEffect> Attributes;
	
	UPROPERTY(EditDefaultsOnly)
	FText CharacterName = FText();
	
	UPROPERTY(EditDefaultsOnly)
	FText CharacterDescription = FText();

	// 杀死敌人得到金币
	UPROPERTY(EditDefaultsOnly)
	int32 RewardMoney;
	
	UPROPERTY(EditDefaultsOnly)
	int32 RewardExp;

	// 其它特性，如初始技能...
};

UCLASS()
class ROUGELIKEPROJECT_API UCharacterClassInfo : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = "Character Class Defaults")
	TMap<ECharacterType, FCharacterClassDefaultInfo> CharacterClassInformation;

	FCharacterClassDefaultInfo GetClassDefaultInfo(ECharacterType CharacterClass);
};
