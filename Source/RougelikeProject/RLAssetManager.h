// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "RLAssetManager.generated.h"

/**
 * 
 */
UCLASS()
class ROUGELIKEPROJECT_API URLAssetManager : public UAssetManager
{
	GENERATED_BODY()
	
public:
	URLAssetManager& Get();

protected:
	virtual void StartInitialLoading() override;
};
