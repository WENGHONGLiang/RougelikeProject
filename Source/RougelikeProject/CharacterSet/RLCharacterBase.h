// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "AttributeSet.h"
#include "GameplayEffect.h"
#include "GameFramework/Character.h"
#include "RougelikeProject/Data/CharacterInfo.h"
#include "RLCharacterBase.generated.h"

UCLASS()
class ARLCharacterBase : public ACharacter, public IAbilitySystemInterface {
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ARLCharacterBase();
	
	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;
	
	// Implement IAbilitySystemInterface
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UAttributeSet* GetAttributeSet() const { return AttributeSet; }	
	int32 GetCharacterLevel();

	UFUNCTION(BlueprintCallable)
	bool GetCharacterAlive() const { return isAlive; }
	
	// 对自身使用 GE
	UFUNCTION(BlueprintCallable)
	void ApplyEffectToSelf(TSubclassOf<UGameplayEffect> GameplayEffectClass, int32 Level);

protected:
	UPROPERTY()
	UAbilitySystemComponent* AbilitySystemComponent;
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Base Attributes")
	TSubclassOf<UGameplayEffect> DefaultAttributes;
	
	virtual void InitAbilityActorInfo();

	virtual void BeginPlay() override;
	
	// 添加初始角色技能 // 在 ASC->ActorInfo设置后调用
	void AddCharacterAbilities();
	
	// 角色初始技能
	UPROPERTY(EditAnywhere, Category="Abilities")
	TArray<TSubclassOf<UGameplayAbility>> StartupAbilities;
	
	// 用于扩展编写后续受到各种技能的效果
	UFUNCTION()
	virtual void GameplayEffectApplied(const FGameplayTagContainer& TagContainer);

	UPROPERTY(BlueprintReadOnly)
	bool isAlive;

	UFUNCTION()
	virtual void CharacterDie();
	
	UFUNCTION(BlueprintImplementableEvent)
	void OnBeAttacked();

	UFUNCTION(BlueprintImplementableEvent)
	void OnBeNormalAttacked();
	
	UFUNCTION(BlueprintImplementableEvent)
	void OnDie();
	
	UPROPERTY(EditAnywhere, Category="CharacterType")
	TObjectPtr<UCharacterClassInfo> CharacterInfo;
	
	UPROPERTY(EditAnywhere, Category="CharacterType")
	ECharacterType CharacterType;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Anim")
	UAnimMontage* AttackAnim;
};
