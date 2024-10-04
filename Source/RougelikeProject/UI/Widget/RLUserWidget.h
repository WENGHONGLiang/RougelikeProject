// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RLUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class ROUGELIKEPROJECT_API URLUserWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	void SetWidgetController(UObject* InWidgetController);
	
	// Widget 关联到一个 Controller
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UObject> WidgetController;

protected:
	// 蓝图事件。在 Controller 被设置时调用。
	UFUNCTION(BlueprintImplementableEvent)
	void OnWidgetControllerSet();
};

