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
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

	//components

	UPROPERTY(VisibleAnywhere, Category="Components")
	UStaticMeshComponent* MeshComp;

	UPROPERTY(VisibleAnywhere, Category="Components")
	class USphereComponent* CollisionComp;

	
	//energy value
	UPROPERTY(EditAnywhere, Replicated, Category="Battery")
	float EnergyValue;

	//rotation
	UPROPERTY(EditAnywhere, Category="Battery|Effects")
	float RotationSpeed = 60.0f;

public:	
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;
	
};
