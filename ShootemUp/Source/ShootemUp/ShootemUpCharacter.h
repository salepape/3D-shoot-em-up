
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "ShootemUpCharacter.generated.h"

UCLASS(Blueprintable)
class AShootemUpCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AShootemUpCharacter();

	// Called every frame.
	virtual void Tick(float DeltaSeconds) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/** Returns TopDownCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetTopDownCameraComponent() const { return TopDownCameraComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns CursorToWorld subobject **/
	FORCEINLINE class UDecalComponent* GetCursorToWorld() { return CursorToWorld; }

	//handles firing
	UFUNCTION()
		void OnFire();

	/** Projectile class to spawn */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Projectile)
		TSubclassOf<class AShootemUpProjectile> ProjectileClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Health)
		float DefaultHealth;

	UPROPERTY(BlueprintReadOnly)
		float Health;

	UFUNCTION()
		void TakeDamage();

	UFUNCTION()
		void OnDeath();

	int Score;

	UFUNCTION()
		void GainPoints();

private:
	/** Top down camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* TopDownCameraComponent;

	/** Camera boom positioning the camera above the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* CameraBoom;

	/** A decal that projects to the cursor location. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UDecalComponent* CursorToWorld;

protected:
	/** True if the controlled character should navigate to the mouse cursor. */
	uint32 bMoveToMouseCursor : 1;

	//go left / right or up / down when A / D or Z / S keyboard are pressed
	UFUNCTION()
		void MoveRight(float value);
	UFUNCTION()
		void MoveForward(float value);

	//sets or clears jump flag when space keyboard is pressed
	UFUNCTION()
		void OnStartJump();
	UFUNCTION()
		void OnStopJump();

	//sets or clears crouch flag when C keyboard is pressed
	UFUNCTION()
		void OnStartCrouch();
	UFUNCTION()
		void OnStopCrouch();
};

