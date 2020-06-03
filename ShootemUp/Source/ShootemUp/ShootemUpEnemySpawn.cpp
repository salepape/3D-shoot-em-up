
#include "ShootemUpEnemySpawn.h"
#include "ShootemUpEnemyCharacter.h"
#include "ShootemUpEnemyAI.h"

#include "TimerManager.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetMathLibrary.h"

AShootemUpEnemySpawn::AShootemUpEnemySpawn()
{
	// Set this actor to call Tick() every frame
	PrimaryActorTick.bCanEverTick = true;

	// Declare spawning box
	SpawnInWorld = CreateDefaultSubobject<UBoxComponent>(TEXT("Spawn In World"));
	RootComponent = SpawnInWorld;

	lowBound = 0.1f;
	highBound = 0.4f;
}

void AShootemUpEnemySpawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AShootemUpEnemySpawn::BeginPlay()
{
	Spawn();
}

FVector AShootemUpEnemySpawn::SetRandomOrigin()
{
	FVector SpawnOrigin = SpawnInWorld->Bounds.Origin;
	FVector SpawnExtent = SpawnInWorld->Bounds.BoxExtent;

	return UKismetMathLibrary::RandomPointInBoundingBox(SpawnOrigin, SpawnExtent);
}

void AShootemUpEnemySpawn::Spawn()
{
	if (EnemyInstance && EnemyAIInstance)
	{
		UWorld* World = GetWorld();
		if (World)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = Instigator;

			FVector SpawnLocation = SetRandomOrigin();
			FRotator SpawnRotation;
			AShootemUpEnemyAI* const SpawnedEnemyAI = World->SpawnActor<AShootemUpEnemyAI>(EnemyAIInstance, SpawnLocation, SpawnRotation, SpawnParams);
			SpawnedEnemyAI->bAllowStrafe = true;
			AShootemUpEnemyCharacter* const SpawnedEnemy = World->SpawnActor<AShootemUpEnemyCharacter>(EnemyInstance, SpawnLocation, SpawnRotation, SpawnParams);

			SpawnDelay = FMath::FRandRange(lowBound, highBound);
			GetWorldTimerManager().SetTimer(SpawnTimer, this, &AShootemUpEnemySpawn::Spawn, SpawnDelay, false);
		}
	}
}

