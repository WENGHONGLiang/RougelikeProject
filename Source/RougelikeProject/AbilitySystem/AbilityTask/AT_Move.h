// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "RougelikeProject/UI/WidgetController/OverlayWidgetController.h"
#include "AT_Move.generated.h"

UCLASS()
class ROUGELIKEPROJECT_API UAT_Move : public UAbilityTask
{
	GENERATED_BODY()
protected:
	UPROPERTY(BlueprintAssignable)
	FEventSignature OnMoveEnd;
	
	UPROPERTY(BlueprintAssignable)
	FEventSignature OnLoopEvent;
	
	UFUNCTION(BlueprintCallable, Category="Ability|Tasks", meta = (DisplayName = "MoveCharacter", HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "true"))
	static  UAT_Move* CreateATMove(UGameplayAbility* OwningAbility, FVector Direction, float Distance, float Speed, bool bLoopEvent = false, float LoopEventTime = 0);

	void Activate() override;
	
	void TickTask(float DeltaTime) override;

	void OnDestroy(bool bInOwnerFinished) override;

	bool bMoving;

	float LoopEventTime;
	float LoopEventTimeCnt;
	bool bLoopEvent;
	FVector Direction;
	float Distance;
	FVector StartPosition;
	float Speed;

	float MaxMoveTime;
	float MoveTime;

	UPROPERTY()
	AActor* AvatarActor;
};
