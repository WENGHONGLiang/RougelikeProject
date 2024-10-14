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

ULevelMapWidgetController* ARLHUD::GetLevelMapWidgetController(const FWidgetControllerParams WCParams)
{
	if(LevelMapWidgetController == nullptr)
	{
		// 新建
		LevelMapWidgetController = NewObject<ULevelMapWidgetController>(this, LevelMapWidgetControllerClass);
		// 初始化
		LevelMapWidgetController->SetWidgetControllerParams(WCParams);
		LevelMapWidgetController->BindCallbacksToDependencies();
	}
	return LevelMapWidgetController;
}

void ARLHUD::ShowLevelMap(bool bShow)
{
	if(LevelMapWidgetController)
		LevelMapWidgetController->ShowWidget(bShow);
}

void ARLHUD::ShowOverlay(bool bShow)
{
	if(OverlayWidgetController)
		OverlayWidgetController->ShowWidget(bShow);
}

void ARLHUD::UpdateLevel(int CurNodeIndex, int LastNodeIndex)
{
	LevelMapWidgetController->UpdateLevel(CurNodeIndex, LastNodeIndex);
}

void ARLHUD::ShowAttributeMenu(bool bShow)
{
	if(AttributeMenuWidgetController)
		AttributeMenuWidgetController->ShowWidget(bShow);
}

void ARLHUD::InitOverlay(APlayerController* PC, UAbilitySystemComponent* ASC, UAttributeSet* AS, APlayerState* PS)
{
	if(!ensureAlways(OverlayWidgetClass) || !ensureAlways(OverlayWidgetControllerClass)) return;
	
	UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), OverlayWidgetClass);

	OverlayWidget = Cast<URLUserWidget>(Widget);

	FWidgetControllerParams WidgetControllerParams(PC, ASC, AS, PS);
	
	UOverlayWidgetController* MyOverlayWidgetController = GetOverlayWidgetController(WidgetControllerParams);

	// Call WidgetControllerSet Event in BP // And Set Controller for sub widgets.
	OverlayWidget->SetWidgetController(MyOverlayWidgetController);

	// 初始化
	OverlayWidgetController->BroadcastInitialValues();
	
	Widget->AddToViewport();
}

void ARLHUD::InitLevelMap(APlayerController* PC, UAbilitySystemComponent* ASC, UAttributeSet* AS, APlayerState* PS)
{
	if(!ensureAlways(LevelMapWidgetClass) || !ensureAlways(LevelMapWidgetControllerClass)) return;

	UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), LevelMapWidgetClass);

	LevelMapWidget = Cast<ULevelMapWidget>(Widget);

	FWidgetControllerParams WidgetControllerParams(PC, ASC, AS, PS);
	
	ULevelMapWidgetController* MyLevelMapWidgetController = GetLevelMapWidgetController(WidgetControllerParams);

	LevelMapWidget->SetWidgetController(MyLevelMapWidgetController);

	// 初始化
	MyLevelMapWidgetController->BroadcastInitialValues();
	MyLevelMapWidgetController->GenerateLevelNode();
	MyLevelMapWidgetController->GenerateLevelLine();
	
	Widget->AddToViewport();
}
