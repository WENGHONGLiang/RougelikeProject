// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "AttributeSet.h"
#include "GameplayEffect.h"
#include "ScalableFloat.h"
#include "GameFramework/PlayerState.h"
#include "RLPlayerState.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnValueChangedSignature, int32);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnExpChangedSignature, int32, int32);

// 角色的ASC放在PlayerState里 // 使 Pawn 可以被销毁重建，而 ASC 保持不动
UCLASS()
class ROUGELIKEPROJECT_API ARLPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	ARLPlayerState();
	
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UAttributeSet* GetAttributeSet() const { return AttributeSet; }

	int32 GetPlayerLevel() const { return PlayerLevel; }
	int32 GetMoney() const { return Money; }
	int32 GetSkillCoin() const { return SkillCoin; }
	int32 GetExp() const { return Exp; }
	int32 GetUpgradeNeedExp();

	void AddLevel();
	
	void AddMoney(int32 Amount);
	bool CostMoney(int32 Amount);
	
	void AddSkillCoin(int32 Amount);
	bool CostSkillCoin(int32 Amount);

	void AddExp(int32 Amount);

	// 只有第一关需要对属性、技能等初始化
	void Init() { AlreadyInit = true; }
	bool HasInit() const { return AlreadyInit; }
	
	FOnValueChangedSignature OnMoneyChanged;
	FOnValueChangedSignature OnSkillCoinChanged;
	FOnValueChangedSignature OnPlayerLevelChanged;
	FOnExpChangedSignature OnExpChanged;
	
protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
	
	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;
	
	UPROPERTY(VisibleAnywhere, Category="Character Attribute")
	int32 PlayerLevel;
	
	UPROPERTY(VisibleAnywhere, Category="Character Attribute")
	int32 Money;
	
	UPROPERTY(VisibleAnywhere, Category="Character Attribute")
	int32 SkillCoin;
	
	UPROPERTY(VisibleAnywhere, Category="Character Attribute")
	int32 Exp;

	bool AlreadyInit = false; // 标记ASC的初始化，全局只需要一次 // 因为Character要反复初始化，防止重复ASC

	UPROPERTY(EditDefaultsOnly)
	int32 PlayerUpgradeNeededBaseExp;
	
	UPROPERTY(EditDefaultsOnly)
	float PlayerUpgradeNeededExpMag;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayEffect> GE_PlayerUpgradeAttributeClass;
};
