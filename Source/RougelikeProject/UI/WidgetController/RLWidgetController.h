// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "RLWidgetController.generated.h"

class UAbilitySystemComponent;
class UAttributeSet;

/* 用于初始化 Controller */
USTRUCT(BlueprintType, Blueprintable)
struct FWidgetControllerParams
{
	GENERATED_BODY()

	FWidgetControllerParams(){}

	FWidgetControllerParams(APlayerController* PC, UAbilitySystemComponent* ASC, UAttributeSet* AS, APlayerState* PS) :
	PlayerController(PC), AbilitySystemComponent(ASC), AttributeSet(AS), PlayerState(PS) {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<APlayerController> PlayerController = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UAttributeSet> AttributeSet = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<APlayerState> PlayerState = nullptr;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnShowSignature, bool, bShow);

UCLASS()
class ROUGELIKEPROJECT_API URLWidgetController : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void SetWidgetControllerParams(const FWidgetControllerParams& WCParams);

	// 初始化数值
	virtual void BroadcastInitialValues();

	// 绑定ASC数值变化事件
	virtual void BindCallbacksToDependencies();
	
	UFUNCTION(BlueprintCallable)
	void ShowWidget(bool bShow);

	FVector2D GetCursorPosition() const;
	
protected:
	
	UPROPERTY(BlueprintAssignable)
	FOnShowSignature OnShow;
	
	// Controller -> Model
	UPROPERTY(BlueprintReadOnly, Category="Widget Controller")
	TObjectPtr<APlayerController> PlayerController;

	UPROPERTY(BlueprintReadOnly, Category="Widget Controller")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(BlueprintReadOnly, Category="Widget Controller")
	TObjectPtr<UAttributeSet> AttributeSet;
	
	UPROPERTY(BlueprintReadOnly, Category="Widget Controller")
	TObjectPtr<APlayerState> PlayerState;
};
