
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "ShootemUpEnemyCharacter.generated.h"

UCLASS()
class AShootemUpEnemyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Set default values for this character's properties
	AShootemUpEnemyCharacter();

	void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = Behaviour)
		class UBehaviorTree* AIBehaviour;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Sphere collision component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Trigger Capsule")
		UCapsuleComponent* TriggerCapsule;

	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
			class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// declare overlap end function
	UFUNCTION()
		void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
			class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

private:
	float DamageDelay;

protected:
	FTimerHandle DamageTimer;
};
