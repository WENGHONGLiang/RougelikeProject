// Fill out your copyright notice in the Description page of Project Settings.


#include "AT_Move.h"

UAT_Move* UAT_Move::CreateATMove(UGameplayAbility* OwningAbility, FVector Direction, float Distance, float Speed, bool bLoopEvent, float LoopEventTime)
{
	UAT_Move* MyObj = NewAbilityTask<UAT_Move>(OwningAbility);
	
	MyObj->bMoving = false;
	MyObj->Direction = Direction;
	MyObj->Distance = Distance;
	MyObj->Speed = Speed;
	MyObj->bLoopEvent = bLoopEvent;
	MyObj->LoopEventTime = LoopEventTime;
	MyObj->LoopEventTimeCnt = 0;
	
	return MyObj;
}

void UAT_Move::Activate()
{
	Super::Activate();
	bTickingTask = true;
	AvatarActor = GetAvatarActor();
	StartPosition = GetAvatarActor()->GetActorLocation();
	bMoving = true;
}

void UAT_Move::TickTask(float DeltaTime)
{
	Super::TickTask(DeltaTime);

	if(!bMoving) return;

	// Loop Event
	LoopEventTimeCnt += DeltaTime;
	if(LoopEventTimeCnt >= LoopEventTime)
	{
		OnLoopEvent.Broadcast();
		LoopEventTimeCnt = 0;
	}

	// 位移
	FVector EndPosition = StartPosition + Direction * Distance;
	FVector NewPosition = FMath::Lerp(AvatarActor->GetActorLocation(), EndPosition,Speed * DeltaTime);
	AvatarActor->SetActorLocation(NewPosition);

	if(FVector::Distance(FVector(NewPosition.X, NewPosition.Y, 0), FVector(EndPosition.X, EndPosition.Y, 0)) < Distance / 10)
	{
		OnMoveEnd.Broadcast();
		bMoving = false;
		bTickingTask = false;
	}
}

void UAT_Move::OnDestroy(bool bInOwnerFinished)
{
	Super::OnDestroy(bInOwnerFinished);
}
