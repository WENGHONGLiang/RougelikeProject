// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RLSubAbilityWidget.h"
#include "RLUserWidget.h"
#include "Components/Image.h"
#include "RougelikeProject/UI/WidgetController/AbilityMenuWidgetController.h"
#include "RLAbilityMenuWidget.generated.h"

/**
 * 
 */
UCLASS()
class ROUGELIKEPROJECT_API URLAbilityMenuWidget : public URLUserWidget
{
	GENERATED_BODY()
	
	void NativeConstruct() override;

	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
	bool bDraggingAbility;
	
protected:
	UAbilityMenuWidgetController* AbilityMenuWidgetController;
	
	UPROPERTY(meta = (BindWidget), BlueprintReadOnly)
	URLSubAbilityWidget* BP_SubAbility_LMB;
	
	UPROPERTY(meta = (BindWidget), BlueprintReadOnly)
	URLSubAbilityWidget* BP_SubAbility_1;
	
	UPROPERTY(meta = (BindWidget), BlueprintReadOnly)
	URLSubAbilityWidget* BP_SubAbility_2;
	
	UPROPERTY(meta = (BindWidget), BlueprintReadOnly)
	URLSubAbilityWidget* BP_SubAbility_3;
	
	UPROPERTY(meta = (BindWidget), BlueprintReadOnly)
	URLSubAbilityWidget* BP_SubAbility_4;
	
	UPROPERTY(meta = (BindWidget), BlueprintReadOnly)
	UImage* AbilityIcon;
	
	UFUNCTION()
	void OnClicked_LMB();
	
	UFUNCTION()
	void OnClicked_1();
	
	UFUNCTION()
	void OnClicked_2();
	
	UFUNCTION()
	void OnClicked_3();
	
	UFUNCTION()
	void OnClicked_4();

	UFUNCTION(BlueprintCallable)
	void SetDragging(bool bDrag);
};
