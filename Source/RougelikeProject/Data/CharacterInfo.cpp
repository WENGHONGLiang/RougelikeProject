// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterInfo.h"

FCharacterClassDefaultInfo UCharacterClassInfo::GetClassDefaultInfo(ECharacterType CharacterClass)
{
	return CharacterClassInformation.FindChecked((CharacterClass));
}
