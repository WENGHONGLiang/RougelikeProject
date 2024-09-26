// Fill out your copyright notice in the Description page of Project Settings.


#include "RLCharacterBase.h"
#include "AbilitySystemComponent.h"


// Sets default values
ARLCharacterBase::ARLCharacterBase() {
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	if (bUseGAS) {
		//创建ASC组件
		AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	}
}

UAbilitySystemComponent* ARLCharacterBase::GetAbilitySystemComponent() const {
	return AbilitySystemComponent;
}

// Called when the game starts or when spawned
void ARLCharacterBase::BeginPlay() {
	if (bUseGAS) {
		for (auto Element : AttributeSetClasses) {
			UAttributeSet* AttributeSet = NewObject<UAttributeSet>(this, Element);
			AbilitySystemComponent->AddSpawnedAttribute(AttributeSet);
		}
	}
	Super::BeginPlay();
}
