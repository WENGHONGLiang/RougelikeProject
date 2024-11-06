// Fill out your copyright notice in the Description page of Project Settings.


#include "RLShopActor.h"

#include "Kismet/GameplayStatics.h"
#include "RougelikeProject/AbilitySystem/RLAbilitySystemLibrary.h"
#include "RougelikeProject/CharacterSet/RLCharacter.h"
#include "RougelikeProject/Player/RLPlayerController.h"
#include "RougelikeProject/UI/HUD/RLHUD.h"

ARLShopActor::ARLShopActor()
{
	BoxComp = CreateDefaultSubobject<UBoxComponent>("BoxComp");
	RootComponent = BoxComp;

	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComp");
	StaticMeshComp->SetupAttachment(RootComponent);
}

void ARLShopActor::BeginPlay()
{
	Super::BeginPlay();
	
	HUD = Cast<ARLHUD>(UGameplayStatics::GetPlayerController(this, 0)->GetHUD());

	HUD->UpdateShopCommodity();

	BoxComp->OnComponentBeginOverlap.AddDynamic(this, &ARLShopActor::BeginOverlap);
	BoxComp->OnComponentEndOverlap.AddDynamic(this, &ARLShopActor::EndOverlap);
}

void ARLShopActor::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ARLCharacter* Character = Cast<ARLCharacter>(OtherActor);
	if(!Character)
		return;

	if(!PlayerController)
	{
		PlayerCharacter = Character;
		PlayerController = Cast<ARLPlayerController>(Character->GetPlayerController());
		PlayerController->OnInteractEvent.AddDynamic(this, &ARLShopActor::OnInteract);
	}
	
	URLAbilitySystemLibrary::GetOverlayWidgetController(this)->SetTipMessageByTag(MessageTipTag_Open); // 交互提示
	
	bBindInteractEvent = true;
}

void ARLShopActor::EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ARLCharacter* Character = Cast<ARLCharacter>(OtherActor);
	if(!Character)
		return;

	if(const auto OverlayWidgetController = URLAbilitySystemLibrary::GetOverlayWidgetController(this))
		OverlayWidgetController->HideMessage(Tip);
	bBindInteractEvent = false;
}

void ARLShopActor::OnInteract()
{
	if(!bBindInteractEvent)
		return;
	
	if(!bShowShopWidget)
	{
		HUD->ShowShopWidget(true);
		bShowShopWidget = true;
		URLAbilitySystemLibrary::GetOverlayWidgetController(this)->SetTipMessageByTag(MessageTipTag_Close);
	}
	else
	{
		HUD->ShowShopWidget(false);
		bShowShopWidget = false;
		URLAbilitySystemLibrary::GetOverlayWidgetController(this)->SetTipMessageByTag(MessageTipTag_Open);
	}
	PlayerController->SetCanMove(!bShowShopWidget);
}
