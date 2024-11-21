// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "InputAction.h"
#include "InputActionValue.h"
#include "Components/SplineComponent.h"
#include "GameFramework/PlayerController.h"
#include "RougelikeProject/AbilitySystem/RLAbilitySystemComponent.h"
#include "RougelikeProject/Data/RLInputConfig.h"
#include "RLPlayerController.generated.h"

class UInputMappingContext;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnButtonClickSignature);

UCLASS()
class ROUGELIKEPROJECT_API ARLPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	ARLPlayerController();

	FOnButtonClickSignature OnMouseClickEvent;
	FOnButtonClickSignature OnInteractEvent;
	FOnButtonClickSignature OnPickUpEvent;
	FOnButtonClickSignature OnCrushEvent;

	UFUNCTION(BlueprintCallable)
	void CallInteract() { OnInteractEvent.Broadcast(); };

	UFUNCTION(BlueprintCallable)
	void SetCanMove(const bool bCan);

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	virtual void Tick(float DeltaSeconds) override;
	
	void CursorTrace();
	void AutoRun();

	void RotateWithCursor();
	
	URLAbilitySystemComponent* GetASC();
	
	// 回调函数
	UFUNCTION()
	void Move(const FInputActionValue& InputActionValue);

	void AbilityInputTagPressed(FGameplayTag InputTag);
	void AbilityInputTagReleased(FGameplayTag InputTag);
	void AbilityInputTagHeld(FGameplayTag InputTag);

	bool bCanMove = true;
	
private:
	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputMappingContext> IM_Context;

	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<URLInputConfig> InputConfig;
	
	UPROPERTY()
	URLAbilitySystemComponent* RLAbilitySystemComponent;

	FHitResult CursorHit;

	// 点击移动相关参数
	FVector CachedDestination = FVector::ZeroVector;
	float FollowTime = 0.f;
	float ShortPressThreshold = 0.5f;

	float MovePressTime = 0.f;
	UPROPERTY(EditDefaultsOnly)
	float MovePressInterval = .2f;
	bool bAutoRunning = false;

	UPROPERTY(EditDefaultsOnly)
	float AutoRunAcceptanceRadius = 50.f;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USplineComponent> Spline;

	// 闪避相关参数
	float RollPressIntervalTime = 0.f;
	float RollPressIntervalCD = 1.f;
	bool bRolling = false;
	bool bCanRoll = true;
	float RollContinuousCD = 1.9f;
	float RollContinuousTime = 0.f;
};
