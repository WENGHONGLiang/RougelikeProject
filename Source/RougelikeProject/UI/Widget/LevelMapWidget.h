// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LevelNodeWidget.h"
#include "RLUserWidget.h"
#include "Components/CanvasPanel.h"
#include "RougelikeProject/UI/WidgetController/LevelMapWidgetController.h"
#include "SplineWidget.h"
#include "LevelMapWidget.generated.h"


class UOverlay;

UCLASS()
class ROUGELIKEPROJECT_API ULevelMapWidget : public URLUserWidget
{
	GENERATED_BODY()
public:
	ULevelMapWidget();

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UOverlay* Lines;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<ULevelNodeWidget> LevelNodeWidgetClass;
	
	TMap<int, ULevelNodeWidget*> NodeWidgetMap;

	UFUNCTION(BlueprintCallable)
	void UpdateNode(FNode node);

	UFUNCTION(BlueprintCallable)
	void AddLine(FLine line);
	
	UFUNCTION(BlueprintCallable)
	void UpdateLine(FLine line); // CurNode可通行，LastNode不可通行
	
	UFUNCTION(BlueprintCallable)
	void GenerateBackground_Test();

	UFUNCTION(BlueprintCallable)
	void ShowLevelMap(bool bShow);
};
