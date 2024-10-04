// Fill out your copyright notice in the Description page of Project Settings.


#include "RLAssetManager.h"

#include "AbilitySystem/RLGameplayTags.h"
#include "AbilitySystemGlobals.h"

URLAssetManager& URLAssetManager::Get()
{
	check(GEngine);

	return *Cast<URLAssetManager>(GEngine->AssetManager);
}

// 在编辑器资源初始化时，就初始化所有自定义的 Native GameplayTag
void URLAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();

	FRLGameplayTags::InitializeNativeGameplayTags();

	UAbilitySystemGlobals::Get().InitGlobalData();
}
