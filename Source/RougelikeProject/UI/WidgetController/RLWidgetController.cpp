// Fill out your copyright notice in the Description page of Project Settings.


#include "RLWidgetController.h"

void URLWidgetController::SetWidgetControllerParams(const FWidgetControllerParams& WCParams)
{
	PlayerController = WCParams.PlayerController;
	AbilitySystemComponent = WCParams.AbilitySystemComponent;
	AttributeSet = WCParams.AttributeSet;
	PlayerState = WCParams.PlayerState;
}

void URLWidgetController::BroadcastInitialValues()
{
	
}

void URLWidgetController::BindCallbacksToDependencies()
{
	
}

void URLWidgetController::ShowWidget(bool bShow)
{
	OnShow.Broadcast(bShow);
}

FVector2D URLWidgetController::GetCursorPosition() const
{
	double X, Y;
	PlayerController->GetMousePosition(X, Y);
	return FVector2D(X, Y);
}
