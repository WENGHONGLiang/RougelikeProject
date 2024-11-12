// Fill out your copyright notice in the Description page of Project Settings.


#include "RLAbilityActor.h"

#include "RougelikeProject/AbilitySystem/RLAbilitySystemLibrary.h"
#include "RougelikeProject/UI/HUD/RLHUD.h"

ARLAbilityActor::ARLAbilityActor()
{
	
}

void ARLAbilityActor::OnPickUp(bool bClick)
{
	Super::OnPickUp(bClick);

	if(!bBindPickEvent && !bClick)
		return;

	if(URLAbilitySystemLibrary::GetAbilityMenuWidgetController(this)->AddAbility(AbilityInfo.AbilityTag, AbilityInfo.AbilityLevel))
	{
		URLAbilitySystemComponent* ASC = Cast<URLAbilitySystemComponent>(PlayerCharacter->GetAbilitySystemComponent());
	
		ASC->AddCharacterAbility(AbilityInfo.AbilityClass, AbilityInfo.AbilityLevel, false);

		URLAbilitySystemLibrary::GetOverlayWidgetController(this)->HideMessage(All);
		Destroy();
	}
}

void ARLAbilityActor::InitAbilityActor_Implementation(FRLAbilityInfo Info)
{
	SetAbilityInfo(Info);
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
