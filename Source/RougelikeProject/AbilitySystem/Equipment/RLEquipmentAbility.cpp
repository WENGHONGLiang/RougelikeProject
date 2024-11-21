// Fill out your copyright notice in the Description page of Project Settings.


#include "RLEquipmentAbility.h"

#include "AbilitySystemComponent.h"
#include "RougelikeProject/CharacterSet/RLCharacter.h"

void URLEquipmentAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                          const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                          const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}


void URLEquipmentAbility::ApplyEquipmentEffect(TSubclassOf<UGameplayEffect> EquipmentEffectClass)
{
	if(ARLCharacter* Character = Cast<ARLCharacter>(GetAvatarActorFromActorInfo()))
	{
		Character->ApplyEffectToSelf(EquipmentEffectClass, GetAbilityLevel());
	}
}
