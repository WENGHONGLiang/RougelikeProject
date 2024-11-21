// Fill out your copyright notice in the Description page of Project Settings.


#include "RLInteractableActor.h"

#include "RougelikeProject/AbilitySystem/RLAbilitySystemLibrary.h"

ARLInteractableActor::ARLInteractableActor()
{
	BoxComp = CreateDefaultSubobject<UBoxComponent>("BoxComp");
	RootComponent = BoxComp;

	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComp");
	StaticMeshComp->SetupAttachment(RootComponent);
}

void ARLInteractableActor::BeginPlay()
{
	Super::BeginPlay();

	BoxComp->OnComponentBeginOverlap.AddDynamic(this, &ARLInteractableActor::BeginOverlap);
	BoxComp->OnComponentEndOverlap.AddDynamic(this, &ARLInteractableActor::EndOverlap);
}

void ARLInteractableActor::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ARLCharacter* Character = Cast<ARLCharacter>(OtherActor);
	if(!Character)
		return;

	if(!PlayerController)
	{
		PlayerCharacter = Character;
		PlayerController = Cast<ARLPlayerController>(Character->GetPlayerController());
		PlayerController->OnInteractEvent.AddDynamic(this, &ARLInteractableActor::OnInteract);
	}
	
	URLAbilitySystemLibrary::GetOverlayWidgetController(this)->SetTipMessageByTag(MessageTipTag_Open); // 交互提示
	
	bBindInteractEvent = true;
}

void ARLInteractableActor::EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ARLCharacter* Character = Cast<ARLCharacter>(OtherActor);
	if(!Character)
		return;

	if(const auto OverlayWidgetController = URLAbilitySystemLibrary::GetOverlayWidgetController(this))
		OverlayWidgetController->HideMessage(EMessageHideMode::Tip);
	bBindInteractEvent = false;
}

void ARLInteractableActor::OnInteract()
{
	if(!bBindInteractEvent)
		return;
}
