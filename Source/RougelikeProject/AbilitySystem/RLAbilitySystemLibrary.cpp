// Fill out your copyright notice in the Description page of Project Settings.


#include "RLAbilitySystemLibrary.h"

#include "Kismet/GameplayStatics.h"
#include "RougelikeProject/CharacterSet/RLCharacter.h"
#include "RougelikeProject/UI/HUD/RLHUD.h"

UOverlayWidgetController* URLAbilitySystemLibrary::GetOverlayWidgetController(const UObject* WorldContextObject)
{
	// WorldContextObject 传入系统自带的静态函数后，会被隐式转换为 WorldContextObject.GetWorld() 世界环境。 // 在蓝图中只要连 this 就可以
	APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0);

	if(PC)
	{
		if(ARLHUD* HUD = Cast<ARLHUD>(PC->GetHUD()))
		{
			ARLCharacter* Character = Cast<ARLCharacter>(PC->GetCharacter());
			UAbilitySystemComponent* ASC = Character->GetAbilitySystemComponent();
			UAttributeSet* AS = Character->GetAttributeSet();

			const FWidgetControllerParams WidgetControllerParams(PC, ASC, AS);

			UOverlayWidgetController* WidgetController = HUD->GetOverlayWidgetController(WidgetControllerParams);
			
			return WidgetController;
		}
	}
	
	return nullptr;
}

UAttributeMenuWidgetController* URLAbilitySystemLibrary::GetAttributeMenuWidgetController(
	const UObject* WorldContextObject)
{
	APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0);

	if(PC)
	{
		if(ARLHUD* HUD = Cast<ARLHUD>(PC->GetHUD()))
		{
			ARLCharacter* Character = Cast<ARLCharacter>(PC->GetCharacter());
			UAbilitySystemComponent* ASC = Character->GetAbilitySystemComponent();
			UAttributeSet* AS = Character->GetAttributeSet();

			const FWidgetControllerParams WidgetControllerParams(PC, ASC, AS);

			UAttributeMenuWidgetController* WidgetController = HUD->GetAttributeMenuWidgetController(WidgetControllerParams);
			
			return WidgetController;
		}
	}
	
	return nullptr;
}
