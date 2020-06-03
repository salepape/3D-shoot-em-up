
#include "ShootemUpProjectile.h"
#include "ShootemUpCharacter.h"
#include "ShootemUpEnemyCharacter.h"

#include "Engine/Engine.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "DrawDebugHelpers.h"

void AShootemUpProjectile::BeginPlay()
{
	Super::BeginPlay();
	CollisionComponent->OnComponentHit.AddDynamic(this, &AShootemUpProjectile::OnHit);
}

AShootemUpProjectile::AShootemUpProjectile(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;

	// Use a sphere as a simple collision representation
	CollisionComponent = ObjectInitializer.CreateDefaultSubobject<USphereComponent>(this, TEXT("SphereComponent"));
	CollisionComponent->BodyInstance.SetCollisionProfileName(TEXT("Projectile"));
	CollisionComponent->InitSphereRadius(15.0f);
	RootComponent = CollisionComponent;

	// Use a ProjectileMovementComponent to govern this projectile's movement
	MovementComponent = ObjectInitializer.CreateDefaultSubobject<UProjectileMovementComponent>(this, TEXT("MovementComponent"));
	MovementComponent->UpdatedComponent = CollisionComponent;
	MovementComponent->InitialSpeed = 1500.f;
	MovementComponent->MaxSpeed = 1500.f;
	MovementComponent->bRotationFollowsVelocity = true;
	MovementComponent->bShouldBounce = true;
	MovementComponent->Bounciness = 0.3f;

	InitialLifeSpan = 3.0f;
}

void AShootemUpProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AShootemUpProjectile::InitVelocity(const FVector& ShootDirection)
{
	if (MovementComponent)
	{
		// Set the projectile's velocity to the desired direction
		MovementComponent->Velocity = ShootDirection * MovementComponent->InitialSpeed;
	}
}

void AShootemUpProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent,
	FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor != nullptr && OtherActor != this && OtherComponent != nullptr)
	{
		AShootemUpEnemyCharacter* enemy = dynamic_cast<AShootemUpEnemyCharacter*>(OtherActor);
		if (OtherComponent->IsSimulatingPhysics() || enemy)
		{
			if (OtherComponent->IsSimulatingPhysics())
			{
				// Add movement to object targeted by the shot
				OtherComponent->AddImpulseAtLocation(MovementComponent->Velocity * 100.0f, Hit.ImpactPoint);
			}

			FVector Scale = OtherComponent->GetComponentScale();
			Scale *= 0.8f;

			if (Scale.GetMin() < 0.5f)
			{
				AShootemUpCharacter* playerCharacter = dynamic_cast<AShootemUpCharacter*>(OtherActor);
				if (playerCharacter)
				{
					if (OtherComponent->IsSimulatingPhysics())
						playerCharacter->Score += 50;
					else if (enemy)
						playerCharacter->Score += 200;
					GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Score : ") + FString::FromInt(playerCharacter->Score));
				}

				OtherActor->Destroy();
			}
			else
			{
				OtherComponent->SetWorldScale3D(Scale);
			}

			UMaterialInstanceDynamic* MaterialInstance = OtherComponent->CreateAndSetMaterialInstanceDynamic(0);
			if (MaterialInstance)
			{
				MaterialInstance->SetVectorParameterValue("Color", FLinearColor::MakeRandomColor());
			}

			Destroy();
		}
	}
}




