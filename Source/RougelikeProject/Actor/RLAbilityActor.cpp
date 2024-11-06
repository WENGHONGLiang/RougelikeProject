// Fill out your copyright notice in the Description page of Project Settings.


#include "RLAbilityActor.h"

#include "RougelikeProject/AbilitySystem/RLAbilitySystemLibrary.h"
#include "RougelikeProject/UI/HUD/RLHUD.h"

ARLAbilityActor::ARLAbilityActor()
{
	
}

void ARLAbilityActor::OnPickUp()
{
	Super::OnPickUp();

	if(!bBindPickEvent)
		return;

	URLAbilitySystemLibrary::GetAbilityMenuWidgetController(this)->AddAbility(AbilityInfo.AbilityTag);
	
	URLAbilitySystemComponent* ASC = Cast<URLAbilitySystemComponent>(PlayerCharacter->GetAbilitySystemComponent());
	
	ASC->AddCharacterAbility(AbilityInfo.AbilityClass, false);

	URLAbilitySystemLibrary::GetOverlayWidgetController(this)->HideMessage(All);
	Destroy();
}

void ARLAbilityActor::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ARLCharacter* Character = Cast<ARLCharacter>(OtherActor);
	if(!Character)
		return;
	
	Super::BeginOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	
	URLAbilitySystemLibrary::GetOverlayWidgetController(this)->SetPropertyMessage(AbilityInfo.AbilityDescription, AbilityInfo.AbilityImage);
}

void ARLAbilityActor::EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::EndOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
}
