// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityMenuWidgetController.h"

#include "AbilitySystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/RepLayout.h"
#include "RougelikeProject/AbilitySystem/RLAbilitySystemComponent.h"
#include "RougelikeProject/AbilitySystem/RLGameplayAbility.h"
#include "RougelikeProject/AbilitySystem/RLGameplayTags.h"
#include "RougelikeProject/GameMode/RLGameMode.h"
#include "RougelikeProject/Player/RLPlayerController.h"
#include "RougelikeProject/Player/RLPlayerState.h"
#include "UObject/FastReferenceCollector.h"

UAbilityMenuWidgetController::UAbilityMenuWidgetController()
{
	bDraggingAbility = false;
	SourceAbilityTag = FRLGameplayTags::Get().HeroAbility_Empty;
	SourceInputTag = FRLGameplayTags::Get().InputTag_Empty;
}

void UAbilityMenuWidgetController::BindCallbacksToDependencies()
{
	URLAbilitySystemComponent* RLASC = Cast<URLAbilitySystemComponent>(AbilitySystemComponent);
	ARLPlayerState* RLPS = Cast<ARLPlayerState>(PlayerState);

	// 新技能添加事件
	RLASC->OnGameplayAbilityGive.AddLambda(
		[this](const FGameplayTag InputTag, const FGameplayTag AbilityTag, float AbilityLevel)
		{
			// 用 技能标签 找到技能信息，再跟 键位标签 一起传给 Widget 使用
			FRLAbilityInfo AbilityInfo = AbilityConfig->FindAbilityInfo(AbilityTag);
			AbilityInfo.AbilityLevel = AbilityLevel;
			OnUpdateAbility.Broadcast(InputTag, AbilityInfo);
		});

	RLASC->OnAbilityLevelUpdate.AddLambda(
		[this](const FGameplayTag InputTag, const FGameplayTag AbilityTag, float AbilityLevel)
		{
			// 用 技能标签 找到技能信息，再跟 键位标签 一起传给 Widget 使用
			FRLAbilityInfo AbilityInfo = AbilityConfig->FindAbilityInfo(AbilityTag);
			AbilityInfo.AbilityLevel = AbilityLevel;
			OnUpdateAbility.Broadcast(InputTag, AbilityInfo);
		});

	// 冷却时间更新
	RLASC->OnCoolDownTimeUpdate.AddLambda(
		[this](const FGameplayTag InputTag, float Percent)
		{
			OnUpdateCoolDown.Broadcast(InputTag, Percent);
		});

	
	RLPS->OnMoneyChanged.AddLambda(
		[this](int32 NewMoney)
		{
			if(!bUpgradeMode) return;
			
			ARLCharacter* RLCharacter = Cast<ARLCharacter>(AbilitySystemComponent->GetAvatarActor());
			for(auto& AbilitySpec : AbilitySystemComponent->GetActivatableAbilities())
			{
				FRLAbilityInfo AbilityInfo = AbilityConfig->FindAbilityInfo(AbilitySpec.Ability->AbilityTags.First());
				AbilityInfo.AbilityLevel = AbilitySpec.Level;
				int NeedMoney = RLCharacter->AbilityLevelCost.GetValueAtLevel(AbilitySpec.Level);
				
				if(AbilitySpec.Level <= 4 && NewMoney >= NeedMoney)
				{
					OnUpdateAbilityUpgradeInfo.Broadcast(AbilitySpec.DynamicAbilityTags.First(), NeedMoney,true);
				}
				else
				{
					OnUpdateAbilityUpgradeInfo.Broadcast(AbilitySpec.DynamicAbilityTags.First(), NeedMoney, false);
				}
			}
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
		FRLAbilityInfo AbilityInfo = AbilityConfig->FindAbilityInfo(RLAbility->AbilityTags.First());
		AbilityInfo.AbilityLevel = AbilitySpec.Level;
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
	else if(bDraggingAbility && !SourceInputTag.MatchesTagExact(FRLGameplayTags::Get().InputTag_Empty)) // 选中目标技能 // 手上的源技能绑定过
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
		SourceInputTag = FRLGameplayTags::Get().InputTag_Empty;
		
		OnStopDrag.Broadcast(EmptyAbilityInfo.AbilityImage);
	}
	else if(bDraggingAbility && SourceInputTag.MatchesTagExact(FRLGameplayTags::Get().InputTag_Empty)) // 选中目标技能 // 手上的技能未绑定
	{
		URLAbilitySystemComponent* RlAbilitySystemComponent = Cast<URLAbilitySystemComponent>(AbilitySystemComponent);
		
		FRLAbilityInfo& SourceAbilityInfo = AbilityConfig->FindAbilityInfo(SourceAbilityTag);
		FRLAbilityInfo& TargetAbilityInfo = AbilityConfig->FindAbilityInfo(AbilityTag);
		FGameplayTag CurrentAbilityTag = SourceAbilityTag;
		
		// 更新手上技能
		if(AbilityTag.MatchesTagExact(FRLGameplayTags::Get().HeroAbility_Empty))
		{
			bDraggingAbility = false;
			OnStopDrag.Broadcast(TargetAbilityInfo.AbilityImage);
		}
		else
		{
			SourceAbilityTag = AbilityTag;
			OnStartDrag.Broadcast(TargetAbilityInfo.AbilityImage);
		}
		
		// 把当前输入键位交给手上技能
		OnUpdateAbility.Broadcast(InputTag, SourceAbilityInfo);
		
		RlAbilitySystemComponent->UpdateCharacterAbilitiesInput(CurrentAbilityTag, InputTag);
		RlAbilitySystemComponent->UpdateCharacterAbilitiesInput(TargetAbilityInfo.AbilityTag, SourceInputTag);
	}
}

bool UAbilityMenuWidgetController::AddAbility(const FGameplayTag& AbilityTag, float AbilityLevel)
{
	if(bDraggingAbility)
	{
		return false;
	}

	URLAbilitySystemComponent* RlAbilitySystemComponent = Cast<URLAbilitySystemComponent>(AbilitySystemComponent);
	// 技能槽已经有的技能，直接不让绑定
	if(RlAbilitySystemComponent->HasAbility(AbilityTag))
	{
		return false;
	}
	
	FRLAbilityInfo& AbilityInfo = AbilityConfig->FindAbilityInfo(AbilityTag);
	AbilityInfo.AbilityLevel = AbilityLevel;
	OnStartDrag.Broadcast(AbilityInfo.AbilityImage);
	SourceInputTag = FRLGameplayTags::Get().InputTag_Empty;
	SourceAbilityTag = AbilityTag;
	bDraggingAbility = true;
	
	// 防止捡起立马丢掉
	bJustPickUp = true;
	PickUpTimerHandle.Invalidate();
	FTimerDelegate Delegate = FTimerDelegate::CreateLambda( [this](){ bJustPickUp = false; });
	GetWorld()->GetTimerManager().SetTimer(PickUpTimerHandle, Delegate, 0.1f, false);
	
	return true;
}

void UAbilityMenuWidgetController::AddAbilityLevel(const FGameplayTag& AbilityTag)
{
	URLAbilitySystemComponent* RlAbilitySystemComponent = Cast<URLAbilitySystemComponent>(AbilitySystemComponent);
	RlAbilitySystemComponent->AddAbilityLevel(AbilityTag);
}

void UAbilityMenuWidgetController::OnMouseClick()
{
	if(bDraggingAbility)
	{
		DelayReleaseDragTimerHandle.Invalidate();
		FTimerDelegate Delegate = FTimerDelegate::CreateUObject(this, &UAbilityMenuWidgetController::DiscardAbility);
		GetWorld()->GetTimerManager().SetTimer(DelayReleaseDragTimerHandle, Delegate, 0.025f, false);
	}
}

void UAbilityMenuWidgetController::ShowAbilityUpgradePanel(bool bShow)
{
	OnShowAbilityUpgradePanel.Broadcast(bShow);
	bUpgradeMode = bShow;

	if(bShow)
	{
		ARLCharacter* RLCharacter = Cast<ARLCharacter>(AbilitySystemComponent->GetAvatarActor());
		ARLPlayerState* RLPS = Cast<ARLPlayerState>(PlayerState);
		const int CurMoney = RLPS->GetMoney();
		for(auto& AbilitySpec : AbilitySystemComponent->GetActivatableAbilities())
		{
			FRLAbilityInfo AbilityInfo = AbilityConfig->FindAbilityInfo(AbilitySpec.Ability->AbilityTags.First());
			AbilityInfo.AbilityLevel = AbilitySpec.Level;
			const int NeedMoney = RLCharacter->AbilityLevelCost.GetValueAtLevel(AbilitySpec.Level);
				
			if(AbilitySpec.Level <= 4 && CurMoney >= NeedMoney)
			{
				OnUpdateAbilityUpgradeInfo.Broadcast(AbilitySpec.DynamicAbilityTags.First(), NeedMoney,true);
			}
			else
			{
				OnUpdateAbilityUpgradeInfo.Broadcast(AbilitySpec.DynamicAbilityTags.First(), NeedMoney, false);
			}
		}
	}
}

void UAbilityMenuWidgetController::DiscardAbility()
{
	if(bJustPickUp)
		return;
	
	FRLAbilityInfo& EmptyAbilityInfo = AbilityConfig->FindAbilityInfo(FRLGameplayTags::Get().HeroAbility_Empty);
	OnStopDrag.Broadcast(EmptyAbilityInfo.AbilityImage);
	
	URLAbilitySystemComponent* RlAbilitySystemComponent = Cast<URLAbilitySystemComponent>(AbilitySystemComponent);
	float SourceAbilityLevel = RlAbilitySystemComponent->GetAbilityLevel(SourceAbilityTag);
	
	RlAbilitySystemComponent->RemoveCharacterAblity(SourceAbilityTag);
	
	OnUpdateCoolDown.Broadcast(SourceInputTag, 0);
	
	// Spawn Ability Actor
	Cast<ARLGameMode>(UGameplayStatics::GetGameMode(this))->SpawnAblityActorAroundPlayer(SourceAbilityTag, SourceAbilityLevel);
	
	SourceAbilityTag = FRLGameplayTags::Get().HeroAbility_Empty;
	SourceInputTag = FRLGameplayTags::Get().InputTag_Empty;
	bDraggingAbility = false;
}

