// Fill out your copyright notice in the Description page of Project Settings.


#include "RLWidgetController.h"

void URLWidgetController::SetWidgetControllerParams(const FWidgetControllerParams& WCParams)
{
	PlayerController = WCParams.PlayerController;
	AbilitySystemComponent = WCParams.AbilitySystemComponent;
	AttributeSet = WCParams.AttributeSet;
}

void URLWidgetController::BroadcastInitialValues()
{
	
}

void URLWidgetController::BindCallbacksToDependencies()
{
	
}