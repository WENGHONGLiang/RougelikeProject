// Fill out your copyright notice in the Description page of Project Settings.


#include "RLCharacterBase.h"
#include "AbilitySystemComponent.h"
#include "RougelikeProject/AbilitySystem/RLAbilitySystemComponent.h"
#include "RougelikeProject/ArributeBaseSet/AttributeSetBase.h"
#include "RougelikeProject/AbilitySystem/RLGameplayTags.h"


// Sets default values
ARLCharacterBase::ARLCharacterBase() {
	PrimaryActorTick.bCanEverTick = true;
	isAlive = true;
}

UAbilitySystemComponent* ARLCharacterBase::GetAbilitySystemComponent() const {
	return AbilitySystemComponent;
}

void ARLCharacterBase::ApplyEffectToSelf(TSubclassOf<UGameplayEffect> GameplayEffectClass, int32 Level)
{
	if(!ensure(GameplayEffectClass)) return;
	
	FGameplayEffectContextHandle ContextHandle = GetAbilitySystemComponent()->MakeEffectContext();
	ContextHandle.AddSourceObject(this);
	const FGameplayEffectSpecHandle SpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(GameplayEffectClass, Level, ContextHandle);
	GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), GetAbilitySystemComponent());
}

void ARLCharacterBase::InitAbilityActorInfo()
{

}

void ARLCharacterBase::BeginPlay() {
	Super::BeginPlay();
}

void ARLCharacterBase::AddCharacterAbilities()
{
	URLAbilitySystemComponent* ASC = CastChecked<URLAbilitySystemComponent>(AbilitySystemComponent);
	ASC->AddCharacterAbilities(StartupAbilities);
}

void ARLCharacterBase::GameplayEffectApplied(const FGameplayTagContainer& TagContainer)
{
	FRLGameplayTags GameplayTags = FRLGameplayTags::Get();
	
	bool bRolling = GetAbilitySystemComponent()->GetOwnedGameplayTags().HasTag(GameplayTags.HeroAbility_Roll);
	if(TagContainer.HasTag(GameplayTags.GE_Damage) && !bRolling)
	{
		OnBeAttacked();
	}

	if(TagContainer.HasTag(GameplayTags.GE_Damage_Normal) && !bRolling)
	{
		OnBeNormalAttacked();
	}
}

void ARLCharacterBase::CharacterDie()
{
	if(isAlive)
	{
		isAlive = false;
		OnDie();
	}
}
