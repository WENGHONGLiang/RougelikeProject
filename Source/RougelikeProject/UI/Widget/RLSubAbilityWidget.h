// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "RLUserWidget.h"
#include "Components/Button.h"
#include "RougelikeProject/Data/AbilityConfig.h"
#include "RougelikeProject/Data/EquipmentConfig.h"
#include "RLSubAbilityWidget.generated.h"

UCLASS()
class ROUGELIKEPROJECT_API URLSubAbilityWidget : public URLUserWidget
{
	GENERATED_BODY()
public:
	URLSubAbilityWidget();

	void NativeConstruct() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag InputTag;
	
	UPROPERTY(meta = (BindWidget), BlueprintReadOnly, VisibleAnywhere)
	UButton* AbilityButton;
	
	UPROPERTY(BlueprintReadOnly)
	FRLAbilityInfo AbilityInfo;
	
	UPROPERTY(meta = (BindWidget), BlueprintReadOnly, VisibleAnywhere)
	UButton* EquipmentButton_1;
	
	UPROPERTY(meta = (BindWidget), BlueprintReadOnly, VisibleAnywhere)
	UButton* EquipmentButton_2;
	
	UPROPERTY(meta = (BindWidget), BlueprintReadOnly, VisibleAnywhere)
	UButton* EquipmentButton_3;
	
	UPROPERTY(BlueprintReadOnly)
	TArray<FRLEquipmentInfo> EquipmentInfos;
	
	UPROPERTY(BlueprintReadOnly)
	bool HasAbility = false;

	UPROPERTY(BlueprintReadOnly)
	TArray<bool> HasEquipments;

	// 蓝图中收到更新事件时，存储更新的技能Info
	UFUNCTION(BlueprintCallable)
	void StoreAbilityInfo(FRLAbilityInfo info);
	
	UFUNCTION(BlueprintCallable)
	void StoreEquipmentInfo(FRLEquipmentInfo info, int Index);

	UFUNCTION(BlueprintCallable)
	void AddAblityLevel();
	
	UFUNCTION(BlueprintCallable)
	void AddEquipmentLevel(int Index);

	UFUNCTION()
	void OnEquipmentButtonClick_1();
	UFUNCTION()
	void OnEquipmentButtonClick_2();
	UFUNCTION()
	void OnEquipmentButtonClick_3();
};
