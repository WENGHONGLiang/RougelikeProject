// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSetBase.h"
#include "HeroAttributeSet.generated.h"

/**
 * 
 */
UCLASS()
class UHeroAttributeSet : public UAttributeSetBase {
	GENERATED_BODY()

public:
	UHeroAttributeSet();	
	//
	UPROPERTY(BlueprintReadOnly)
	FGameplayAttributeData Move;
	ATTRIBUTE_ACCESSORS(UHeroAttributeSet, Move);	
};
