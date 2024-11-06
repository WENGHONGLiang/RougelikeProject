// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "AttributeSet.h"
#include "GameFramework/PlayerState.h"
#include "RLPlayerState.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnValueChangedSignature, int32);

// 角色的ASC放在PlayerState里 // 使 Pawn 可以被销毁重建，而 ASC 保持不动
UCLASS()
class ROUGELIKEPROJECT_API ARLPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	ARLPlayerState();
	
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UAttributeSet* GetAttributeSet() const { return AttributeSet; }

	int32 GetPlayerLevel() const { return Level; }
	int32 GetMoney() const { return Money; }

	void AddMoney(int32 Amount);
	bool CostMoney(int32 Amount );

	// 只有第一关需要对属性、技能等初始化
	void Init() { AlreadyInit = true; }
	bool HasInit() const { return AlreadyInit; }
	
	FOnValueChangedSignature OnMoneyChanged;
	
protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
	
	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;
	
	UPROPERTY(VisibleAnywhere, Category="Character Attribute")
	int32 Level = 1;
	
	UPROPERTY(VisibleAnywhere, Category="Character Attribute")
	int32 Money = 8;

	bool AlreadyInit = false; // 标记ASC的初始化，全局只需要一次 // 因为Character要反复初始化，防止重复ASC
};
