// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelConfig.h"

FLevelInfo ULevelConfig::GetLevelInfo(ELevelType LevelType)
{
	return LevelInformation.FindChecked((LevelType));
}
