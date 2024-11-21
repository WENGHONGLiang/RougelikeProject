// Fill out your copyright notice in the Description page of Project Settings.


#include "RLOpenLevelMapActor.h"

#include "Kismet/GameplayStatics.h"
#include "RougelikeProject/AbilitySystem/RLAbilitySystemLibrary.h"
#include "RougelikeProject/GameMode/RLGameMode.h"

void ARLOpenLevelMapActor::BeginPlay()
{
	Super::BeginPlay();

	bEnemyClear = false;
	GM = Cast<ARLGameMode>(UGameplayStatics::GetGameMode(this));
	GM->OnEnemyClear.AddLambda([this]()
	{
		bEnemyClear = true;

		// TODO: 播放特效
	});
}

void ARLOpenLevelMapActor::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::BeginOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	if(!bEnemyClear)
		URLAbilitySystemLibrary::GetOverlayWidgetController(this)->SetTipMessageByTag(MessageTipTag_Close); // 未开启
}

void ARLOpenLevelMapActor::OnInteract()
{
	Super::OnInteract();

	if(!bBindInteractEvent)
		return;

	if(bEnemyClear)
	{
		GM->EndLevel();
	}
}
