// Fill out your copyright notice in the Description page of Project Settings.


#include "ShopWidgetController.h"

#include "Kismet/GameplayStatics.h"
#include "RougelikeProject/GameMode/RLGameMode.h"
#include "RougelikeProject/Player/RLPlayerState.h"

void UShopWidgetController::BindCallbacksToDependencies()
{
	Super::BindCallbacksToDependencies();
	
}

void UShopWidgetController::BroadcastInitialValues()
{
	Super::BroadcastInitialValues();
}

void UShopWidgetController::UpdateCommodity()
{
	CommodityInfos.Reset();
	for(int i = 0; i < 8; i++)
	{
		const int AbilityCommodityID = FMath::RandRange(0, CommodityConfig->GetAbilityCommodityNum()-1);
		FCommodityInfo CommodityInfo = CommodityConfig->FindCommodityInfo(Ability, AbilityCommodityID);
		
		if(!CommodityInfo.bHasInit)
		{
			FRLAbilityInfo& AbilityInfo = AbilityConfig->FindAbilityInfo(CommodityInfo.Tag);
			CommodityInfo.Description = AbilityInfo.AbilityDescription;
			CommodityInfo.CommodityImage = AbilityInfo.AbilityImage;
			CommodityInfo.Type = Ability;
			// Other Value...
		}
		
		CommodityInfos.Add(CommodityInfo);
		OnUpdateCommodity.Broadcast(i, CommodityInfo);
	}
}

void UShopWidgetController::BuyCommodity(int CommodityID)
{
	FCommodityInfo CommodityInfo = CommodityInfos[CommodityID];
	ARLPlayerState* RLPlayerState = Cast<ARLPlayerState>(PlayerState);

	// 扣费
	if(!RLPlayerState->CostMoney(CommodityInfo.Cost))
		return;
	
	if(CommodityInfo.Type == Ability)
	{
		ARLGameMode* GameMode = Cast<ARLGameMode>(UGameplayStatics::GetGameMode(this));
		GameMode->SpawnAblityActorAroundPlayer(CommodityInfo.Tag, CommodityInfo.Level);
	}
	CommodityInfo.Number--;
	OnUpdateCommodity.Broadcast(CommodityID, CommodityInfo);
}
