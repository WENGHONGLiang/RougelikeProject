// Fill out your copyright notice in the Description page of Project Settings.


#include "RLAbilityUpgradeActor.h"

#include "RougelikeProject/AbilitySystem/RLAbilitySystemLibrary.h"

void ARLAbilityUpgradeActor::OnInteract()
{
	Super::OnInteract();
	
	if(!bBindInteractEvent)
		return;

	if(!bShowAbilityUpgradePanel)
	{
		URLAbilitySystemLibrary::GetAbilityMenuWidgetController(this)->ShowAbilityUpgradePanel(true);
		URLAbilitySystemLibrary::GetOverlayWidgetController(this)->SetTipMessageByTag(MessageTipTag_Close);
		bShowAbilityUpgradePanel = true;
	}
	else
	{
		URLAbilitySystemLibrary::GetAbilityMenuWidgetController(this)->ShowAbilityUpgradePanel(false);
		URLAbilitySystemLibrary::GetOverlayWidgetController(this)->SetTipMessageByTag(MessageTipTag_Open);
		bShowAbilityUpgradePanel = false;
	}
}


void ARLAbilityUpgradeActor::EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::EndOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);

	if(UAbilityMenuWidgetController* AbilityMenuWidgetController = URLAbilitySystemLibrary::GetAbilityMenuWidgetController(this))
		AbilityMenuWidgetController->ShowAbilityUpgradePanel(false);
	bShowAbilityUpgradePanel = false;
}
