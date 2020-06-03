
#include "ShootemUpCharacter.h"
#include "ShootemUpProjectile.h"

#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Runtime/Engine/Classes/Components/DecalComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/DecalComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Actor.h"
#include "Materials/Material.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "Engine/Engine.h"
#include "Math/UnrealMathUtility.h"
#include "TimerManager.h"

AShootemUpCharacter::AShootemUpCharacter()
{
	// Set size for player capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Rotate character to moving direction
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	// Create a camera boom
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true); // Don't want arm to rotate when character does
	CameraBoom->TargetArmLength = 800.f;
	CameraBoom->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));
	CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

	// Create a camera
	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Create a decal in the world to show the cursor's location
	CursorToWorld = CreateDefaultSubobject<UDecalComponent>("CursorToWorld");
	CursorToWorld->SetupAttachment(RootComponent);
	static ConstructorHelpers::FObjectFinder<UMaterial> DecalMaterialAsset(TEXT("Material'/Game/Mannequin/CursorMaterial'"));
	if (DecalMaterialAsset.Succeeded())
	{
		CursorToWorld->SetDecalMaterial(DecalMaterialAsset.Object);
	}
	CursorToWorld->DecalSize = FVector(16.0f, 32.0f, 32.0f);
	CursorToWorld->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f).Quaternion());

	// Activate ticking in order to update the cursor every frame
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	DefaultHealth = 100;
	Health = DefaultHealth;
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Health : ") + FString::SanitizeFloat(Health));

	Score = 0;
}

// Called every frame on this actor
void AShootemUpCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (CursorToWorld != nullptr)
	{
		if (APlayerController* PC = Cast<APlayerController>(GetController()))
		{
			FHitResult TraceHitResult;
			PC->GetHitResultUnderCursor(ECC_Visibility, true, TraceHitResult);
			FVector CursorFV = TraceHitResult.ImpactNormal;
			FRotator CursorR = CursorFV.Rotation();
			CursorToWorld->SetWorldLocation(TraceHitResult.Location);
			CursorToWorld->SetWorldRotation(CursorR);
		}

		FRotator NewRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), CursorToWorld->GetComponentLocation());
		NewRotation.Pitch = 0.0f;
		NewRotation.Roll = 0.0f;
		SetActorRotation(NewRotation);
	}
}

void AShootemUpCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &AShootemUpCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AShootemUpCharacter::MoveRight);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AShootemUpCharacter::OnStartJump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &AShootemUpCharacter::OnStopJump);

	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &AShootemUpCharacter::OnStartCrouch);
	PlayerInputComponent->BindAction("Crouch", IE_Released, this, &AShootemUpCharacter::OnStopCrouch);

	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AShootemUpCharacter::OnFire);

	PlayerInputComponent->BindAction("Death", IE_Pressed, this, &AShootemUpCharacter::OnDeath);
}

void AShootemUpCharacter::MoveForward(float value)
{
	if (Controller && (value != 0.0f))
	{
		// Find out which way is forward
		FRotator Rotation = Controller->GetControlRotation();

		// Limit pitch when walking or falling
		if (GetCharacterMovement()->IsMovingOnGround() || GetCharacterMovement()->IsFalling())
		{
			Rotation.Pitch = 0.0f;
		}

		// Add movement in that direction
		const FVector Direction = FRotationMatrix(Rotation).GetScaledAxis(EAxis::X);
		AddMovementInput(Direction, value);
	}
}

void AShootemUpCharacter::MoveRight(float value)
{
	if (Controller && (value != 0.0f))
	{
		// Find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FVector Direction = FRotationMatrix(Rotation).GetScaledAxis(EAxis::Y);

		// Add movement in that direction
		AddMovementInput(Direction, value);
	}
}

void AShootemUpCharacter::OnStartJump()
{
	bPressedJump = true;
}

void AShootemUpCharacter::OnStopJump()
{
	bPressedJump = false;
}

void AShootemUpCharacter::OnStartCrouch()
{
	float NewEyeHeight = BaseEyeHeight / 4;

	bIsCrouched = true;
	CrouchedEyeHeight = NewEyeHeight;
	RecalculateBaseEyeHeight();
	//GetCharacterMovement()->GetNavAgentPropertiesRef().bCanCrouch = true;
	//if (CanCrouch() == true)
	//{
	//Crouch();
	//}
}

void AShootemUpCharacter::OnStopCrouch()
{
	bIsCrouched = false;
	RecalculateBaseEyeHeight();
	//GetCharacterMovement()->GetNavAgentPropertiesRef().bCanCrouch = true;
	//if (CanCrouch() == false)
	//{
	//UnCrouch();
	//}
}

void AShootemUpCharacter::OnFire()
{
	// Try and fire a projectile
	if (ProjectileClass != NULL)
	{
		auto proj = ProjectileClass.GetDefaultObject()->MovementComponent;
		float dist = FVector::Dist(GetActorLocation(), CursorToWorld->GetComponentLocation());
		proj->InitialSpeed = dist * 4.0f;
		proj->MaxSpeed = dist * 4.0f;

		UWorld* const World = GetWorld();
		if (World)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = Instigator;

			FRotator NewRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), CursorToWorld->GetComponentLocation());

			AShootemUpProjectile* const Projectile = World->SpawnActor<AShootemUpProjectile>(ProjectileClass, GetActorLocation(), NewRotation, SpawnParams);
			if (Projectile)
			{
				// Find launch direction
				FVector const LaunchDir = NewRotation.Vector();
				Projectile->InitVelocity(LaunchDir);
			}
		}
	}
}

void AShootemUpCharacter::OnDeath()
{
	Destroy();
	UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()), false);
}

void AShootemUpCharacter::TakeDamage()
{
	Health -= 10;

	if (Health <= 0)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("GAME OVER !"));
		OnDeath();
	}

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Health : ") + FString::SanitizeFloat(Health));
}

void AShootemUpCharacter::GainPoints() 
{

}
