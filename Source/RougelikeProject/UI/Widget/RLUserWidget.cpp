// Fill out your copyright notice in the Description page of Project Settings.


#include "RLUserWidget.h"

void URLUserWidget::SetWidgetController(UObject* InWidgetController)
{
	WidgetController = InWidgetController;
	OnWidgetControllerSet();
}