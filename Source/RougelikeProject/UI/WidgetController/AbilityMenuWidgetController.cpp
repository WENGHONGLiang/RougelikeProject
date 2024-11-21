// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityMenuWidgetController.h"

#include "AbilitySystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/RepLayout.h"
#include "RougelikeProject/AbilitySystem/RLAbilitySystemComponent.h"
#include "RougelikeProject/AbilitySystem/RLAbilitySystemLibrary.h"
#include "RougelikeProject/AbilitySystem/RLGameplayAbility.h"
#include "RougelikeProject/AbilitySystem/RLGameplayTags.h"
#include "RougelikeProject/Data/EquipmentConfig.h"
#include "RougelikeProject/GameMode/RLGameMode.h"
#include "RougelikeProject/Player/RLPlayerController.h"
#include "RougelikeProject/Player/RLPlayerState.h"
#include "UObject/FastReferenceCollector.h"

UAbilityMenuWidgetController::UAbilityMenuWidgetController()
{
	bDraggingAbility = false;
	bDraggingEquipment = false;
	SourceAbilityTag = FRLGameplayTags::Get().HeroAbility_Empty;
	SourceEquipmentTag = FRLGameplayTags::Get().Equipment_Empty;
	SourceInputTag = FRLGameplayTags::Get().InputTag_Empty;
}

