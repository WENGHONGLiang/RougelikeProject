// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "RougelikeProject/UI/WidgetController/AttributeMenuWidgetController.h"
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

	UFUNCTION(BlueprintCallable)
	void InitOverlay(APlayerController* PC, UAbilitySystemComponent* ASC, UAttributeSet* AS);
	
	// 初始化 或 得到 WC
	UOverlayWidgetController* GetOverlayWidgetController(const FWidgetControllerParams WCParams);
	UAttributeMenuWidgetController* GetAttributeMenuWidgetController(const FWidgetControllerParams WCParams);
protected:
	
private:
	UPROPERTY()
	TObjectPtr<URLUserWidget> OverlayWidget;
	
	UPROPERTY()
	TObjectPtr<UOverlayWidgetController> OverlayWidgetController;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> OverlayWidgetClass;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<UOverlayWidgetController> OverlayWidgetControllerClass;

	
	UPROPERTY()
	TObjectPtr<URLUserWidget> AttributeMenuWidget;
	
	UPROPERTY()
	TObjectPtr<UAttributeMenuWidgetController> AttributeMenuWidgetController;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> AttributeMenuWidgetClass;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<UAttributeMenuWidgetController> AttributeMenuWidgetControllerClass;
};
