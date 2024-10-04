// Fill out your copyright notice in the Description page of Project Settings.


#include "PropertyActor.h"

#include "Particles/ParticleSystemComponent.h"
#include "RougelikeProject/AbilitySystem/RLAbilitySystemLibrary.h"
#include "RougelikeProject/AbilitySystem/RLGameplayTags.h"
#include "RougelikeProject/CharacterSet/RLCharacter.h"

APropertyActor::APropertyActor()
{
	PrimaryActorTick.bCanEverTick = true;

	SphereComp = CreateDefaultSubobject<USphereComponent>("SphereComp");
	RootComponent = SphereComp;

	EffectComp = CreateDefaultSubobject<UParticleSystemComponent>("EffectComp");
	EffectComp->SetupAttachment(SphereComp);
	
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MeshComp->SetupAttachment(SphereComp);

	TipTag = FRLGameplayTags::Get().Message_Tip_PickUp;
}

// Called when the game starts or when spawned
void APropertyActor::BeginPlay()
{
	Super::BeginPlay();
	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &APropertyActor::BeginOverlap);
	SphereComp->OnComponentEndOverlap.AddDynamic(this, &APropertyActor::EndOverlap);
}

// Called every frame
void APropertyActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APropertyActor::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(!Cast<ARLCharacter>(OtherActor))
		return;
	
	URLAbilitySystemLibrary::GetOverlayWidgetController(this)->SetMessage(MessageTag); // 具体装备
	
	URLAbilitySystemLibrary::GetOverlayWidgetController(this)->SetMessage(TipTag); // 拾取提示
}

void APropertyActor::EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if(!Cast<ARLCharacter>(OtherActor))
		return;
	
	URLAbilitySystemLibrary::GetOverlayWidgetController(this)->HideMessage(All);
}
