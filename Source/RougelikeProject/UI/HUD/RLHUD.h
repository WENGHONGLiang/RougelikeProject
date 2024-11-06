// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "RougelikeProject/UI/Widget/LevelMapWidget.h"
#include "RougelikeProject/UI/Widget/RLAbilityMenuWidget.h"
#include "RougelikeProject/UI/WidgetController/AbilityMenuWidgetController.h"
#include "RougelikeProject/UI/WidgetController/AttributeMenuWidgetController.h"
#include "RougelikeProject/UI/WidgetController/LevelMapWidgetController.h"
#include "RougelikeProject/UI/WidgetController/OverlayWidgetController.h"
#include "RougelikeProject/UI/WidgetController/RLWidgetController.h"
#include "RougelikeProject/UI/WidgetController/ShopWidgetController.h"
#include "RLHUD.generated.h"

class UAbilitySystemComponent;
class UAttributeSet;
class URLUserWidget;
/**
 * 
 */
UCLASS()
class ROUGELIKEPROJECT_API ARLHUD : public AHUD
{
	GENERATED_BODY()
public:
	// Character 中调用 // 开始一局时才有
	UFUNCTION(BlueprintCallable)
	void InitOverlay(APlayerController* PC, UAbilitySystemComponent* ASC, UAttributeSet* AS, APlayerState* PS);
	
	UFUNCTION(BlueprintCallable)
	void InitAbilityMenu(APlayerController* PC, UAbilitySystemComponent* ASC, UAttributeSet* AS, APlayerState* PS);

	// Controller 中调用 // 保证一开始就有
	UFUNCTION(BlueprintCallable)
	void InitLevelMap(APlayerController* PC, UAbilitySystemComponent* ASC, UAttributeSet* AS, APlayerState* PS);
	
	// Controller 中调用 // 调用时只初始化Controller绑定事件 // Shop Actor初始化时更新实际商品
	UFUNCTION(BlueprintCallable)
	void InitShop(APlayerController* PC, UAbilitySystemComponent* ASC, UAttributeSet* AS, APlayerState* PS);
	
	// 初始化 或 得到 WC
	UOverlayWidgetController* GetOverlayWidgetController(const FWidgetControllerParams WCParams);
	UAttributeMenuWidgetController* GetAttributeMenuWidgetController(const FWidgetControllerParams WCParams);
	ULevelMapWidgetController* GetLevelMapWidgetController(const FWidgetControllerParams WCParams);
	UAbilityMenuWidgetController* GetAbilityMenuWidgetController(const FWidgetControllerParams WCParams);
	UShopWidgetController* GetShopWidgetController(const FWidgetControllerParams WCParams);

	UFUNCTION(BlueprintCallable)
	void ShowLevelMap(bool bShow);
	
	UFUNCTION(BlueprintCallable)
	void ShowBattleWidget(bool bShow);
	
	UFUNCTION(BlueprintCallable)
	void ShowShopWidget(bool bShow);

	UFUNCTION(BlueprintCallable)
	void UpdateShopCommodity(bool bShow = false);

	void UpdateLevel(int CurNodeIndex, int LastNodeIndex);
	
	UFUNCTION(BlueprintCallable)
	void ShowAttributeMenu(bool bShow);

private:
	/* Overlay：局内主界面 */
	UPROPERTY()
	TObjectPtr<URLUserWidget> OverlayWidget;
	
	UPROPERTY()
	TObjectPtr<UOverlayWidgetController> OverlayWidgetController;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> OverlayWidgetClass;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<UOverlayWidgetController> OverlayWidgetControllerClass;

	
	/* AttributeMenu：局内属性展示界面 */
	UPROPERTY()
	TObjectPtr<URLUserWidget> AttributeMenuWidget;
	
	UPROPERTY()
	TObjectPtr<UAttributeMenuWidgetController> AttributeMenuWidgetController;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> AttributeMenuWidgetClass;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<UAttributeMenuWidgetController> AttributeMenuWidgetControllerClass;

	
	/* AbilityMenu：局内技能界面 */
	UPROPERTY()
	TObjectPtr<URLAbilityMenuWidget> AbilityMenuWidget;
	
	UPROPERTY()
	TObjectPtr<UAbilityMenuWidgetController> AbilityMenuWidgetController;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<URLAbilityMenuWidget> AbilityMenuWidgetClass;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<UAbilityMenuWidgetController> AbilityMenuWidgetControllerClass;
	
	
	/* LevelMap：局外关卡选择界面 */
	UPROPERTY()
	TObjectPtr<ULevelMapWidget> LevelMapWidget;
	
	UPROPERTY()
	TObjectPtr<ULevelMapWidgetController> LevelMapWidgetController;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<ULevelMapWidget> LevelMapWidgetClass;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<ULevelMapWidgetController> LevelMapWidgetControllerClass;

	
	/* LevelMap：局外关卡选择界面 */
	UPROPERTY()
	TObjectPtr<URLUserWidget> ShopWidget;
	
	UPROPERTY()
	TObjectPtr<UShopWidgetController> ShopWidgetController;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<URLUserWidget> ShopWidgetClass;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<UShopWidgetController> ShopWidgetControllerClass;
	
};
