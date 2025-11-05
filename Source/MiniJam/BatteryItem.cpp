#include "BatteryItem.h"
#include "Components/SphereComponent.h"
#include "Net/UnrealNetwork.h"

ABatteryItem::ABatteryItem()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComp"));
	RootComponent = CollisionComp;
	CollisionComp->InitSphereRadius(50.f);
	CollisionComp->SetCollisionProfileName(TEXT("OverlapAllDynamic"));

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(RootComponent);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereMesh(TEXT("/Engine/BasicShapes/Sphere"));
	if (SphereMesh.Succeeded())
	{
		MeshComp->SetStaticMesh(SphereMesh.Object);
	}

	
	EnergyValue = 25.f;
}

void ABatteryItem::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABatteryItem::NotifyActorBeginOverlap(AActor* OtherActor)
{
	if (HasAuthority())
	{
		Destroy();
	}
}

#include "Net/UnrealNetwork.h"

void ABatteryItem::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
}


