// Copyright (Vikas)


#include "Player/AuraPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Math/RotationMatrix.h"
#include "InputActionValue.h"
#include "Engine/LocalPlayer.h"
#include "GameFramework/Pawn.h"
#include "Interaction/EnemyInterface.h"

#include "Engine/GameEngine.h"

AAuraPlayerController::AAuraPlayerController()
{
	//multiplayer replication...updates client server
	bReplicates = true;
}

void AAuraPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	CursorTrace();
}

void AAuraPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// halt/assert execution if input is not working
	check(AuraContext);

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	check(Subsystem);
	Subsystem->AddMappingContext(AuraContext, 0);

	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;

	//cursor setup
	FInputModeGameAndUI InputModeData;
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputModeData.SetHideCursorDuringCapture(false);
	SetInputMode(InputModeData);
}

void AAuraPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);

	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAuraPlayerController::Move);
	
}

void AAuraPlayerController::Move(const FInputActionValue& InputActionValue)
{
	const FVector2d InputAxisVector = InputActionValue.Get<FVector2d>();
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		ControlledPawn->AddMovementInput(ForwardDirection, InputAxisVector.Y);
		ControlledPawn->AddMovementInput(RightDirection, InputAxisVector.X);	
	}
	
}

void AAuraPlayerController::CursorTrace()
{
	FHitResult CursorHit;
	GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);
	if(!CursorHit.bBlockingHit) return;

	LastActor = ThisActor;
	ThisActor = Cast<IEnemyInterface>(CursorHit.GetActor());

	/**
	 * Line trace from cursor. There are several scenarios:
	 * A. LastActor is null and ThisActor is null
	 * - Do nothing
	 * B. LastActor is null and ThisActor is valid
	 * - Highlight ThisActor
	 * C. LastActor is valid && ThisActor is null
	 * - Unhighlight LastAcgtor
	 * D. Both actors are valid but LastActor != ThisActor
	 * - Unhighlight LastActor and Highlight ThisActor
	 * E. Both actors are valid and are the same actors
	 * - Do nothing
	 **/

	if (LastActor == nullptr)
	{
		if (ThisActor != nullptr)
		{
			//case B
			ThisActor->HighlightActor();
			/*if(GEngine)
				GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, TEXT("ThisActor->HighlightActor()"));*/	
		}
		else
		{
			//case A - both are null and do nothing
			/*if(GEngine)
				GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, TEXT("Case A"));*/
			
		}
	}
	else // LastActor is valid
	{
		if (ThisActor == nullptr)
		{
			//case C
			LastActor->UnHighlightActor();
			/*if(GEngine)
				GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, TEXT("LastActor->UnHighlightActor()"));*/
		}
		else //both actors are valid
		{
			if (LastActor != ThisActor)
			{
				//case B
				LastActor->UnHighlightActor();
				ThisActor->HighlightActor();

				/*if(GEngine)
					GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, TEXT("LastActor->UnHighlightActor()... ThisActor->HighlightActor()"));*/
				
			}
			else
			{
				// case E - do nothing
				/*if(GEngine)
					GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, TEXT("Case E"));*/
			}
		}
	}
}