void UAbilityMenuWidgetController::BindCallbacksToDependencies()
{
	URLAbilitySystemComponent* RLASC = Cast<URLAbilitySystemComponent>(AbilitySystemComponent);
	ARLPlayerState* RLPS = Cast<ARLPlayerState>(PlayerState);

	// 新技能添加事件
	//RLASC->OnGameplayAbilityGive.AddLambda(
	//	[this](const FGameplayTag InputTag, const FGameplayTag AbilityTag, float AbilityLevel)
	//	{
	//		// 用 技能标签 找到技能信息，再跟 键位标签 一起传给 Widget 使用
	//		FRLAbilityInfo AbilityInfo = AbilityConfig->FindAbilityInfo(AbilityTag);
	//		AbilityInfo.AbilityLevel = AbilityLevel;
	//		OnUpdateAbility.Broadcast(InputTag, AbilityInfo);
	//	});

	RLASC->OnAbilityLevelUpdate.AddLambda(
		[this](const FGameplayTag InputTag, const FGameplayTag AbilityTag, float AbilityLevel)
		{
			// 用 技能标签 找到技能信息，再跟 键位标签 一起传给 Widget 使用
			FRLAbilityInfo AbilityInfo = AbilityConfig->FindAbilityInfo(AbilityTag);
			AbilityInfo.AbilityLevel = AbilityLevel;
			OnUpdateAbility.Broadcast(InputTag, AbilityInfo);

			if(bUpgradeMode)
			{
				ARLCharacter* RLCharacter = Cast<ARLCharacter>(AbilitySystemComponent->GetAvatarActor());
				ARLPlayerState* RLPS = Cast<ARLPlayerState>(PlayerState);
				AbilityInfo.AbilityLevel = AbilityLevel;
				int NeedSkillCoin = RLCharacter->AbilityLevelCost.GetValueAtLevel(AbilityLevel);
				
				if(AbilityLevel <= 4 && RLPS->GetSkillCoin() >= NeedSkillCoin)
				{
					OnUpdateAbilityUpgradeInfo.Broadcast(InputTag, NeedSkillCoin,true);
				}
				else
				{
					OnUpdateAbilityUpgradeInfo.Broadcast(InputTag, NeedSkillCoin, false);
				}
			}
		});
	
	RLASC->OnEquipmentLevelUpdate.AddLambda(
		[this](const FGameplayTag InputTag, const FGameplayTag EquipmentTag, float EquipmentLevel)
		{
			FRLEquipmentInfo EquipmentInfo = EquipmentConfig->FindEquipmentInfo(EquipmentTag);
			EquipmentInfo.EquipmentLevel = EquipmentLevel;
			OnUpdateEquipmentByEquipmentTag.Broadcast(EquipmentTag, EquipmentInfo);
			
			if(bUpgradeMode)
			{
				ARLCharacter* RLCharacter = Cast<ARLCharacter>(AbilitySystemComponent->GetAvatarActor());
				ARLPlayerState* RLPS = Cast<ARLPlayerState>(PlayerState);
				EquipmentInfo.EquipmentLevel = EquipmentLevel;
				int NeedSkillCoin = RLCharacter->AbilityLevelCost.GetValueAtLevel(EquipmentLevel);
				
				if(EquipmentLevel <= 4 && RLPS->GetSkillCoin() >= NeedSkillCoin)
				{
					OnUpdateEquipmentUpgradeInfo.Broadcast(EquipmentTag, NeedSkillCoin,true);
				}
				else
				{
					OnUpdateEquipmentUpgradeInfo.Broadcast(EquipmentTag, NeedSkillCoin,false);
				}
			}
		});

	// 冷却时间更新
	RLASC->OnCoolDownTimeUpdate.AddLambda(
		[this](const FGameplayTag InputTag, float Percent)
		{
			OnUpdateAbilityCoolDown.Broadcast(InputTag, Percent);
		});

	
	RLPS->OnSkillCoinChanged.AddLambda(
		[this](int32 NewSkillCoin)
		{
			if(!bUpgradeMode) return;
			
			ARLCharacter* RLCharacter = Cast<ARLCharacter>(AbilitySystemComponent->GetAvatarActor());
			for(auto& AbilitySpec : AbilitySystemComponent->GetActivatableAbilities())
			{
				//FRLAbilityInfo AbilityInfo = AbilityConfig->FindAbilityInfo(AbilitySpec.Ability->AbilityTags.First());
				//AbilityInfo.AbilityLevel = AbilitySpec.Level;
				int NeedSkillCoin = RLCharacter->AbilityLevelCost.GetValueAtLevel(AbilitySpec.Level);

				if(AbilitySpec.Ability->AbilityTags.First().MatchesTag(FRLGameplayTags::Get().Equipment))
				{
					if(AbilitySpec.Level <= 4 && NewSkillCoin >= NeedSkillCoin)
					{
						OnUpdateEquipmentUpgradeInfo.Broadcast(AbilitySpec.Ability->AbilityTags.First(), NeedSkillCoin,true);
					}
					else
					{
						OnUpdateEquipmentUpgradeInfo.Broadcast(AbilitySpec.Ability->AbilityTags.First(), NeedSkillCoin, false);
					}
				}
				else
				{
					if(AbilitySpec.Level <= 4 && NewSkillCoin >= NeedSkillCoin)
					{
						OnUpdateAbilityUpgradeInfo.Broadcast(AbilitySpec.DynamicAbilityTags.First(), NeedSkillCoin,true);
					}
					else
					{
						OnUpdateAbilityUpgradeInfo.Broadcast(AbilitySpec.DynamicAbilityTags.First(), NeedSkillCoin, false);
					}
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

	// 空装备
	FRLEquipmentInfo EmptyEquipmentInfo = EquipmentConfig->FindEquipmentInfo(GameplayTags.Equipment_Empty);
	OnUpdateEquipmentByInputSlot.Broadcast(GameplayTags.InputTag_1, 0, EmptyEquipmentInfo);
	OnUpdateEquipmentByInputSlot.Broadcast(GameplayTags.InputTag_1, 1, EmptyEquipmentInfo);
	OnUpdateEquipmentByInputSlot.Broadcast(GameplayTags.InputTag_1, 2, EmptyEquipmentInfo);
	OnUpdateEquipmentByInputSlot.Broadcast(GameplayTags.InputTag_2, 0, EmptyEquipmentInfo);
	OnUpdateEquipmentByInputSlot.Broadcast(GameplayTags.InputTag_2, 1, EmptyEquipmentInfo);
	OnUpdateEquipmentByInputSlot.Broadcast(GameplayTags.InputTag_2, 2, EmptyEquipmentInfo);
	OnUpdateEquipmentByInputSlot.Broadcast(GameplayTags.InputTag_3, 0, EmptyEquipmentInfo);
	OnUpdateEquipmentByInputSlot.Broadcast(GameplayTags.InputTag_3, 1, EmptyEquipmentInfo);
	OnUpdateEquipmentByInputSlot.Broadcast(GameplayTags.InputTag_3, 2, EmptyEquipmentInfo);
	OnUpdateEquipmentByInputSlot.Broadcast(GameplayTags.InputTag_4, 0, EmptyEquipmentInfo);
	OnUpdateEquipmentByInputSlot.Broadcast(GameplayTags.InputTag_4, 1, EmptyEquipmentInfo);
	OnUpdateEquipmentByInputSlot.Broadcast(GameplayTags.InputTag_4, 2, EmptyEquipmentInfo);

	// 角色已有技能 / 装备
	for(auto& AbilitySpec : AbilitySystemComponent->GetActivatableAbilities())
	{
		URLGameplayAbility* RLAbility = Cast<URLGameplayAbility>(AbilitySpec.Ability);
		if(AbilitySpec.Ability->AbilityTags.First().MatchesTag(FRLGameplayTags::Get().Equipment))
		{
			FRLEquipmentInfo EquipmentInfo = EquipmentConfig->FindEquipmentInfo(RLAbility->AbilityTags.First());
			EquipmentInfo.EquipmentLevel = AbilitySpec.Level;
			OnUpdateEquipmentByInputSlot.Broadcast(AbilitySpec.DynamicAbilityTags.First(), -1, EquipmentInfo); // -1 代表随便找个空给它填上
		}
		else
		{
			FRLAbilityInfo AbilityInfo = AbilityConfig->FindAbilityInfo(RLAbility->AbilityTags.First());
			AbilityInfo.AbilityLevel = AbilitySpec.Level;
			OnUpdateAbility.Broadcast(AbilitySpec.DynamicAbilityTags.First(), AbilityInfo);
		}
	}
}

void UAbilityMenuWidgetController::OnAbilityButtonClicked(const FGameplayTag& InputTag, const FGameplayTag& AbilityTag)
{
	if(bDraggingEquipment)
		return;
	
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
	else if(bDraggingAbility && !SourceInputTag.MatchesTagExact(FRLGameplayTags::Get().InputTag_Empty)) // 选中目标技能 // 手上的源技能绑定过 // 交换源技能和目标技能
	{
		URLAbilitySystemComponent* RlAbilitySystemComponent = Cast<URLAbilitySystemComponent>(AbilitySystemComponent);
		FRLAbilityInfo& SourceAbilityInfo = AbilityConfig->FindAbilityInfo(SourceAbilityTag);
		FRLAbilityInfo& TargetAbilityInfo = AbilityConfig->FindAbilityInfo(AbilityTag);

		SourceAbilityInfo.AbilityLevel = RlAbilitySystemComponent->GetAbilityLevel(SourceAbilityTag);
		TargetAbilityInfo.AbilityLevel = RlAbilitySystemComponent->GetAbilityLevel(AbilityTag);
		
		// 交换源技能和目标技能（UI）
		OnUpdateAbility.Broadcast(SourceInputTag, TargetAbilityInfo);
		OnUpdateAbility.Broadcast(InputTag, SourceAbilityInfo);
		
		// 实际技能绑定交换
		RlAbilitySystemComponent->UpdateCharacterAbilitiesInput(SourceAbilityTag, InputTag);
		RlAbilitySystemComponent->UpdateCharacterAbilitiesInput(TargetAbilityInfo.AbilityTag, SourceInputTag);

		// Cooldown的UI先清掉，如果实际有Cooldown还会更新
		OnUpdateAbilityCoolDown.Broadcast(SourceInputTag, 0);
		OnUpdateAbilityCoolDown.Broadcast(InputTag, 0);
		
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
		TargetAbilityInfo.AbilityLevel = RlAbilitySystemComponent->GetAbilityLevel(AbilityTag);
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

void UAbilityMenuWidgetController::OnEquipmemtButtonClicked(const FGameplayTag& InputTag, int Index, const FGameplayTag& EquipmentTag)
{
	if(bDraggingAbility)
		return;
	
	FRLEquipmentInfo& EmptyEquipmentInfo = EquipmentConfig->FindEquipmentInfo(FRLGameplayTags::Get().Equipment_Empty);
	
	if(!EquipmentTag.MatchesTagExact(FRLGameplayTags::Get().Equipment_Empty) && !bDraggingEquipment) // 选中源装备
	{
		SourceInputTag = InputTag;
		SourceEquipmentTag = EquipmentTag;
		SourcePropertyIndex = Index;
		bDraggingEquipment = true;

		FRLEquipmentInfo& SourceEquipmentInfo = EquipmentConfig->FindEquipmentInfo(SourceEquipmentTag);
		
		OnUpdateEquipmentByInputSlot.Broadcast(SourceInputTag, SourcePropertyIndex, EmptyEquipmentInfo);

		OnStartDrag.Broadcast(SourceEquipmentInfo.EquipmentImage);
	}
	else if(bDraggingEquipment && !SourceInputTag.MatchesTagExact(FRLGameplayTags::Get().InputTag_Empty)) // 选中目标装备 // 手上的源装备绑定过 // 交换
	{
		URLAbilitySystemComponent* RlAbilitySystemComponent = Cast<URLAbilitySystemComponent>(AbilitySystemComponent);
		FRLEquipmentInfo& SourceEquipmentInfo = EquipmentConfig->FindEquipmentInfo(SourceEquipmentTag);
		FRLEquipmentInfo& TargetEquipmentInfo = EquipmentConfig->FindEquipmentInfo(EquipmentTag);

		SourceEquipmentInfo.EquipmentLevel = RlAbilitySystemComponent->GetAbilityLevel(SourceEquipmentTag);
		TargetEquipmentInfo.EquipmentLevel = RlAbilitySystemComponent->GetAbilityLevel(EquipmentTag);
		
		// 交换源技能和目标技能（UI）
		OnUpdateEquipmentByInputSlot.Broadcast(SourceInputTag, SourcePropertyIndex, TargetEquipmentInfo);
		OnUpdateEquipmentByInputSlot.Broadcast(InputTag, Index, SourceEquipmentInfo);
		
		// 实际键位绑定交换
		RlAbilitySystemComponent->UpdateCharacterAbilitiesInput(SourceEquipmentTag, InputTag);
		RlAbilitySystemComponent->UpdateCharacterAbilitiesInput(TargetEquipmentInfo.EquipmentTag, SourceInputTag);

		// TODO: 装备的Cooldown暂时没加
		
		bDraggingEquipment = false;
		SourceEquipmentTag = FRLGameplayTags::Get().Equipment_Empty;
		SourceInputTag = FRLGameplayTags::Get().InputTag_Empty;
		
		OnStopDrag.Broadcast(EmptyEquipmentInfo.EquipmentImage);
	}
	else if(bDraggingEquipment && SourceInputTag.MatchesTagExact(FRLGameplayTags::Get().InputTag_Empty)) // 选中目标装备 // 手上的装备未绑定
	{
		URLAbilitySystemComponent* RlAbilitySystemComponent = Cast<URLAbilitySystemComponent>(AbilitySystemComponent);
		
		FRLEquipmentInfo& SourceEquipmentInfo = EquipmentConfig->FindEquipmentInfo(SourceEquipmentTag);
		FRLEquipmentInfo& TargetEquipmentInfo = EquipmentConfig->FindEquipmentInfo(EquipmentTag);
		TargetEquipmentInfo.EquipmentLevel = RlAbilitySystemComponent->GetAbilityLevel(EquipmentTag);
		FGameplayTag CurrentEquipmentTag = SourceEquipmentTag;
			
		// 更新手上技能
		if(EquipmentTag.MatchesTagExact(FRLGameplayTags::Get().Equipment_Empty))
		{
			bDraggingEquipment = false;
			OnStopDrag.Broadcast(TargetEquipmentInfo.EquipmentImage);
		}
		else
		{
			SourceEquipmentTag = EquipmentTag;
			SourcePropertyIndex = Index;
			OnStartDrag.Broadcast(TargetEquipmentInfo.EquipmentImage);
		}
		
		// 把当前槽位交给手上装备
		OnUpdateEquipmentByInputSlot.Broadcast(InputTag, Index, SourceEquipmentInfo);
		
		RlAbilitySystemComponent->UpdateCharacterAbilitiesInput(CurrentEquipmentTag, InputTag);
		RlAbilitySystemComponent->UpdateCharacterAbilitiesInput(TargetEquipmentInfo.EquipmentTag, SourceInputTag);
	}
}

EAddPropertyRes UAbilityMenuWidgetController::AddProperty(const FGameplayTag& PropertyTag, float PropertyLevel)
{
	if(bDraggingAbility || bDraggingEquipment)
	{
		return DontAdd;
	}

	URLAbilitySystemComponent* RlAbilitySystemComponent = Cast<URLAbilitySystemComponent>(AbilitySystemComponent);
	
	// 技能槽已经有的技能 / 装备，提升等级，不重新绑定
	if(RlAbilitySystemComponent->HasAbility(PropertyTag))
	{
		RlAbilitySystemComponent->AddAbilityLevel(PropertyTag, false, PropertyLevel);
		return AlreadyHasProperty;
	}
	
	SourceInputTag = FRLGameplayTags::Get().InputTag_Empty;

	if(PropertyTag.MatchesTag(FRLGameplayTags::Get().Equipment)) // 装备
	{
		SourceEquipmentTag = PropertyTag;
		bDraggingEquipment = true;
		
		FRLEquipmentInfo& EquipmentInfo = EquipmentConfig->FindEquipmentInfo(PropertyTag);
		EquipmentInfo.EquipmentLevel = PropertyLevel;
		OnStartDrag.Broadcast(EquipmentInfo.EquipmentImage);
		FTimerDelegate Delegate = FTimerDelegate::CreateLambda([this, EquipmentInfo]()
		{
			URLAbilitySystemLibrary::GetOverlayWidgetController(this)->SetPropertyMessage(EquipmentInfo.EquipmentName, EquipmentInfo.EquipmentQuality, EquipmentInfo.EquipmentDescription, EquipmentInfo.EquipmentLevel, 0, EquipmentInfo.EquipmentBaseCooldown);
		});
		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, Delegate, 0.001f, false);
	}
	else // 技能
	{
		SourceAbilityTag = PropertyTag;
		bDraggingAbility = true;

		FRLAbilityInfo& AbilityInfo = AbilityConfig->FindAbilityInfo(PropertyTag);
		AbilityInfo.AbilityLevel = PropertyLevel;
		OnStartDrag.Broadcast(AbilityInfo.AbilityImage);
		
		FTimerDelegate Delegate = FTimerDelegate::CreateLambda([this, AbilityInfo]()
		{
			URLAbilitySystemLibrary::GetOverlayWidgetController(this)->SetPropertyMessage(AbilityInfo.AbilityName, AbilityInfo.AbilityQuality, AbilityInfo.AbilityDescription, AbilityInfo.AbilityLevel, AbilityInfo.AbilityBaseDamage, AbilityInfo.AbilityBaseCooldown);
		});
		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, Delegate, 0.001f, false);
	}
	
	// 防止捡起立马丢掉
	bJustPickUp = true;
	PickUpTimerHandle.Invalidate();
	FTimerDelegate Delegate = FTimerDelegate::CreateLambda( [this](){ bJustPickUp = false; });
	GetWorld()->GetTimerManager().SetTimer(PickUpTimerHandle, Delegate, 0.1f, false);
	
	return NewlyAdded;
}

void UAbilityMenuWidgetController::AddPropertyLevelWithSkillCoin(FGameplayTag PropertyTag)
{
	URLAbilitySystemComponent* RlAbilitySystemComponent = Cast<URLAbilitySystemComponent>(AbilitySystemComponent);
	RlAbilitySystemComponent->AddAbilityLevel(PropertyTag, true, 1);
}

void UAbilityMenuWidgetController::OnMouseClick()
{
	if(bDraggingAbility || bDraggingEquipment)
	{
		DelayReleaseDragTimerHandle.Invalidate();
		FTimerDelegate Delegate = FTimerDelegate::CreateUObject(this, &UAbilityMenuWidgetController::DiscardProperty);
		GetWorld()->GetTimerManager().SetTimer(DelayReleaseDragTimerHandle, Delegate, 0.025f, false);
	}
}

void UAbilityMenuWidgetController::ShowAbilityUpgradePanel(bool bShow)
{
	OnShowUpgradePanel.Broadcast(bShow);
	bUpgradeMode = bShow;

	if(bShow)
	{
		ARLCharacter* RLCharacter = Cast<ARLCharacter>(AbilitySystemComponent->GetAvatarActor());
		ARLPlayerState* RLPS = Cast<ARLPlayerState>(PlayerState);
		const int CurSkillCoin = RLPS->GetSkillCoin();
		for(auto& AbilitySpec : AbilitySystemComponent->GetActivatableAbilities())
		{
			//FRLAbilityInfo AbilityInfo = AbilityConfig->FindAbilityInfo(AbilitySpec.Ability->AbilityTags.First());
			//AbilityInfo.AbilityLevel = AbilitySpec.Level;
			const int NeedSkillCoin = RLCharacter->AbilityLevelCost.GetValueAtLevel(AbilitySpec.Level);

			
			if(AbilitySpec.Ability->AbilityTags.First().MatchesTag(FRLGameplayTags::Get().Equipment))
			{
				if(AbilitySpec.Level <= 4 && CurSkillCoin >= NeedSkillCoin)
				{
					OnUpdateEquipmentUpgradeInfo.Broadcast(AbilitySpec.Ability->AbilityTags.First(), NeedSkillCoin,true);
				}
				else
				{
					OnUpdateEquipmentUpgradeInfo.Broadcast(AbilitySpec.Ability->AbilityTags.First(), NeedSkillCoin, false);
				}
			}
			else
			{
				if(AbilitySpec.Level <= 4 && CurSkillCoin >= NeedSkillCoin)
				{
					OnUpdateAbilityUpgradeInfo.Broadcast(AbilitySpec.DynamicAbilityTags.First(), NeedSkillCoin,true);
				}
				else
				{
					OnUpdateAbilityUpgradeInfo.Broadcast(AbilitySpec.DynamicAbilityTags.First(), NeedSkillCoin, false);
				}
			}
		}
	}
}

void UAbilityMenuWidgetController::DiscardProperty()
{
	if(bJustPickUp)
		return;
	
	URLAbilitySystemComponent* RlAbilitySystemComponent = Cast<URLAbilitySystemComponent>(AbilitySystemComponent);
	if(bDraggingAbility)
	{
		FRLAbilityInfo& EmptyAbilityInfo = AbilityConfig->FindAbilityInfo(FRLGameplayTags::Get().HeroAbility_Empty);
		OnStopDrag.Broadcast(EmptyAbilityInfo.AbilityImage);
	
		float SourceAbilityLevel = RlAbilitySystemComponent->GetAbilityLevel(SourceAbilityTag);
	
		RlAbilitySystemComponent->RemoveCharacterAblity(SourceAbilityTag);
	
		OnUpdateAbilityCoolDown.Broadcast(SourceInputTag, 0);
	
		// Spawn Ability Actor
		Cast<ARLGameMode>(UGameplayStatics::GetGameMode(this))->SpawnAblityActorAroundPlayer(SourceAbilityTag, SourceAbilityLevel);
	
		SourceAbilityTag = FRLGameplayTags::Get().HeroAbility_Empty;
		bDraggingAbility = false;
	}
	if(bDraggingEquipment)
	{
		FRLEquipmentInfo& EmptyEquipmentInfo = EquipmentConfig->FindEquipmentInfo(FRLGameplayTags::Get().Equipment_Empty);
		OnStopDrag.Broadcast(EmptyEquipmentInfo.EquipmentImage);
	
		float SourceEquipmentLevel = RlAbilitySystemComponent->GetAbilityLevel(SourceEquipmentTag);
	
		RlAbilitySystemComponent->RemoveCharacterAblity(SourceEquipmentTag);
	
		// Spawn Ability Actor
		Cast<ARLGameMode>(UGameplayStatics::GetGameMode(this))->SpawnEquipmentActorAroundPlayer(SourceEquipmentTag, SourceEquipmentLevel);
	
		SourceEquipmentTag = FRLGameplayTags::Get().Equipment_Empty;
		bDraggingEquipment = false;
	}
	SourceInputTag = FRLGameplayTags::Get().InputTag_Empty;
}

