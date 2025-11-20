// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "MiniJamPawn.h"
#include "MiniJamOffroadCar.generated.h"

/**
 *  Offroad car wheeled vehicle implementation
 */
UCLASS(abstract)
class MINIJAM_API AMiniJamOffroadCar : public AMiniJamPawn
{
	GENERATED_BODY()
	
	/** Chassis static mesh */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Meshes, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* Chassis;

	/** FL Tire static mesh */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Meshes, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* TireFrontLeft;

	/** FR Tire static mesh */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Meshes, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* TireFrontRight;

	/** RL Tire static mesh */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Meshes, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* TireRearLeft;

	/** RR Tire static mesh */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Meshes, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* TireRearRight;

public:

	AMiniJamOffroadCar();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

protected:

	// energy
	UPROPERTY(EditAnywhere, Replicated, BlueprintReadWrite, Category = "Vehicle Stats")
	float CurrentEnergy;

	UFUNCTION()
	void OnRep_CurrentEnergy();
	
	UPROPERTY()
	class UMiniJamUI* VehicleHUD;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vehicle Stats")
	float MaxEnergy = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vehicle Stats")
	float BaseEnergyDrainRate = 1.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vehicle Stats")
	float MovingEnergyDrainRate = 0.9f;

	

public:

	
	UFUNCTION(BlueprintCallable, Category = "Vehicle Stats")
	void AddEnergy(float Amount);
	
	UFUNCTION(BlueprintCallable, Category = "Vehicle Stats")
	float GetCurrentEnergy() const { return CurrentEnergy; }

	UFUNCTION(BlueprintCallable, Category = "Vehicle Stats")
	float GetMaxEnergy() const { return MaxEnergy; }

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Vehicle Stats")
	bool bCanMove = true;

	UFUNCTION(BlueprintCallable, Category = "UI")
	FText GetBatteryStatusText() const;
};
