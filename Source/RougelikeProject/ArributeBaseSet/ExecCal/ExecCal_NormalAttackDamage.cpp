// Fill out your copyright notice in the Description page of Project Settings.


#include "ExecCal_NormalAttackDamage.h"
#include "RougelikeProject/AbilitySystem/RLGameplayTags.h"
#include "RougelikeProject/ArributeBaseSet/AttributeSetBase.h"
#include "Math/UnrealMathUtility.h"

struct NormalDamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Damage);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalDamageRatio);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalDamageMultiple);
	
	DECLARE_ATTRIBUTE_CAPTUREDEF(Defense);

	NormalDamageStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAttributeSetBase, Damage, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAttributeSetBase, CriticalDamageRatio, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAttributeSetBase, CriticalDamageMultiple, Source, false);
		
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAttributeSetBase, Defense, Target, false);
	}
};

static const NormalDamageStatics& GetNormalDamageStatic()
{
	static NormalDamageStatics NDStatic;
	return NDStatic;
}

UExecCal_NormalAttackDamage::UExecCal_NormalAttackDamage()
{
	RelevantAttributesToCapture.Add(GetNormalDamageStatic().DamageDef);
	RelevantAttributesToCapture.Add(GetNormalDamageStatic().CriticalDamageRatioDef);
	RelevantAttributesToCapture.Add(GetNormalDamageStatic().CriticalDamageMultipleDef);
	
	RelevantAttributesToCapture.Add(GetNormalDamageStatic().DefenseDef);
}

void UExecCal_NormalAttackDamage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	const UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
	const UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();

	AActor* SourceAvatar = SourceASC ? SourceASC->GetAvatarActor() : nullptr;
	AActor* TargetAvatar = TargetASC ? TargetASC->GetAvatarActor() : nullptr;

	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();
	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	float BaseDamage = Spec.GetSetByCallerMagnitude(FRLGameplayTags::Get().GE_Damage); // 对应 Ability 中的 AssignTagSetByCallerMagnitude // 能力的基础伤害

	/* 攻击力 */
	float SourceDamage = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetNormalDamageStatic().DamageDef, EvaluationParameters, SourceDamage);
	SourceDamage = FMath::Max<float>(SourceDamage, 0.f);
	
	/* 暴击率 */
	float SourceCriticalDamageRatio = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetNormalDamageStatic().CriticalDamageRatioDef, EvaluationParameters, SourceCriticalDamageRatio);
	SourceCriticalDamageRatio = FMath::Max<float>(SourceCriticalDamageRatio, 0.f);
	
	/* 暴击伤害倍率 */
	float SourceCriticalDamageMultiple = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetNormalDamageStatic().CriticalDamageMultipleDef, EvaluationParameters, SourceCriticalDamageMultiple);
	SourceCriticalDamageMultiple = FMath::Max<float>(SourceCriticalDamageMultiple, 0.f);
	
	/* 护甲 */
	float TargetDefense = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetNormalDamageStatic().DefenseDef, EvaluationParameters, TargetDefense);
	TargetDefense = FMath::Max<float>(TargetDefense, 0.f);

	const bool bCriticalHit = FMath::RandRange(1, 100) < SourceCriticalDamageRatio * 100; // 是否暴击

	float RealDamage = BaseDamage * SourceDamage; // 基础伤害
	RealDamage = bCriticalHit ? RealDamage * SourceCriticalDamageMultiple : RealDamage; // 暴击加成
	RealDamage = RealDamage - TargetDefense; // 护甲

	const FGameplayModifierEvaluatedData EvaluatedData(UAttributeSetBase::GetIncomingDamageAttribute(), EGameplayModOp::Additive, RealDamage);
	OutExecutionOutput.AddOutputModifier(EvaluatedData);
}
