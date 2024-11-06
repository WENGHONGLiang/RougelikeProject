// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "CommodityConfig.generated.h"

enum CommodityType
{
	Ability,
	Equipment,
	Item
};

USTRUCT(BlueprintType, Blueprintable)
struct FCommodityInfo
{
	GENERATED_BODY()

	int CommodityID;
	
	// 对应 技能 / 装备 / 道具 的 Tag，可以去各自表中查询实际信息
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag Tag = FGameplayTag();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int Number;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int Cost;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int Level;
	
	// 运行时填写部分
	UPROPERTY(BlueprintReadOnly)
	bool bHasInit = false;
	
	UPROPERTY(BlueprintReadOnly)
	FText Description = FText();

	UPROPERTY(BlueprintReadOnly)
	UTexture2D* CommodityImage = nullptr;

	CommodityType Type;
};

UCLASS()
class ROUGELIKEPROJECT_API UCommodityConfig : public UDataAsset
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FCommodityInfo> AbilityCommodityInfo;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FCommodityInfo> EquipmentCommodityInfo;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FCommodityInfo> ItemCommodityInfo;
	
public:
	FCommodityInfo& FindCommodityInfo(CommodityType commodityType, int commodityID);

	int GetAbilityCommodityNum() const { return AbilityCommodityInfo.Num(); }
	int GetEquipmentCommodityNum() const { return EquipmentCommodityInfo.Num(); }
	int GetItemCommodityNum() const { return ItemCommodityInfo.Num(); }
};
