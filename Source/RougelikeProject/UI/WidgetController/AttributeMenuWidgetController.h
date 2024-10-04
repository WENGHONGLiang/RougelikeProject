// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RLWidgetController.h"
#include "AttributeMenuWidgetController.generated.h"

class UAttributeInfo;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAttributeInfoSignature, const FRLAttributeInfo&, Info);
	
UCLASS(Blueprintable)
class ROUGELIKEPROJECT_API UAttributeMenuWidgetController : public URLWidgetController
{
	GENERATED_BODY()
public:
	// 在HUD中新建时调用
	virtual void BindCallbacksToDependencies() override;

	// 在蓝图中创建Widget时调用
	UFUNCTION(BlueprintCallable)
	virtual void BroadcastInitialValues() override;

	UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")
	FAttributeInfoSignature AttributeInfoDelegate;

protected:
	// 编辑好的 Data Asset
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UAttributeInfo> AttributeInfo;
};
