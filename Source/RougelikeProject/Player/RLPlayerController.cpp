// Fill out your copyright notice in the Description page of Project Settings.


#include "RLPlayerController.h"

#include "EnhancedInputSubsystems.h"
#include "RougelikeProject/Input/RLEnhancedInputComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "RougelikeProject/AbilitySystem/RLGameplayTags.h"

ARLPlayerController::ARLPlayerController()
{
	Spline = CreateDefaultSubobject<USplineComponent>("Spline");
}

void ARLPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	if(!ensureAlways(IM_Context)) return;

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if(Subsystem)
		Subsystem->AddMappingContext(IM_Context, 0);

	/*	Set Cursor	*/
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;
	
	FInputModeGameAndUI InputModeData;
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputModeData.SetHideCursorDuringCapture(false);
	SetInputMode(InputModeData);
}

void ARLPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	URLEnhancedInputComponent* WInputComponent = CastChecked<URLEnhancedInputComponent>(InputComponent);

	// Action绑定回调函数 // WASD移动单独挂，其它都作为可绑定键位
	WInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ARLPlayerController::Move);
	WInputComponent->BindAbilityActions(InputConfig, this, &ARLPlayerController::AbilityInputTagPressed, &ARLPlayerController::AbilityInputTagReleased, &ARLPlayerController::AbilityInputTagHeld);
}

void ARLPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	CursorTrace();
	AutoRun();
	RotateWithCursor();
	
	if(MovePressTime < MovePressInterval)
	{
		MovePressTime += DeltaSeconds;
	}
}



void ARLPlayerController::AutoRun()
{
	if(!bAutoRunning) return;
	if(APawn* ControllerPawn = GetPawn())
	{
		const FVector LocationOnSpline = Spline->FindLocationClosestToWorldLocation(ControllerPawn->GetActorLocation(), ESplineCoordinateSpace::World);
		const FVector Direction = Spline->FindDirectionClosestToWorldLocation(LocationOnSpline, ESplineCoordinateSpace::World);
		ControllerPawn->AddMovementInput(Direction);

		const float DistanceToDestination = (LocationOnSpline - CachedDestination).Length();
		if(DistanceToDestination <= AutoRunAcceptanceRadius)
		{
			bAutoRunning = false;
		}
	}
}

void ARLPlayerController::RotateWithCursor()
{
	APawn* ControllerPawn = GetPawn<APawn>();
	FVector Dir = CursorHit.ImpactPoint - ControllerPawn->GetActorLocation();
	FRotator Rotation = Dir.Rotation();
	Rotation.Pitch = ControllerPawn->GetActorRotation().Pitch;
	ControllerPawn->SetActorRotation(Rotation);
}

URLAbilitySystemComponent* ARLPlayerController::GetASC()
{
	if(RLAbilitySystemComponent == nullptr)
	{
		RLAbilitySystemComponent = Cast<URLAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn<APawn>()));
	}
	return RLAbilitySystemComponent;
}


void ARLPlayerController::Move(const FInputActionValue& InputActionValue)
{
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	const FVector2d InputAxisVector = InputActionValue.Get<FVector2d>();

	APawn* ControllerPawn = GetPawn<APawn>();
	if(!ensureAlways(ControllerPawn)) return;
	// 实际角色添加运动
	ControllerPawn->AddMovementInput(FVector(1, 0, 0), InputAxisVector.Y);
	ControllerPawn->AddMovementInput(FVector(0, 1, 0), InputAxisVector.X);
}


void ARLPlayerController::CursorTrace()
{
	GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);

	if(!CursorHit.bBlockingHit) return;
}


void ARLPlayerController::AbilityInputTagPressed(FGameplayTag InputTag)
{
	if(InputTag.MatchesTagExact(FRLGameplayTags::Get().InputTag_RMB) && MovePressTime >= MovePressInterval)
	{
		bAutoRunning = false;
	}
}


void ARLPlayerController::AbilityInputTagHeld(FGameplayTag InputTag)
{
	if(InputTag.MatchesTagExact(FRLGameplayTags::Get().InputTag_RMB))
	{
		// 点击移动
		FollowTime += GetWorld()->GetDeltaSeconds();

		if(CursorHit.bBlockingHit)
		{
			CachedDestination = CursorHit.ImpactPoint;
		}

		if(APawn* ControllerPawn = GetPawn())
		{
			const FVector WorldDir = (CachedDestination - ControllerPawn->GetActorLocation()).GetSafeNormal();
			ControllerPawn->AddMovementInput(WorldDir);
		}
		return;
	}
	GetASC()->AbilityInputTagHeld(InputTag);
}


void ARLPlayerController::AbilityInputTagReleased(FGameplayTag InputTag)
{
	if(InputTag.MatchesTagExact(FRLGameplayTags::Get().InputTag_RMB))
	{
		APawn* ControllerPawn = GetPawn();
		if(FollowTime <= ShortPressThreshold && ControllerPawn && MovePressTime >= MovePressInterval)
		{
			UNavigationPath* NavPath = UNavigationSystemV1::FindPathToLocationSynchronously(this, ControllerPawn->GetActorLocation(), CachedDestination);
			if(NavPath && NavPath->PathPoints.Num() >= 1)
			{
				Spline->ClearSplinePoints();
				for(const FVector& PointLoc : NavPath->PathPoints)
				{
					Spline->AddSplinePoint(PointLoc, ESplineCoordinateSpace::World);
					//DrawDebugSphere(GetWorld(), PointLoc, 8.f, 8, FColor::Green, false, 5.f);
				}
				CachedDestination = NavPath->PathPoints[NavPath->PathPoints.Num() - 1];
				bAutoRunning = true;
			}
			MovePressTime = 0.f;
		}
		FollowTime = 0.f;
		return;
	}
	GetASC()->AbilityInputTagReleased(InputTag);
}