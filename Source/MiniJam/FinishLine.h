// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FinishLine.generated.h"

UCLASS()
class MINIJAM_API AFinishLine : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AFinishLine();

protected:
	// Caja de colisión para detectar el coche
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UBoxComponent* FinishTrigger;

	// Variable para asegurar que solo se gana una vez
	bool bRaceEnded = false;

public:	
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
};
