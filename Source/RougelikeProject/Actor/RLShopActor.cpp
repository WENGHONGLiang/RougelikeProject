// Fill out your copyright notice in the Description page of Project Settings.


#include "RLShopActor.h"

#include "Kismet/GameplayStatics.h"
#include "RougelikeProject/AbilitySystem/RLAbilitySystemLibrary.h"
#include "RougelikeProject/CharacterSet/RLCharacter.h"
#include "RougelikeProject/Player/RLPlayerController.h"
#include "RougelikeProject/UI/HUD/RLHUD.h"

void ARLShopActor::BeginPlay()
{
	Super::BeginPlay();
	
	HUD = Cast<ARLHUD>(UGameplayStatics::GetPlayerController(this, 0)->GetHUD());
	HUD->UpdateShopCommodity();
}

void ARLShopActor::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::BeginOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
}

void ARLShopActor::EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::EndOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
}

void ARLShopActor::OnInteract()
{
	Super::OnInteract();
	
	if(!bBindInteractEvent)
		return;
	
	if(!bShowShopWidget)
	{
		HUD->ShowShopWidget(true);
		bShowShopWidget = true;
		URLAbilitySystemLibrary::GetOverlayWidgetController(this)->SetTipMessageByTag(MessageTipTag_Close);
	}
	else
	{
		HUD->ShowShopWidget(false);
		bShowShopWidget = false;
		URLAbilitySystemLibrary::GetOverlayWidgetController(this)->SetTipMessageByTag(MessageTipTag_Open);
	}
	PlayerController->SetCanMove(!bShowShopWidget);
}
