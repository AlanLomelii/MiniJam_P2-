// Fill out your copyright notice in the Description page of Project Settings.

#include "FinishLine.h"
#include "Components/BoxComponent.h"
#include "MiniJamOffroadCar.h"
#include "MiniJamPlayerController.h" // Necesitamos incluir tu controlador
#include "Engine/World.h"

AFinishLine::AFinishLine()
{
	bReplicates = true; // Importante para que exista en red

	FinishTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("FinishTrigger"));
	RootComponent = FinishTrigger;
	FinishTrigger->SetBoxExtent(FVector(500.f, 100.f, 200.f));
	FinishTrigger->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
}

void AFinishLine::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
	
	if (!HasAuthority() || bRaceEnded)
	{
		return;
	}
	
	if (OtherActor->IsA(AMiniJamOffroadCar::StaticClass()))
	{
		bRaceEnded = true;
		
		for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
		{
			AMiniJamPlayerController* PC = Cast<AMiniJamPlayerController>(Iterator->Get());
			if (PC)
			{
				if (PC->GetPawn() == OtherActor)
				{
					PC->Client_ShowGameOver(true);
				}
				else
				{
					PC->Client_ShowGameOver(false);
				}
			}
		}
        
		UE_LOG(LogTemp, Warning, TEXT("¡Carrera terminada! Ganador: %s"), *OtherActor->GetName());
	}
}
