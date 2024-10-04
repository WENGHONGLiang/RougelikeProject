// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "AttributeSetBase.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDelegateSignature);

template<class T>
using TStaticFuncPrt = typename TBaseStaticDelegateInstance<T, FDefaultDelegateUserPolicy>::FFuncPtr;

// GE 生效后用于记录 源、目标对象的结构体
USTRUCT()
struct FEffectProperties
{
	GENERATED_BODY()

	FEffectProperties(){};

	FGameplayEffectContextHandle EffectContextHandle;

	UPROPERTY()
	UAbilitySystemComponent* SourceASC = nullptr;

	UPROPERTY()
	AActor* SourceAvatarActor = nullptr;

	UPROPERTY()
	AController* SourceController = nullptr;

	UPROPERTY()
	ACharacter* SourceCharacter = nullptr;

	UPROPERTY()
	UAbilitySystemComponent* TargetASC = nullptr;

	UPROPERTY()
	AActor* TargetAvatarActor = nullptr;

	UPROPERTY()
	AController* TargetController = nullptr;

	UPROPERTY()
	ACharacter* TargetCharacter = nullptr;
};

/**
 * 属性基类，带有宏与其他通用方法
 */
UCLASS(BlueprintType)
class UAttributeSetBase : public UAttributeSet {
	GENERATED_BODY()
public:
	UAttributeSetBase();
	
	// TStaticFuncPrt<返回值 (参数)> // Tag 绑定一个返回 Attribute 的函数指针
	TMap<FGameplayTag, TStaticFuncPrt<FGameplayAttribute(/*参数*/)>> TagsToAttributes;

	/* --- 最大生命 --- */
	UPROPERTY(BlueprintReadOnly, Category = "Vital Attribute")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UAttributeSetBase, MaxHealth);
	
	/* --- 当前生命 --- */
	UPROPERTY(BlueprintReadOnly, Category = "Vital Attribute")
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UAttributeSetBase, Health);
	
	/* --- 基础伤害 --- */
	UPROPERTY(BlueprintReadOnly, Category = "Vital Attribute")
	FGameplayAttributeData Damage;
	ATTRIBUTE_ACCESSORS(UAttributeSetBase, Damage);
	
	/* --- 技能伤害 --- */
	UPROPERTY(BlueprintReadOnly, Category = "Vital Attribute")
	FGameplayAttributeData SkillIntensity;
	ATTRIBUTE_ACCESSORS(UAttributeSetBase, SkillIntensity);
	
	/* --- 攻击速度 --- */
	UPROPERTY(BlueprintReadOnly, Category = "Vital Attribute")
	FGameplayAttributeData AttackSpeed;
	ATTRIBUTE_ACCESSORS(UAttributeSetBase, AttackSpeed);
	
	/* --- 护甲 --- */
	UPROPERTY(BlueprintReadOnly, Category = "Vital Attribute")
	FGameplayAttributeData Defense;
	ATTRIBUTE_ACCESSORS(UAttributeSetBase, Defense);
	
	/* --- 移动速度 --- */
	UPROPERTY(BlueprintReadOnly, Category = "Vital Attribute")
	FGameplayAttributeData MoveSpeed;
	ATTRIBUTE_ACCESSORS(UAttributeSetBase, MoveSpeed);
	
	/* --- 暴击率 --- */
	UPROPERTY(BlueprintReadOnly, Category = "Vital Attribute")
	FGameplayAttributeData CriticalDamageRatio;
	ATTRIBUTE_ACCESSORS(UAttributeSetBase, CriticalDamageRatio);
	
	/* --- 暴击伤害倍数 --- */
	UPROPERTY(BlueprintReadOnly, Category = "Vital Attribute")
	FGameplayAttributeData CriticalDamageMultiple;
	ATTRIBUTE_ACCESSORS(UAttributeSetBase, CriticalDamageMultiple);

	/* --- Meta属性，作为中间值 --- */
	UPROPERTY(BlueprintReadOnly, Category = "Meta Attributes")
	FGameplayAttributeData IncomingDamage;
	ATTRIBUTE_ACCESSORS(UAttributeSetBase, IncomingDamage);
	
	// 数值变化预处理
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;

	// GE 执行结束后，把相关参数保留下来
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

	FDelegateSignature OnCharacterDie;
	
private:
	void SetEffectProperties(const FGameplayEffectModCallbackData& Data, FEffectProperties& Props);
};
