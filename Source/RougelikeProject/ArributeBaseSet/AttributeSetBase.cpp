// Fill out your copyright notice in the Description page of Project Settings.


#include "AttributeSetBase.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "RougelikeProject/AbilitySystem/RLGameplayTags.h"
#include "GameplayEffectExtension.h"
#include "GameFramework/Character.h"

UAttributeSetBase::UAttributeSetBase()
{
	const FRLGameplayTags& GameplayTags = FRLGameplayTags::Get();

	// 绑定 Tag 映射
	TagsToAttributes.Add(GameplayTags.Attributes_MaxHealth, GetMaxHealthAttribute); 
	TagsToAttributes.Add(GameplayTags.Attributes_Health, GetHealthAttribute); 
	TagsToAttributes.Add(GameplayTags.Attributes_Damage, GetDamageAttribute); 
	TagsToAttributes.Add(GameplayTags.Attributes_Defense, GetDefenseAttribute); 
	TagsToAttributes.Add(GameplayTags.Attributes_AttackSpeed, GetAttackSpeedAttribute); 
	TagsToAttributes.Add(GameplayTags.Attributes_MoveSpeed, GetMoveSpeedAttribute); 
	TagsToAttributes.Add(GameplayTags.Attributes_SkillIntensity, GetSkillIntensityAttribute); 
	TagsToAttributes.Add(GameplayTags.Attributes_CriticalDamageRatio, GetCriticalDamageRatioAttribute); 
	TagsToAttributes.Add(GameplayTags.Attributes_CriticalDamageMultiple, GetCriticalDamageMultipleAttribute); 
}

void UAttributeSetBase::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);
	
	// Clamp // 只是限制了查询的NewValue，并未改到本身
	if(Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxHealth());
	}
}

void UAttributeSetBase::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	FEffectProperties Props;
	SetEffectProperties(Data, Props);

	// Clamp实际值
	if(Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(), 0.f, GetMaxHealth()));
		UE_LOG(LogTemp, Warning, TEXT("Change Health on %s, Health: %f"), *Props.TargetAvatarActor->GetName(), GetHealth());
	}

	// 使用 IncomingDamage 做一系列属性计算，最终作用于 Health
	if(Data.EvaluatedData.Attribute == GetIncomingDamageAttribute())
	{
		const float LocalIncomingDamage = GetIncomingDamage();
		SetIncomingDamage(0.f);

		if(LocalIncomingDamage > 0.f)
		{
			const float NewHealth = GetHealth() - LocalIncomingDamage;
			SetHealth(FMath::Clamp(NewHealth, 0.f, GetMaxHealth()));

			const bool bFatal = NewHealth <= 0.f;
			if(bFatal)
			{
				OnCharacterDie.Broadcast();
			}
		}
	}
	
	// TODO: 使用Props处理GE逻辑
	
}

void UAttributeSetBase::SetEffectProperties(const FGameplayEffectModCallbackData& Data, FEffectProperties& Props)
{
	// Source = couser of the effect, Target = target of the effect

	// Source
	Props.EffectContextHandle = Data.EffectSpec.GetContext();
	Props.SourceASC = Props.EffectContextHandle.GetOriginalInstigatorAbilitySystemComponent();

	if(IsValid(Props.SourceASC) && Props.SourceASC->AbilityActorInfo.IsValid() && Props.SourceASC->AbilityActorInfo->AvatarActor.IsValid())
	{
		Props.SourceAvatarActor = Props.SourceASC->AbilityActorInfo->AvatarActor.Get();
		Props.SourceController = Props.SourceASC->AbilityActorInfo->PlayerController.Get();

		// 如果有Pawn但是没得到Controller，手动获取Controller
		if(Props.SourceController == nullptr && Props.SourceAvatarActor != nullptr)
		{
			if(const APawn* Pawn = Cast<APawn>(Props.SourceAvatarActor))
			{
				Props.SourceController = Pawn->GetController();
			}
		}
		if(Props.SourceController)
		{
			Props.SourceCharacter = Cast<ACharacter>(Props.SourceController->GetPawn());
		}
	}

	// Target
	if(Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
	{
		Props.TargetAvatarActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
		Props.TargetController = Data.Target.AbilityActorInfo->PlayerController.Get();
		Props.TargetCharacter = Cast<ACharacter>(Props.TargetAvatarActor);
		Props.TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Props.TargetAvatarActor);
	}
}
