// Copyright Epic Games, Inc. All Rights Reserved.


#include "MiniJamOffroadCar.h"
#include "MiniJamOffroadWheelFront.h"
#include "MiniJamOffroadWheelRear.h"
#include "ChaosWheeledVehicleMovementComponent.h"
#include "MiniJamUI.h"
#include "GameFramework/SpringArmComponent.h"
#include "Net/UnrealNetwork.h"

AMiniJamOffroadCar::AMiniJamOffroadCar()
{
	PrimaryActorTick.bCanEverTick = true;
	// construct the mesh components
	Chassis = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Chassis"));
	Chassis->SetupAttachment(GetMesh());

	// NOTE: tire sockets are set from the Blueprint class
	TireFrontLeft = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Tire Front Left"));
	TireFrontLeft->SetupAttachment(GetMesh(), FName("VisWheel_FL"));
	TireFrontLeft->SetCollisionProfileName(FName("NoCollision"));

	TireFrontRight = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Tire Front Right"));
	TireFrontRight->SetupAttachment(GetMesh(), FName("VisWheel_FR"));
	TireFrontRight->SetCollisionProfileName(FName("NoCollision"));
	TireFrontRight->SetRelativeRotation(FRotator(0.0f, 180.0f, 0.0f));

	TireRearLeft = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Tire Rear Left"));
	TireRearLeft->SetupAttachment(GetMesh(), FName("VisWheel_BL"));
	TireRearLeft->SetCollisionProfileName(FName("NoCollision"));

	TireRearRight = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Tire Rear Right"));
	TireRearRight->SetupAttachment(GetMesh(), FName("VisWheel_BR"));
	TireRearRight->SetCollisionProfileName(FName("NoCollision"));
	TireRearRight->SetRelativeRotation(FRotator(0.0f, 180.0f, 0.0f));

	// adjust the cameras
	GetFrontSpringArm()->SetRelativeLocation(FVector(-5.0f, -30.0f, 135.0f));
	GetBackSpringArm()->SetRelativeLocation(FVector(0.0f, 0.0f, 75.0f));

	// Note: for faster iteration times, the vehicle setup can be tweaked in the Blueprint instead

	// Set up the chassis
	GetChaosVehicleMovement()->ChassisHeight = 160.0f;
	GetChaosVehicleMovement()->DragCoefficient = 0.1f;
	GetChaosVehicleMovement()->DownforceCoefficient = 0.1f;
	GetChaosVehicleMovement()->CenterOfMassOverride = FVector(0.0f, 0.0f, 75.0f);
	GetChaosVehicleMovement()->bEnableCenterOfMassOverride = true;

	// Set up the wheels
	GetChaosVehicleMovement()->bLegacyWheelFrictionPosition = true;
	GetChaosVehicleMovement()->WheelSetups.SetNum(4);

	GetChaosVehicleMovement()->WheelSetups[0].WheelClass = UMiniJamOffroadWheelFront::StaticClass();
	GetChaosVehicleMovement()->WheelSetups[0].BoneName = FName("PhysWheel_FL");
	GetChaosVehicleMovement()->WheelSetups[0].AdditionalOffset = FVector(0.0f, 0.0f, 0.0f);

	GetChaosVehicleMovement()->WheelSetups[1].WheelClass = UMiniJamOffroadWheelFront::StaticClass();
	GetChaosVehicleMovement()->WheelSetups[1].BoneName = FName("PhysWheel_FR");
	GetChaosVehicleMovement()->WheelSetups[1].AdditionalOffset = FVector(0.0f, 0.0f, 0.0f);

	GetChaosVehicleMovement()->WheelSetups[2].WheelClass = UMiniJamOffroadWheelRear::StaticClass();
	GetChaosVehicleMovement()->WheelSetups[2].BoneName = FName("PhysWheel_BL");
	GetChaosVehicleMovement()->WheelSetups[2].AdditionalOffset = FVector(0.0f, 0.0f, 0.0f);

	GetChaosVehicleMovement()->WheelSetups[3].WheelClass = UMiniJamOffroadWheelRear::StaticClass();
	GetChaosVehicleMovement()->WheelSetups[3].BoneName = FName("PhysWheel_BR");
	GetChaosVehicleMovement()->WheelSetups[3].AdditionalOffset = FVector(0.0f, 0.0f, 0.0f);

	// Set up the engine
	// NOTE: Check the Blueprint asset for the Torque Curve
	GetChaosVehicleMovement()->EngineSetup.MaxTorque = 600.0f;
	GetChaosVehicleMovement()->EngineSetup.MaxRPM = 5000.0f;
	GetChaosVehicleMovement()->EngineSetup.EngineIdleRPM = 1200.0f;
	GetChaosVehicleMovement()->EngineSetup.EngineBrakeEffect = 0.05f;
	GetChaosVehicleMovement()->EngineSetup.EngineRevUpMOI = 5.0f;
	GetChaosVehicleMovement()->EngineSetup.EngineRevDownRate = 600.0f;

	// Set up the differential
	GetChaosVehicleMovement()->DifferentialSetup.DifferentialType = EVehicleDifferential::AllWheelDrive;
	GetChaosVehicleMovement()->DifferentialSetup.FrontRearSplit = 0.5f;

	// Set up the steering
	// NOTE: Check the Blueprint asset for the Steering Curve
	GetChaosVehicleMovement()->SteeringSetup.SteeringType = ESteeringType::AngleRatio;
	GetChaosVehicleMovement()->SteeringSetup.AngleRatio = 0.7f;

	bReplicates = true;

	if (UChaosWheeledVehicleMovementComponent* MovementComp = GetChaosVehicleMovement())
	{
		MovementComp->SetIsReplicated(true); 
		
		MovementComp->SetComponentTickEnabled(true); 
	}

	CurrentEnergy = MaxEnergy;
}


