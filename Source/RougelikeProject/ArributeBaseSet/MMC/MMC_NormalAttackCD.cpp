// Fill out your copyright notice in the Description page of Project Settings.


#include "MMC_NormalAttackCD.h"

#include "RougelikeProject/ArributeBaseSet/AttributeSetBase.h"

UMMC_NormalAttackCD::UMMC_NormalAttackCD()
{
	AttackSpeedDef.AttributeToCapture = UAttributeSetBase::GetAttackSpeedAttribute();
	AttackSpeedDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	AttackSpeedDef.bSnapshot = false;

	RelevantAttributesToCapture.Add(AttackSpeedDef);
}

float UMMC_NormalAttackCD::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	// 评估器 // 可以设置如有某些 Tag 则 MMC 不生效等效果
	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	float AttackSpeed = 0.f;
	GetCapturedAttributeMagnitude(AttackSpeedDef, Spec, EvaluationParameters, AttackSpeed);
	AttackSpeed = FMath::Max<float>(AttackSpeed, 0.f);

	const float LevelReduce = 1.0 - 0.1 * (Spec.GetLevel() - 1); // 每级 -10% 冷却
	
	return LevelReduce * (100.0f / (100.0f + AttackSpeed));
}
