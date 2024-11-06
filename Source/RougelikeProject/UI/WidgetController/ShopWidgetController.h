// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RLWidgetController.h"
#include "RougelikeProject/Data/AbilityConfig.h"
#include "RougelikeProject/Data/CommodityConfig.h"
#include "ShopWidgetController.generated.h"



DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCommodityUpdateSignature, int, CommodityID, FCommodityInfo, CommodityInfo);

UCLASS(Blueprintable)
class ROUGELIKEPROJECT_API UShopWidgetController : public URLWidgetController
{
	GENERATED_BODY()
public:
	void BindCallbacksToDependencies() override;
	void BroadcastInitialValues() override;
	void UpdateCommodity();
	
protected:
	TArray<FCommodityInfo> CommodityInfos;
	
	UFUNCTION(BlueprintCallable)
	void BuyCommodity(int CommodityID);
	
	UPROPERTY(BlueprintAssignable)
	FOnCommodityUpdateSignature OnUpdateCommodity;
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UCommodityConfig> CommodityConfig;
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UAbilityConfig> AbilityConfig;
};
