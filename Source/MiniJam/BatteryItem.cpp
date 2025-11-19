#include "BatteryItem.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Net/UnrealNetwork.h"
#include "MiniJamOffroadCar.h"

ABatteryItem::ABatteryItem()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	// Colision principal
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComp"));
	RootComponent = CollisionComp;
	CollisionComp->InitSphereRadius(50.f);
	CollisionComp->SetCollisionProfileName(TEXT("OverlapOnlyPawn"));

	// Mesh visual
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(RootComponent);
	
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// Valor que dara al jugador
	EnergyValue = 25.f;
}


void ABatteryItem::BeginPlay()
{
	Super::BeginPlay();
}

void ABatteryItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	
	FRotator Rot(0.f, RotationSpeed * DeltaTime, 0.f);
	AddActorLocalRotation(Rot);
}

void ABatteryItem::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	if (!HasAuthority())
		return;

	if (!OtherActor || OtherActor == this)
		return;

	
	AMiniJamOffroadCar* MyCar = Cast<AMiniJamOffroadCar>(OtherActor);

	if (MyCar)
	{
		MyCar->AddEnergy(EnergyValue);
		Destroy();
	}
}

void ABatteryItem::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	
	DOREPLIFETIME(ABatteryItem, EnergyValue);
}

