// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "RougelikeProject/UI/Widget/LevelMapWidget.h"
#include "RougelikeProject/UI/WidgetController/AttributeMenuWidgetController.h"
#include "RougelikeProject/UI/WidgetController/LevelMapWidgetController.h"
#include "RougelikeProject/UI/WidgetController/OverlayWidgetController.h"
#include "RougelikeProject/UI/WidgetController/RLWidgetController.h"
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

	// Controller 中调用 // 保证一开始就有
	UFUNCTION(BlueprintCallable)
	void InitLevelMap(APlayerController* PC, UAbilitySystemComponent* ASC, UAttributeSet* AS, APlayerState* PS);
	
	// 初始化 或 得到 WC
	UOverlayWidgetController* GetOverlayWidgetController(const FWidgetControllerParams WCParams);
	UAttributeMenuWidgetController* GetAttributeMenuWidgetController(const FWidgetControllerParams WCParams);
	ULevelMapWidgetController* GetLevelMapWidgetController(const FWidgetControllerParams WCParams);

	UFUNCTION(BlueprintCallable)
	void ShowLevelMap(bool bShow);
	
	UFUNCTION(BlueprintCallable)
	void ShowOverlay(bool bShow);

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

	
	/* AttributeMenu：局内属性、技能展示界面 */
	UPROPERTY()
	TObjectPtr<URLUserWidget> AttributeMenuWidget;
	
	UPROPERTY()
	TObjectPtr<UAttributeMenuWidgetController> AttributeMenuWidgetController;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> AttributeMenuWidgetClass;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<UAttributeMenuWidgetController> AttributeMenuWidgetControllerClass;
	
	/* LevelMap：局外关卡选择界面 */
	UPROPERTY()
	TObjectPtr<ULevelMapWidget> LevelMapWidget;
	
	UPROPERTY()
	TObjectPtr<ULevelMapWidgetController> LevelMapWidgetController;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<ULevelMapWidget> LevelMapWidgetClass;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<ULevelMapWidgetController> LevelMapWidgetControllerClass;
};
