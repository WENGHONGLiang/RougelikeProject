// Fill out your copyright notice in the Description page of Project Settings.


#include "ExecCal_SkillAttackDamage.h"

#include "RougelikeProject/AbilitySystem/RLGameplayTags.h"
#include "RougelikeProject/ArributeBaseSet/AttributeSetBase.h"

struct SkillDamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(SkillIntensity);
	DECLARE_ATTRIBUTE_CAPTUREDEF(Defense);

	SkillDamageStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAttributeSetBase, SkillIntensity, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAttributeSetBase, Defense, Target, false);
	}
};

static const SkillDamageStatics& GetSkillDamageStatic()
{
	static SkillDamageStatics SDStatic;
	return SDStatic;
}

UExecCal_SkillAttackDamage::UExecCal_SkillAttackDamage()
{
	RelevantAttributesToCapture.Add(GetSkillDamageStatic().SkillIntensityDef);
	RelevantAttributesToCapture.Add(GetSkillDamageStatic().DefenseDef);
}

void UExecCal_SkillAttackDamage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
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

	/* 技能强度 */
	float SourceDamage = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetSkillDamageStatic().SkillIntensityDef, EvaluationParameters, SourceDamage);
	SourceDamage = FMath::Max<float>(SourceDamage, 0.f);
	
	/* 护甲 */
	float TargetDefense = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetSkillDamageStatic().DefenseDef, EvaluationParameters, TargetDefense);
	TargetDefense = FMath::Max<float>(TargetDefense, 0.f);

	float RealDamage = BaseDamage * SourceDamage; // 基础伤害
	RealDamage = RealDamage - TargetDefense; // 护甲

	const FGameplayModifierEvaluatedData EvaluatedData(UAttributeSetBase::GetIncomingDamageAttribute(), EGameplayModOp::Additive, RealDamage);
	OutExecutionOutput.AddOutputModifier(EvaluatedData);
}
