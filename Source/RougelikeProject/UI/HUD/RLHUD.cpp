// Fill out your copyright notice in the Description page of Project Settings.


#include "RLHUD.h"

#include "Blueprint/UserWidget.h"
#include "RougelikeProject/UI/Widget/RLUserWidget.h"


UOverlayWidgetController* ARLHUD::GetOverlayWidgetController(const FWidgetControllerParams WCParams)
{
	if(OverlayWidgetController == nullptr)
	{
		// 新建
		OverlayWidgetController = NewObject<UOverlayWidgetController>(this, OverlayWidgetControllerClass);
		// 初始化
		OverlayWidgetController->SetWidgetControllerParams(WCParams);
		OverlayWidgetController->BindCallbacksToDependencies();
	}
	return OverlayWidgetController;
}

UAttributeMenuWidgetController* ARLHUD::GetAttributeMenuWidgetController(const FWidgetControllerParams WCParams)
{
	if(AttributeMenuWidgetController == nullptr)
	{
		// 新建
		AttributeMenuWidgetController = NewObject<UAttributeMenuWidgetController>(this, AttributeMenuWidgetControllerClass);
		// 初始化
		AttributeMenuWidgetController->SetWidgetControllerParams(WCParams);
		AttributeMenuWidgetController->BindCallbacksToDependencies();
	}
	return AttributeMenuWidgetController;
}

void ARLHUD::InitOverlay(APlayerController* PC, UAbilitySystemComponent* ASC, UAttributeSet* AS)
{
	if(!ensureAlways(OverlayWidgetClass) || !ensureAlways(OverlayWidgetControllerClass)) return;
	
	UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), OverlayWidgetClass);

	OverlayWidget = Cast<URLUserWidget>(Widget);

	FWidgetControllerParams WidgetControllerParams(PC, ASC, AS);
	
	UOverlayWidgetController* MyOverlayWidgetController = GetOverlayWidgetController(WidgetControllerParams);

	// Call WidgetControllerSet Event in BP // And Set Controller for sub widgets.
	OverlayWidget->SetWidgetController(MyOverlayWidgetController);

	// 初始化
	OverlayWidgetController->BroadcastInitialValues();
	
	Widget->AddToViewport();
}