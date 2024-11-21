// Fill out your copyright notice in the Description page of Project Settings.


#include "RLEquipmentActor.h"

#include "RougelikeProject/AbilitySystem/RLAbilitySystemLibrary.h"
#include "RougelikeProject/AbilitySystem/RLGameplayTags.h"
#include "RougelikeProject/Player/RLPlayerState.h"


ARLEquipmentActor::ARLEquipmentActor()
{
	EquipmentInfo.EquipmentTag = FRLGameplayTags::Get().Equipment_Empty;
}

void ARLEquipmentActor::OnPickUp(bool bClick)
{
	Super::OnPickUp(bClick);
	if(!bBindPickEvent && !bClick)
		return;
	const auto AddRes = URLAbilitySystemLibrary::GetAbilityMenuWidgetController(this)->AddProperty(EquipmentInfo.EquipmentTag, EquipmentInfo.EquipmentLevel);

	// 新装备需要实际添加到ASC
	if(AddRes == NewlyAdded)
	{
		URLAbilitySystemComponent* ASC = Cast<URLAbilitySystemComponent>(PlayerCharacter->GetAbilitySystemComponent());
		ASC->AddCharacterAbility(EquipmentInfo.EquipmentAbilityClass, EquipmentInfo.EquipmentLevel, false);
	}
	if(AddRes != DontAdd)
	{
		URLAbilitySystemLibrary::GetOverlayWidgetController(this)->HideMessage(EMessageHideMode::All);
		Destroy();
	}
}

void ARLEquipmentActor::OnCrush()
{
	Super::OnCrush();
	if(!bBindPickEvent)
		return;

	ARLPlayerState* RLPS = PlayerController->GetPlayerState<ARLPlayerState>();
	RLPS->AddSkillCoin(EquipmentInfo.EquipmentSkillCoinValue);
	URLAbilitySystemLibrary::GetOverlayWidgetController(this)->HideMessage(EMessageHideMode::All);
	Destroy();
}

void ARLEquipmentActor::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::BeginOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	if(!EquipmentInfo.EquipmentTag.MatchesTagExact(FRLGameplayTags::Get().Equipment_Empty))
		URLAbilitySystemLibrary::GetOverlayWidgetController(this)->SetPropertyMessage(EquipmentInfo.EquipmentName, EquipmentInfo.EquipmentQuality, EquipmentInfo.EquipmentDescription, EquipmentInfo.EquipmentLevel, 0, EquipmentInfo.EquipmentBaseCooldown);
	else
	{
		FTimerDelegate Delegate = FTimerDelegate::CreateLambda([this]()
		{
			if(!EquipmentInfo.EquipmentTag.MatchesTagExact(FRLGameplayTags::Get().Equipment_Empty))
				URLAbilitySystemLibrary::GetOverlayWidgetController(this)->SetPropertyMessage(EquipmentInfo.EquipmentName, EquipmentInfo.EquipmentQuality, EquipmentInfo.EquipmentDescription, EquipmentInfo.EquipmentLevel, 0, EquipmentInfo.EquipmentBaseCooldown);
		});
		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, Delegate, 0.1f, false);
	}
}

void ARLEquipmentActor::InitEquipmentActor_Implementation(FRLEquipmentInfo Info)
{
	SetEquipmentInfo(Info);
}
