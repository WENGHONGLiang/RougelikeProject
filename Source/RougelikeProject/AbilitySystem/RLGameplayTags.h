// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

struct FRLGameplayTags
{
public:
	static const FRLGameplayTags& Get() { return GameplayTags; }
	static void InitializeNativeGameplayTags();

	/*
	 * 将初始化的属性记录在实例中
	 * 在其它地方就不需用字符串得到Tag
	 */

	/* 属性 */
	FGameplayTag Attributes_MaxHealth;
	FGameplayTag Attributes_Health;
	FGameplayTag Attributes_Damage;
	FGameplayTag Attributes_SkillIntensity;
	FGameplayTag Attributes_AttackSpeed;
	FGameplayTag Attributes_SkillSpeed;
	FGameplayTag Attributes_Defense;
	FGameplayTag Attributes_MoveSpeed;
	FGameplayTag Attributes_CriticalDamageRatio;
	FGameplayTag Attributes_CriticalDamageMultiple;

	/* 输入键位 */
	FGameplayTag InputTag_Empty;
	
	FGameplayTag InputTag_LMB;
	FGameplayTag InputTag_RMB;
	
	FGameplayTag InputTag_1;
	FGameplayTag InputTag_2;
	FGameplayTag InputTag_3;
	FGameplayTag InputTag_4;
	
	FGameplayTag InputTag_PickUp; // 拾取键
	FGameplayTag InputTag_Interact; // 交互键
	FGameplayTag InputTag_Crush; // 粉碎道具键
	
	FGameplayTag InputTag_Roll; // 闪避键

	/* 消息 */
	FGameplayTag Message_Tip;
	FGameplayTag Message_Property;
	
	FGameplayTag Message_Tip_PickUp;

	/* GE Tag, 用于技能判定 和 数值传递 */
	FGameplayTag GE_Damage;
	FGameplayTag GE_Damage_Normal;

	/* 英雄技能, 大部分技能Tag直接在编辑器添加 */
	FGameplayTag HeroAbility_Empty;
	FGameplayTag HeroAbility_Roll;

	/* 英雄装备, 大部分装备Tag直接在编辑器添加 */
	FGameplayTag Equipment;
	FGameplayTag Equipment_Empty;
	
private:
	// instance
	static FRLGameplayTags GameplayTags;
};