void AMiniJamOffroadCar::BeginPlay()
{
	Super::BeginPlay();
	
	OnRep_CurrentEnergy();
}


void AMiniJamOffroadCar::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(AMiniJamOffroadCar, CurrentEnergy);

	DOREPLIFETIME(AMiniJamOffroadCar, bCanMove);
}


void AMiniJamOffroadCar::AddEnergy(float Amount)
{
	CurrentEnergy = FMath::Clamp(CurrentEnergy + Amount, 0.0f, MaxEnergy);
	
	if (HasAuthority())
	{
		GetChaosVehicleMovement()->SetBrakeInput(0.0f); 
	}
}


void AMiniJamOffroadCar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
    
	if (HasAuthority()) 
	{
		UChaosWheeledVehicleMovementComponent* MovementComp = GetChaosVehicleMovement();
		if (!MovementComp) return; 
       
		float CurrentSpeed = MovementComp->GetForwardSpeed();
       
		if (CurrentEnergy > 0.0f)
		{
			float TotalDrainRate = BaseEnergyDrainRate;
			
			if (FMath::Abs(CurrentSpeed) > 1.0f) 
			{
				TotalDrainRate += MovingEnergyDrainRate;
			}
          
			CurrentEnergy = FMath::Max(0.0f, CurrentEnergy - TotalDrainRate * DeltaTime);
			
			MovementComp->SetBrakeInput(0.0f);
			MovementComp->SetHandbrakeInput(false); 
          
			bCanMove = true; 
		}
		else 
		{
          
			bCanMove = false;
			
			if (FMath::Abs(CurrentSpeed) > 10.0f) 
			{
				MovementComp->SetThrottleInput(0.0f);   
				MovementComp->SetBrakeInput(0.0f);      
				MovementComp->SetHandbrakeInput(false); 
			}
			
			else 
			{
				MovementComp->SetThrottleInput(0.0f); 
				MovementComp->SetBrakeInput(1.0f);    
				MovementComp->SetHandbrakeInput(true); 
			}
		}
	}
}

void AMiniJamOffroadCar::OnRep_CurrentEnergy()
{
	// 1. Lógica para determinar el texto de estado
	float EnergyPercent = CurrentEnergy / MaxEnergy;
	FText StatusText;
    
	if (EnergyPercent <= 0.0f)
	{
		StatusText = FText::FromString(TEXT("SIN BATERÍA"));
	}
	else if (EnergyPercent <= 0.20f)
	{
		StatusText = FText::FromString(TEXT("BATERÍA BAJA"));
	}
	else
	{
		StatusText = FText::GetEmpty();
	}

	// 2. Comunicar los dos estados a la UI
	if (VehicleHUD)
	{
		// 1. Actualiza el medidor/barra de energía (usando la función existente)
		VehicleHUD->UpdateEnergy(CurrentEnergy, MaxEnergy); 

		// 2. Actualiza el texto de alerta (usando la función NUEVA)
		VehicleHUD->OnBatteryStatusUpdate(StatusText); 
	}
}

FText AMiniJamOffroadCar::GetBatteryStatusText() const
{
	float EnergyPercent = 0.0f;
	if (MaxEnergy > 0.0f)
	{
		EnergyPercent = CurrentEnergy / MaxEnergy;
	}

	if (EnergyPercent <= 0.0f)
	{
		return FText::FromString(TEXT("SIN BATERÍA"));
	}
	else if (EnergyPercent <= 0.20f)
	{
		return FText::FromString(TEXT("BATERÍA BAJA"));
	}
    
	// Retorna texto vacío si la energía está bien
	return FText::GetEmpty(); 
}