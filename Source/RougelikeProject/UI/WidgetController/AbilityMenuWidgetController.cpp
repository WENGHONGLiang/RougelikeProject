// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityMenuWidgetController.h"

#include "AbilitySystemComponent.h"
#include "Net/RepLayout.h"
#include "RougelikeProject/AbilitySystem/RLAbilitySystemComponent.h"
#include "RougelikeProject/AbilitySystem/RLGameplayAbility.h"
#include "RougelikeProject/AbilitySystem/RLGameplayTags.h"
#include "RougelikeProject/Player/RLPlayerController.h"

UAbilityMenuWidgetController::UAbilityMenuWidgetController()
{
	bDraggingAbility = false;
	SourceAbilityTag = FRLGameplayTags::Get().HeroAbility_Empty;
	SourceInputTag = FGameplayTag::EmptyTag;
}

void UAbilityMenuWidgetController::BindCallbacksToDependencies()
{
	URLAbilitySystemComponent* RLASC = Cast<URLAbilitySystemComponent>(AbilitySystemComponent);

	// 新技能添加事件
	RLASC->OnGameplayAbilityGive.AddLambda(
		[this](const FGameplayTag InputTag, const FGameplayTag AbilityTag, float AbilityBaseDamage)
		{
			// 用 技能标签 找到技能信息，再跟 键位标签 一起传给 Widget 使用
			FRLAbilityInfo& AbilityInfo = AbilityConfig->FindAbilityInfo(AbilityTag);
			AbilityInfo.AbilityBaseDamage = AbilityBaseDamage;
			OnUpdateAbility.Broadcast(InputTag, AbilityInfo);
		});

	// 冷却时间更新
	RLASC->OnCoolDownTimeUpdate.AddLambda(
		[this](const FGameplayTag InputTag, float Percent)
		{
			OnUpdateCoolDown.Broadcast(InputTag, Percent);
		});

	// 鼠标点击 // 暂时用于技能面板的操作
	Cast<ARLPlayerController>(PlayerController)->OnMouseClickEvent.AddDynamic(this, &UAbilityMenuWidgetController::OnMouseClick);
	
}

void UAbilityMenuWidgetController::BroadcastInitialValues()
{
	// 空技能
	FRLGameplayTags GameplayTags = FRLGameplayTags::Get();
	FRLAbilityInfo& EmptyAbilityInfo = AbilityConfig->FindAbilityInfo(GameplayTags.HeroAbility_Empty);
	OnUpdateAbility.Broadcast(GameplayTags.InputTag_LMB, EmptyAbilityInfo);
	OnUpdateAbility.Broadcast(GameplayTags.InputTag_1, EmptyAbilityInfo);
	OnUpdateAbility.Broadcast(GameplayTags.InputTag_2, EmptyAbilityInfo);
	OnUpdateAbility.Broadcast(GameplayTags.InputTag_3, EmptyAbilityInfo);
	OnUpdateAbility.Broadcast(GameplayTags.InputTag_4, EmptyAbilityInfo);

	// 角色已有技能
	for(auto& AbilitySpec : AbilitySystemComponent->GetActivatableAbilities())
	{
		URLGameplayAbility* RLAbility = Cast<URLGameplayAbility>(AbilitySpec.Ability);
		FRLAbilityInfo& AbilityInfo = AbilityConfig->FindAbilityInfo(RLAbility->AbilityTags.First());
		AbilityInfo.AbilityBaseDamage = RLAbility->BaseDamage;
		OnUpdateAbility.Broadcast(AbilitySpec.DynamicAbilityTags.First(), AbilityInfo);
	}
}

void UAbilityMenuWidgetController::OnAbilityButtonClicked(const FGameplayTag& InputTag, const FGameplayTag& AbilityTag)
{
	FRLAbilityInfo& EmptyAbilityInfo = AbilityConfig->FindAbilityInfo(FRLGameplayTags::Get().HeroAbility_Empty);
	
	if(!AbilityTag.MatchesTagExact(FRLGameplayTags::Get().HeroAbility_Empty) && !bDraggingAbility) // 选中源技能
	{
		SourceInputTag = InputTag;
		SourceAbilityTag = AbilityTag;
		
		bDraggingAbility = true;

		FRLAbilityInfo& SourceAbilityInfo = AbilityConfig->FindAbilityInfo(SourceAbilityTag);
		
		OnUpdateAbility.Broadcast(SourceInputTag, EmptyAbilityInfo);

		OnStartDrag.Broadcast(SourceAbilityInfo.AbilityImage);
	}
	else if(bDraggingAbility) // 选中目标技能
	{
		FRLAbilityInfo& SourceAbilityInfo = AbilityConfig->FindAbilityInfo(SourceAbilityTag);
		FRLAbilityInfo& TargetAbilityInfo = AbilityConfig->FindAbilityInfo(AbilityTag);

		// 交换源技能和目标技能（UI）
		OnUpdateAbility.Broadcast(SourceInputTag, TargetAbilityInfo);
		OnUpdateAbility.Broadcast(InputTag, SourceAbilityInfo);
		
		// 实际技能绑定交换
		URLAbilitySystemComponent* RlAbilitySystemComponent = Cast<URLAbilitySystemComponent>(AbilitySystemComponent);
		RlAbilitySystemComponent->UpdateCharacterAbilitiesInput(SourceAbilityTag, InputTag);
		RlAbilitySystemComponent->UpdateCharacterAbilitiesInput(TargetAbilityInfo.AbilityTag, SourceInputTag);

		// Cooldown的UI先清掉，如果实际有Cooldown还会更新
		OnUpdateCoolDown.Broadcast(SourceInputTag, 0);
		OnUpdateCoolDown.Broadcast(InputTag, 0);
		
		bDraggingAbility = false;
		SourceAbilityTag = FRLGameplayTags::Get().HeroAbility_Empty;
		SourceInputTag = FGameplayTag::EmptyTag;
		
		OnStopDrag.Broadcast(EmptyAbilityInfo.AbilityImage);
	}
}

void UAbilityMenuWidgetController::OnMouseClick()
{
	if(bDraggingAbility)
	{
		FTimerDelegate Delegate = FTimerDelegate::CreateUObject(this, &UAbilityMenuWidgetController::CancelDrag);
		GetWorld()->GetTimerManager().SetTimer(DelayReleaseDragTimerHandle, Delegate, 0.025f, false);
	}
}

void UAbilityMenuWidgetController::CancelDrag()
{
	FRLAbilityInfo& EmptyAbilityInfo = AbilityConfig->FindAbilityInfo(FRLGameplayTags::Get().HeroAbility_Empty);
	OnStopDrag.Broadcast(EmptyAbilityInfo.AbilityImage);
	
	FRLAbilityInfo& SourceAbilityInfo = AbilityConfig->FindAbilityInfo(SourceAbilityTag);
	OnUpdateAbility.Broadcast(SourceInputTag, SourceAbilityInfo);

	bDraggingAbility = false;
}