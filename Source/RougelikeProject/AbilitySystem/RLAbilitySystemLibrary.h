// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "RougelikeProject/UI/WidgetController/AbilityMenuWidgetController.h"
#include "RougelikeProject/UI/WidgetController/AttributeMenuWidgetController.h"
#include "RougelikeProject/UI/WidgetController/OverlayWidgetController.h"
#include "RLAbilitySystemLibrary.generated.h"

/**
 * 
 */
UCLASS()
class ROUGELIKEPROJECT_API URLAbilitySystemLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	// 提供一个世界上下文，就返回对应 WidgetController
	UFUNCTION(BlueprintPure, Category="WAbilitySystemLibary|WidgetController")
	static UOverlayWidgetController* GetOverlayWidgetController(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, Category="WAbilitySystemLibary|WidgetController")
	static UAttributeMenuWidgetController* GetAttributeMenuWidgetController(const UObject* WorldContextObject);
	
	UFUNCTION(BlueprintPure, Category="WAbilitySystemLibary|WidgetController")
	static UAbilityMenuWidgetController* GetAbilityMenuWidgetController(const UObject* WorldContextObject);
};
