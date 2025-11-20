// Copyright Epic Games, Inc. All Rights Reserved.


#include "MiniJamPlayerController.h"
#include "MiniJamPawn.h"
#include "MiniJamUI.h"
#include "EnhancedInputSubsystems.h"
#include "ChaosWheeledVehicleMovementComponent.h"
#include "MiniJamOffroadCar.h"

void AMiniJamPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	if (VehicleUIClass && IsLocalPlayerController())
	{
		VehicleUI = CreateWidget<UMiniJamUI>(this, VehicleUIClass);
		
		if (VehicleUI)
		{
			VehicleUI->AddToViewport();
		}
	}
}

void AMiniJamPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	
	// get the enhanced input subsystem
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		// add the mapping context so we get controls
		Subsystem->AddMappingContext(InputMappingContext, 0);

		// optionally add the steering wheel context
		if (bUseSteeringWheelControls && SteeringWheelInputMappingContext)
		{
			Subsystem->AddMappingContext(SteeringWheelInputMappingContext, 1);
		}
	}
}

void AMiniJamPlayerController::Tick(float Delta)
{
	Super::Tick(Delta);
	if (VehiclePawn == nullptr)
	{
		VehiclePawn = Cast<AMiniJamPawn>(GetPawn());
	}

	if (IsValid(VehiclePawn) && IsValid(VehicleUI))
	{
		VehicleUI->UpdateSpeed(VehiclePawn->GetChaosVehicleMovement()->GetForwardSpeed());
		VehicleUI->UpdateGear(VehiclePawn->GetChaosVehicleMovement()->GetCurrentGear());
		
		AMiniJamOffroadCar* MyCar = Cast<AMiniJamOffroadCar>(VehiclePawn);
        
		if (MyCar)
		{
			VehicleUI->UpdateEnergy(MyCar->GetCurrentEnergy(), MyCar->GetMaxEnergy());
			
			VehicleUI->OnBatteryStatusUpdate(MyCar->GetBatteryStatusText());
		}
	}
}

void AMiniJamPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	// get a pointer to the controlled pawn
	VehiclePawn = CastChecked<AMiniJamPawn>(InPawn);
}

void AMiniJamPlayerController::Client_ShowGameOver_Implementation(bool bIsWinner)
{
	if (VehicleUI)
	{
		
		VehicleUI->OnGameOver(bIsWinner);
	}
}
