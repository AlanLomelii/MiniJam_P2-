#include "BatterySpawner.h"
#include "Components/BoxComponent.h"
#include "BatteryItem.h"
#include "Kismet/KismetMathLibrary.h"

ABatterySpawner::ABatterySpawner()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	SpawnArea = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawnArea"));
	RootComponent = SpawnArea;
	SpawnArea->SetBoxExtent(FVector(500.f, 500.f, 100.f));
}

void ABatterySpawner::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority() && BatteryClass)
	{
		// Empezar a generar baterías cada cierto tiempo
		GetWorldTimerManager().SetTimer(SpawnTimer, this, &ABatterySpawner::SpawnBattery, SpawnInterval, true);
	}
}

void ABatterySpawner::SpawnBattery()
{
	if (!HasAuthority() || !BatteryClass)
		return;

	if (CurrentBatteryCount >= MaxBatteries)
		return;

	FVector SpawnLocation = GetRandomPointInArea();
	FRotator SpawnRotation = FRotator::ZeroRotator;

	ABatteryItem* NewBattery = GetWorld()->SpawnActor<ABatteryItem>(BatteryClass, SpawnLocation, SpawnRotation);
	if (NewBattery)
	{
		CurrentBatteryCount++;
		NewBattery->OnDestroyed.AddDynamic(this, &ABatterySpawner::OnBatteryDestroyed);
	}
}

void ABatterySpawner::OnBatteryDestroyed(AActor* DestroyedActor)
{
	CurrentBatteryCount = FMath::Max(0, CurrentBatteryCount - 1);
}


FVector ABatterySpawner::GetRandomPointInArea() const
{
	FVector Origin = SpawnArea->GetComponentLocation();
	FVector Extent = SpawnArea->GetScaledBoxExtent();
	return UKismetMathLibrary::RandomPointInBoundingBox(Origin, Extent);
}

void ABatterySpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
