// Fill out your copyright notice in the Description page of Project Settings.


#include "RLRecoverActor.h"

#include "RougelikeProject/AbilitySystem/RLAbilitySystemLibrary.h"

void ARLRecoverActor::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::BeginOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	
	if(bAlreadyUsed)
		URLAbilitySystemLibrary::GetOverlayWidgetController(this)->SetTipMessageByTag(MessageTipTag_Close);
}

void ARLRecoverActor::OnInteract()
{
	Super::OnInteract();
	
	if(!bBindInteractEvent)
		return;

	if(bAlreadyUsed)
		return;
	
	PlayerCharacter->ApplyEffectToSelf(GE_Recover, 1);
	bAlreadyUsed = true;
	
	URLAbilitySystemLibrary::GetOverlayWidgetController(this)->SetTipMessageByTag(MessageTipTag_Close);
}
