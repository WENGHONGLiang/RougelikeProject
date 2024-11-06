// Fill out your copyright notice in the Description page of Project Settings.


#include "RLEnemy.h"

#include "AbilitySystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "RougelikeProject/AbilitySystem/RLAbilitySystemComponent.h"
#include "RougelikeProject/ArributeBaseSet/AttributeSetBase.h"
#include "RougelikeProject/Player/RLPlayerState.h"
#include "RougelikeProject/UI/Widget/RLUserWidget.h"

ARLEnemy::ARLEnemy()
{
	AbilitySystemComponent = CreateDefaultSubobject<URLAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AttributeSet = CreateDefaultSubobject<UAttributeSetBase>("AttributeSet");
	
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

	AbilitySystemComponent->InitAbilityActorInfo(this, this);

	URLAbilitySystemComponent* RLASC = Cast<URLAbilitySystemComponent>(AbilitySystemComponent);
	
	// ASC绑定GE被应用的回调函数
	RLASC->BindGameplayEffectAppliedDelegate();
	RLASC->OnGameplayEffectApplied.AddUObject(this, &ARLEnemy::GameplayEffectApplied);

	// 初始属性
	FCharacterClassDefaultInfo info = CharacterInfo->GetClassDefaultInfo(CharacterType);
	ApplyEffectToSelf(info.Attributes, GetCharacterLevel());
	RewardMoney = info.RewardMoney;
	
	// 添加初始技能
	AddCharacterAbilities();

	Cast<UAttributeSetBase>(AttributeSet)->OnCharacterDie.AddDynamic(this, &ARLEnemy::CharacterDie);
}

void ARLEnemy::GameplayEffectApplied(const FGameplayTagContainer& TagContainer)
{
	Super::GameplayEffectApplied(TagContainer);

	// TODO: 敌人受到角色特殊技能后的效果
}

void ARLEnemy::CharacterDie()
{
	Super::CharacterDie();

	Cast<ARLPlayerState>(UGameplayStatics::GetPlayerState(this, 0))->AddMoney(RewardMoney);
}

void ARLEnemy::UseAbility(const FGameplayTag& InputTag)
{
	Cast<URLAbilitySystemComponent>(AbilitySystemComponent)->ActivateAbilityDirectly(InputTag);
}
