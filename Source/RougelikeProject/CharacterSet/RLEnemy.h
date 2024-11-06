// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RLCharacterBase.h"
#include "Components/WidgetComponent.h"
#include "RougelikeProject/UI/WidgetController/OverlayWidgetController.h"
#include "RLEnemy.generated.h"





UCLASS()
class ROUGELIKEPROJECT_API ARLEnemy : public ARLCharacterBase
{
	GENERATED_BODY()
public:
	ARLEnemy();
	
	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnHealthChanged;

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnMaxHealthChanged;

	int32 RewardMoney;
	
protected:
	virtual void BeginPlay() override;
	virtual void InitAbilityActorInfo() override;

	void CharacterDie() override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UWidgetComponent> HealthBar;
	
	virtual void GameplayEffectApplied(const FGameplayTagContainer& TagContainer) override;

	UFUNCTION(BlueprintCallable)
	void UseAbility(const FGameplayTag& InputTag);
};
