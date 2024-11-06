// Fill out your copyright notice in the Description page of Project Settings.


#include "RLGameplayTags.h"

#include "GameplayTagsManager.h"

// 外部声明一次静态变量，作为单实例
FRLGameplayTags FRLGameplayTags::GameplayTags;

void FRLGameplayTags::InitializeNativeGameplayTags()
{
	/* --- Attribute Tag --- */
	
	GameplayTags.Attributes_MaxHealth = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.MaxHealth"),
		FString("Player Max Health")
		);

	
	GameplayTags.Attributes_Health = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Health"),
		FString("Player Current Health")
		);

	
	GameplayTags.Attributes_Damage = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Damage"),
		FString("Attack Damage")
		);

	GameplayTags.Attributes_SkillIntensity = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.SkillIntensity"),
		FString("Skill Intensity")
		);

	GameplayTags.Attributes_AttackSpeed = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.AttackSpeed"),
		FString("Attack Speed")
		);
	
	GameplayTags.Attributes_Defense = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Defense"),
		FString("Defense")
		);
	
	GameplayTags.Attributes_MoveSpeed = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.MoveSpeed"),
		FString("Move Speed")
		);
	
	GameplayTags.Attributes_CriticalDamageRatio = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.CriticalDamageRatio"),
		FString("Critical Damage Ratio")
		);
	
	GameplayTags.Attributes_CriticalDamageMultiple = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.CriticalDamageMultiple"),
		FString("Critical Damage Multiple")
		);

	/* --- Input Tag --- */

	GameplayTags.InputTag_Empty = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Input.Empty")
		);
	
	GameplayTags.InputTag_LMB = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Input.LMB"),
		FString("Left Mouse Button Input")
		);
	
	GameplayTags.InputTag_RMB = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Input.RMB"),
		FString("Right Mouse Button Input")
		);
	
	GameplayTags.InputTag_1 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Input.1"),
		FString("1 Button Input")
		);
	
	GameplayTags.InputTag_2 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Input.2"),
		FString("2 Button Input")
		);
	
	GameplayTags.InputTag_3 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Input.3"),
		FString("3 Button Input")
		);
	
	GameplayTags.InputTag_4 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Input.4"),
		FString("4 Button Input")
		);
	
	GameplayTags.InputTag_Interact = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Input.Interact")
		);
	
	GameplayTags.InputTag_PickUp = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Input.PickUp")
		);
	
	GameplayTags.InputTag_Roll = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Input.Roll")
		);

	
	/* --- Message Tag --- */
	
	GameplayTags.Message_Tip = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Message.Tip")
		);
	
	GameplayTags.Message_Property = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Message.Property")
		);

	GameplayTags.Message_Tip_PickUp = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Message.Tip.PickUp")
		);
	
	GameplayTags.Message_Tip_Test1 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Message.Tip.Test1")
		);

	GameplayTags.Message_Tip_Test2 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Message.Tip.Test2")
		);
	
	GameplayTags.Message_Property_Test1 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Message.Property.Test1")
		);
	
	GameplayTags.Message_Property_Test2 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Message.Property.Test2")
		);
	
	GameplayTags.GE_Damage = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("GE.Damage")
		);
	
	GameplayTags.GE_Damage_Normal = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("GE.Damage.Normal")
		);
	
	GameplayTags.HeroAbility_Empty = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("HeroAbility.Empty")
		);
	
	GameplayTags.HeroAbility_Roll = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("HeroAbility.Roll")
		);
}