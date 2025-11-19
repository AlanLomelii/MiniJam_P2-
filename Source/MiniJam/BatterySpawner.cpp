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

	
	FActorSpawnParameters SpawnParams;
	
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	ABatteryItem* NewBattery = GetWorld()->SpawnActor<ABatteryItem>(BatteryClass, SpawnLocation, SpawnRotation, SpawnParams);
    
	if (NewBattery)
	{
		CurrentBatteryCount++;
		NewBattery->OnDestroyed.AddDynamic(this, &ABatterySpawner::OnBatteryDestroyed);
		
		UE_LOG(LogTemp, Warning, TEXT("Bateria spawneada en: %s"), *SpawnLocation.ToString());
	}
	else
	{
		
		UE_LOG(LogTemp, Error, TEXT("Fallo al spawnear bateria."));
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
