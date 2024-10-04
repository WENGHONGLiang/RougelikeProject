// Fill out your copyright notice in the Description page of Project Settings.


#include "RLEnemy.h"

#include "AbilitySystemComponent.h"
#include "RougelikeProject/AbilitySystem/RLAbilitySystemComponent.h"
#include "RougelikeProject/ArributeBaseSet/AttributeSetBase.h"
#include "RougelikeProject/UI/Widget/RLUserWidget.h"

ARLEnemy::ARLEnemy()
{
	HealthBar = CreateDefaultSubobject<UWidgetComponent>("HealthBar");
	HealthBar->SetupAttachment(GetRootComponent());
}

void ARLEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	// 初始化 ASC
	InitAbilityActorInfo();
	
	if(URLUserWidget* WUserWidget = Cast<URLUserWidget>(HealthBar->GetUserWidgetObject()))
	{
		WUserWidget->SetWidgetController(this);
	}
	
	if(const UAttributeSetBase* WAS = CastChecked<UAttributeSetBase>(AttributeSet))
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(WAS->GetHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			 
			{
				OnHealthChanged.Broadcast(Data.NewValue);
			}
		);
		
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(WAS->GetMaxHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnMaxHealthChanged.Broadcast(Data.NewValue);
			}
		);
		
		OnHealthChanged.Broadcast(WAS->GetHealth());
		OnMaxHealthChanged.Broadcast(WAS->GetMaxHealth());
	}
}

void ARLEnemy::InitAbilityActorInfo()
{
	Super::InitAbilityActorInfo();
}

void ARLEnemy::GameplayEffectApplied(const FGameplayTagContainer& TagContainer)
{
	Super::GameplayEffectApplied(TagContainer);

	// TODO: 敌人受到角色特殊技能后的效果
}

void ARLEnemy::UseAbility(const FGameplayTag& InputTag)
{
	Cast<URLAbilitySystemComponent>(AbilitySystemComponent)->ActivateAbilityDirectly(InputTag);
}
