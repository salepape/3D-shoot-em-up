
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

#include "ShootemUpProjectile.generated.h"

UCLASS(Blueprintable)
class AShootemUpProjectile : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AShootemUpProjectile(const FObjectInitializer& ObjectInitializer);

	/** inits velocity of the projectile in the shoot direction */
	void InitVelocity(const FVector& ShootDirection);

	/** Projectile movement component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement)
		UProjectileMovementComponent* ProjectileMovement;

	/** Sphere collision component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Projectile)
		USphereComponent* CollisionComponent;
};