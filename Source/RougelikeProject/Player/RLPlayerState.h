// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "AttributeSet.h"
#include "GameFramework/PlayerState.h"
#include "RLPlayerState.generated.h"

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

	// 只有第一关需要对属性、技能等初始化
	void Init() { AlreadyInit = true; };
	bool HasInit() const { return AlreadyInit; }
	
protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
	
	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;
	
	UPROPERTY(VisibleAnywhere, Category="Character Attribute")
	int32 Level = 1;

	bool AlreadyInit = false;
};
