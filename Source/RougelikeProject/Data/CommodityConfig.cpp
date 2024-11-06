// Fill out your copyright notice in the Description page of Project Settings.


#include "CommodityConfig.h"

FCommodityInfo& UCommodityConfig::FindCommodityInfo(CommodityType commodityType, int commodityID)
{
	switch (commodityType)
	{
	case Ability:
		if(commodityID < AbilityCommodityInfo.Num())
			return AbilityCommodityInfo[commodityID];
		break;
	case Equipment:
		if(commodityID < EquipmentCommodityInfo.Num())
			return EquipmentCommodityInfo[commodityID];
		break;
	case Item:
		if(commodityID < ItemCommodityInfo.Num())
			return ItemCommodityInfo[commodityID];
		break;
	}
	return AbilityCommodityInfo.Top();
}
