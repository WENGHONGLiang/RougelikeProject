// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "AttributeInfo.generated.h"

// 用于 Controller 向 Widget 打包传递更新的 Attribute
USTRUCT(BlueprintType)
struct FRLAttributeInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag AttributeTag = FGameplayTag();

	// 需要用 Widget 显示的用 FText
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText AttributeName = FText();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText AttributeDescription = FText();

	// 实际改变值，希望 Controller 来编辑，Widget 蓝图来查看。而不是在编辑器中改变，所以编辑器中不暴露。
	UPROPERTY(BlueprintReadOnly)
	float AttributeValue = 0.f;
};

UCLASS()
class ROUGELIKEPROJECT_API UAttributeInfo : public UDataAsset
{
	GENERATED_BODY()
public:
	// 通过 Tag 在 DA 中找到对应数据包Data
	FRLAttributeInfo FindAttributeInfoTag(const FGameplayTag& AttributeTag, bool bLogNotFound = false);
	
	// DA 中包含一组 Attribute 结构 // 在编辑器中编辑
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FRLAttributeInfo> AttributeInformation;
};
