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

	MessageTipTag = FRLGameplayTags::Get().Message_Tip_PickUp;
}

// Called when the game starts or when spawned
void APropertyActor::BeginPlay()
{
	Super::BeginPlay();
	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &APropertyActor::BeginOverlap);
	SphereComp->OnComponentEndOverlap.AddDynamic(this, &APropertyActor::EndOverlap);

}

void APropertyActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	SphereComp->OnComponentBeginOverlap.RemoveDynamic(this, &APropertyActor::BeginOverlap);
	SphereComp->OnComponentEndOverlap.RemoveDynamic(this, &APropertyActor::EndOverlap);
}

// Called every frame
void APropertyActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APropertyActor::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ARLCharacter* Character = Cast<ARLCharacter>(OtherActor);
	if(!Character)
		return;

	if(!PlayerController)
	{
		PlayerCharacter = Character;
	    PlayerController = Cast<ARLPlayerController>(Character->GetPlayerController());
		PlayerController->OnPickUpEvent.AddDynamic(this, &APropertyActor::OnButtonPickUp);
		PlayerController->OnCrushEvent.AddDynamic(this, &APropertyActor::OnCrush);
	}
	
	URLAbilitySystemLibrary::GetOverlayWidgetController(this)->SetTipMessageByTag(MessageTipTag); // 拾取提示
	bBindPickEvent = true;
}

void APropertyActor::EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if(!Cast<ARLCharacter>(OtherActor))
		return;
	
	URLAbilitySystemLibrary::GetOverlayWidgetController(this)->HideMessage(EMessageHideMode::All);
	bBindPickEvent = false;
}

void APropertyActor::NotifyActorOnReleased(FKey ButtonPressed)
{
	Super::NotifyActorOnReleased(ButtonPressed);
	OnPickUp(true);
}

void APropertyActor::OnButtonPickUp()
{
	OnPickUp(false);
}

void APropertyActor::OnPickUp(bool bClick)
{
	if(!bBindPickEvent && !bClick)
		return;
}

void APropertyActor::OnCrush()
{
	if(!bBindPickEvent)
		return;
}

void APropertyActor::Destroyed()
{
	Super::Destroyed();
}
