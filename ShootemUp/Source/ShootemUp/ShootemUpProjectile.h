
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
	AShootemUpProjectile(const FObjectInitializer& ObjectInitializer);

	virtual void Tick(float DeltaTime) override;

	// Init velocity of the projectile in the shoot direction
	void InitVelocity(const FVector& ShootDirection);

	UFUNCTION()
		void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, 
			FVector NormalImpulse, const FHitResult& Hit);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement)
		UProjectileMovementComponent* MovementComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Projectile)
		USphereComponent* CollisionComponent;

protected:
	virtual void BeginPlay() override;
};