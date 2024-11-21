// Fill out your copyright notice in the Description page of Project Settings.


#include "RLAbilityActor.h"

#include "RougelikeProject/AbilitySystem/RLAbilitySystemLibrary.h"
#include "RougelikeProject/AbilitySystem/RLGameplayTags.h"
#include "RougelikeProject/Player/RLPlayerState.h"
#include "RougelikeProject/UI/HUD/RLHUD.h"

ARLAbilityActor::ARLAbilityActor()
{
	AbilityInfo.AbilityTag = FRLGameplayTags::Get().HeroAbility_Empty;
}

void ARLAbilityActor::OnPickUp(bool bClick)
{
	Super::OnPickUp(bClick);

	if(!bBindPickEvent && !bClick)
		return;
	const auto AddRes = URLAbilitySystemLibrary::GetAbilityMenuWidgetController(this)->AddProperty(AbilityInfo.AbilityTag, AbilityInfo.AbilityLevel);

	// 新技能需要实际添加到ASC
	if(AddRes == NewlyAdded)
	{
		URLAbilitySystemComponent* ASC = Cast<URLAbilitySystemComponent>(PlayerCharacter->GetAbilitySystemComponent());
	
		ASC->AddCharacterAbility(AbilityInfo.AbilityClass, AbilityInfo.AbilityLevel, false);
	}
	if(AddRes != DontAdd)
	{
		URLAbilitySystemLibrary::GetOverlayWidgetController(this)->HideMessage(EMessageHideMode::All);
		Destroy();
	}
}

void ARLAbilityActor::OnCrush()
{
	Super::OnCrush();

	if(!bBindPickEvent)
		return;

	ARLPlayerState* RLPS = PlayerController->GetPlayerState<ARLPlayerState>();
	RLPS->AddSkillCoin(AbilityInfo.AbilitySkillCoinValue);
	URLAbilitySystemLibrary::GetOverlayWidgetController(this)->HideMessage(EMessageHideMode::All);
	Destroy();
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

	if(!AbilityInfo.AbilityTag.MatchesTagExact(FRLGameplayTags::Get().HeroAbility_Empty))
		URLAbilitySystemLibrary::GetOverlayWidgetController(this)->SetPropertyMessage(AbilityInfo.AbilityName, AbilityInfo.AbilityQuality, AbilityInfo.AbilityDescription, AbilityInfo.AbilityLevel, AbilityInfo.AbilityBaseDamage, AbilityInfo.AbilityBaseCooldown);
	else
	{
		FTimerDelegate Delegate = FTimerDelegate::CreateLambda([this]()
		{
			if(!AbilityInfo.AbilityTag.MatchesTagExact(FRLGameplayTags::Get().HeroAbility_Empty))
				URLAbilitySystemLibrary::GetOverlayWidgetController(this)->SetPropertyMessage(AbilityInfo.AbilityName, AbilityInfo.AbilityQuality, AbilityInfo.AbilityDescription, AbilityInfo.AbilityLevel, AbilityInfo.AbilityBaseDamage, AbilityInfo.AbilityBaseCooldown);
		});
		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, Delegate, 0.1f, false);
	}
}
