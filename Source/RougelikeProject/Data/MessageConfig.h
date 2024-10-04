// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "MessageConfig.generated.h"

/* 在 OverlayWidget 上展示的消息结构，用于显示提示、道具信息等内容 */
USTRUCT(BlueprintType)
struct FRLMessageInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	FGameplayTag MessageTag = FGameplayTag();
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText Message = FText();
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UTexture2D* Image = nullptr;
};

UCLASS()
class ROUGELIKEPROJECT_API UMessageConfig : public UDataAsset
{
	GENERATED_BODY()
	
public:
	// 根据 Tag 查找 IA
	FRLMessageInfo FindMessageTag(const FGameplayTag& MessageTag, bool bLogNotFound = false);
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FRLMessageInfo> Messages;
};
