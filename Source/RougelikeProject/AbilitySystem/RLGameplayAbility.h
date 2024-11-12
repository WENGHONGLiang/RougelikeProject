// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "RougelikeProject/CharacterSet/RLCharacterBase.h"
#include "RLGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class ROUGELIKEPROJECT_API URLGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
	
public:
	URLGameplayAbility();

	void InitAbilityBaseDamage(float Amount);
	
	UPROPERTY(EditDefaultsOnly, Category="Input")
	FGameplayTag StartupInputTag;

	// 基础伤害
	// 普通实际伤害 = 基础伤害 * 攻击方攻击力 - 被攻击方护甲值
	// 技能实际伤害 = 基础伤害 * 攻击方技能强度 - 被攻击方护甲值
	// 在 AbilityConfig 配置
	UPROPERTY(BlueprintReadOnly)
	float BaseDamage;

	UFUNCTION(BlueprintPure)
	ARLCharacterBase* GetCharacterBase();
};
