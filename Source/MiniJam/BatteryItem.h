#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BatteryItem.generated.h"

UCLASS()
class MINIJAM_API ABatteryItem : public AActor
{
	GENERATED_BODY()

public:	
	ABatteryItem();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category="Components")
	UStaticMeshComponent* MeshComp;

	UPROPERTY(VisibleAnywhere, Category="Components")
	class USphereComponent* CollisionComp;

	UPROPERTY(EditAnywhere, Replicated, Category="Battery")
	float EnergyValue = 25.f;

public:	
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
};
