// Fill out your copyright notice in the Description page of Project Settings.


#include "RLCharacterBase.h"
#include "AbilitySystemComponent.h"
#include "RougelikeProject/AbilitySystem/RLAbilitySystemComponent.h"
#include "RougelikeProject/ArributeBaseSet/AttributeSetBase.h"
#include "RougelikeProject/AbilitySystem/RLGameplayTags.h"


// Sets default values
ARLCharacterBase::ARLCharacterBase() {
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	// 创建ASC组件
	AbilitySystemComponent = CreateDefaultSubobject<URLAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AttributeSet = CreateDefaultSubobject<UAttributeSetBase>("AttributeSet");

	isAlive = true;
}

UAbilitySystemComponent* ARLCharacterBase::GetAbilitySystemComponent() const {
	return AbilitySystemComponent;
}

void ARLCharacterBase::ApplyEffectToSelf(TSubclassOf<UGameplayEffect> IniGameplayEffectClass, float CharacterLevel)
{
	if(!ensure(IniGameplayEffectClass)) return;
	
	FGameplayEffectContextHandle ContextHandle = GetAbilitySystemComponent()->MakeEffectContext();
	ContextHandle.AddSourceObject(this);
	const FGameplayEffectSpecHandle SpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(IniGameplayEffectClass, CharacterLevel, ContextHandle);
	GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), GetAbilitySystemComponent());
}

void ARLCharacterBase::InitAbilityActorInfo()
{
	AbilitySystemComponent->InitAbilityActorInfo(this, this);

	URLAbilitySystemComponent* RLASC = Cast<URLAbilitySystemComponent>(AbilitySystemComponent);
	
	// ASC绑定GE被应用的回调函数
	RLASC->BindGameplayEffectAppliedDelegate();
	RLASC->OnGameplayEffectApplied.AddUObject(this, &ARLCharacterBase::GameplayEffectApplied);

	// 初始属性
	FCharacterClassDefaultInfo info = CharacterInfo->GetClassDefaultInfo(CharacterType);
	ApplyEffectToSelf(info.Attributes, GetCharacterLevel());

	// 添加初始技能
	AddCharacterAbilities();

	Cast<UAttributeSetBase>(AttributeSet)->OnCharacterDie.AddDynamic(this, &ARLCharacterBase::CharacterDie);

}

// Called when the game starts or when spawned
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
	
	if(TagContainer.HasTag(GameplayTags.GE_Damage))
	{
		OnBeAttacked();
	}

	if(TagContainer.HasTag(GameplayTags.GE_Damage_Normal))
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
