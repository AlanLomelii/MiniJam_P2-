// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BatterySpawner.generated.h"

UCLASS()
class MINIJAM_API ABatterySpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	ABatterySpawner();

protected:
	virtual void BeginPlay() override;

	
	UPROPERTY(VisibleAnywhere, Category="Components")
	class UBoxComponent* SpawnArea;
	
	UPROPERTY(EditAnywhere, Category="Spawner")
	TSubclassOf<class ABatteryItem> BatteryClass;

	UPROPERTY(EditAnywhere, Category="Spawner")
	float SpawnInterval = 5.f; 

	UPROPERTY(EditAnywhere, Category="Spawner")
	int32 MaxBatteries = 10;

	UPROPERTY(EditAnywhere, Category="Spawner")
	int32 CurrentBatteryCount = 0;

	FTimerHandle SpawnTimer;

	
	FVector GetRandomPointInArea() const;

	UFUNCTION()
	void SpawnBattery();

	UFUNCTION()
	void OnBatteryDestroyed(AActor* DestroyedActor);


public:	
	virtual void Tick(float DeltaTime) override;
};
