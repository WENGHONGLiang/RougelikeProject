// Fill out your copyright notice in the Description page of Project Settings.


#include "MMC_SkillCD.h"

#include "RougelikeProject/ArributeBaseSet/AttributeSetBase.h"

UMMC_SkillCD::UMMC_SkillCD()
{
	SkillSpeedDef.AttributeToCapture = UAttributeSetBase::GetSkillSpeedAttribute();
	SkillSpeedDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	SkillSpeedDef.bSnapshot = false;

	RelevantAttributesToCapture.Add(SkillSpeedDef);
}

float UMMC_SkillCD::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	// 评估器 // 可以设置如有某些 Tag 则 MMC 不生效等效果
	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	float SkillSpeed = 0.f;
	GetCapturedAttributeMagnitude(SkillSpeedDef, Spec, EvaluationParameters, SkillSpeed);
	SkillSpeed = FMath::Max<float>(SkillSpeed, 0.f);

	const float LevelReduce = 1.0 - 0.1 * (Spec.GetLevel() - 1); // 每级 -10% 冷却
	
	return LevelReduce * (100.0f / (100.0f + SkillSpeed));
}
