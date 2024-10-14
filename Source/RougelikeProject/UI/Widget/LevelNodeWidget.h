// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RLUserWidget.h"
#include "../WidgetController/LevelMapWidgetController.h"
#include "Components/Button.h"
#include "LevelNodeWidget.generated.h"

class UWidgetSwitcher;
class USizeBox;
/**
 * 
 */
UCLASS()
class ROUGELIKEPROJECT_API ULevelNodeWidget : public URLUserWidget
{
	GENERATED_BODY()

	int NodeIndex;
	FName LevelName;
	
public:

	void NativeConstruct() override;
	
	UPROPERTY(meta = (BindWidget))
	UWidgetSwitcher* BattleNodeSwitcher;
	
	UPROPERTY(meta = (BindWidget))
	UWidgetSwitcher* ShopNodeSwitcher;
	
	UPROPERTY(meta = (BindWidget))
	UWidgetSwitcher* StartNodeSwitcher;
	
	UPROPERTY(meta = (BindWidget))
	UWidgetSwitcher* EndNodeSwitcher;

	// 不可行节点
	UPROPERTY(meta = (BindWidget))
	UButton* btn_Lock_1;
	UPROPERTY(meta = (BindWidget))
	UButton* btn_Lock_2;
	UPROPERTY(meta = (BindWidget))
	UButton* btn_Lock_3;
	UPROPERTY(meta = (BindWidget))
	UButton* btn_Lock_4;

	// 未探索
	UPROPERTY(meta = (BindWidget))
	UButton* btn_Inc_1;
	UPROPERTY(meta = (BindWidget))
	UButton* btn_Inc_2;
	UPROPERTY(meta = (BindWidget))
	UButton* btn_Inc_3;
	UPROPERTY(meta = (BindWidget))
	UButton* btn_Inc_4;

	// 已探索（可进入）
	UPROPERTY(meta = (BindWidget))
	UButton* btn_com_1;
	UPROPERTY(meta = (BindWidget))
	UButton* btn_com_2;
	UPROPERTY(meta = (BindWidget))
	UButton* btn_com_3;
	UPROPERTY(meta = (BindWidget))
	UButton* btn_com_4;

	
	UPROPERTY(meta = (BindWidget))
	UButton* btn_comForbid_1;
	UPROPERTY(meta = (BindWidget))
	UButton* btn_comForbid_2;
	UPROPERTY(meta = (BindWidget))
	UButton* btn_comForbid_3;
	UPROPERTY(meta = (BindWidget))
	UButton* btn_comForbid_4;
	
	void SetNode(FNode node);

	UFUNCTION()
	void OnButtonClick();
};
