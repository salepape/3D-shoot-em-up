
#include "ShootemUpEnemyCharacter.h"
#include "ShootemUpCharacter.h"

#include "GameFramework/Actor.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/DamageType.h"
#include "Engine/Engine.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"

AShootemUpEnemyCharacter::AShootemUpEnemyCharacter()
{
	// Set this character to call Tick() every frame
	PrimaryActorTick.bCanEverTick = true;

	// Declare trigger capsule
	TriggerCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Trigger Capsule"));
	TriggerCapsule->InitCapsuleSize(55.f, 96.0f);;
	TriggerCapsule->SetCollisionProfileName(TEXT("Trigger"));
	TriggerCapsule->SetupAttachment(RootComponent);

	GetCharacterMovement()->MaxWalkSpeed = 300.0f;
	DamageDelay = 1.0f;
}

void AShootemUpEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Declare overlap events
	TriggerCapsule->OnComponentBeginOverlap.AddDynamic(this, &AShootemUpEnemyCharacter::OnOverlapBegin);
	TriggerCapsule->OnComponentEndOverlap.AddDynamic(this, &AShootemUpEnemyCharacter::OnOverlapEnd);
}

// Called to bind functionality to input
void AShootemUpEnemyCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AShootemUpEnemyCharacter::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
	class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AShootemUpCharacter* playerCharacter = dynamic_cast<AShootemUpCharacter*>(OtherActor);

	if (OtherActor && OtherActor != this && playerCharacter && OtherComp && GetWorld())
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Overlap Begin"));

		GetWorldTimerManager().SetTimer(DamageTimer, playerCharacter, &AShootemUpCharacter::TakeDamage, DamageDelay, true);
	}
}

void AShootemUpEnemyCharacter::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
	class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && OtherActor != this && OtherComp && GetWorld())
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Overlap End"));

		GetWorldTimerManager().ClearTimer(DamageTimer);
	}
}
