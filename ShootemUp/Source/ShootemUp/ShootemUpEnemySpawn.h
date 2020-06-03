
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "ShootemUpEnemySpawn.generated.h"

UCLASS()
class AShootemUpEnemySpawn : public AActor
{
	GENERATED_BODY()

public:
	AShootemUpEnemySpawn();

	FORCEINLINE class UBoxComponent* GetSpawnInWorld() const { return SpawnInWorld; }

	UFUNCTION(BlueprintPure, Category = Spawning)
		FVector SetRandomOrigin();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditAnywhere, Category = Spawning)
		TSubclassOf<class AShootemUpEnemyAI> EnemyAIInstance;

	UPROPERTY(EditAnywhere, Category = Spawning)
		TSubclassOf<class AShootemUpEnemyCharacter> EnemyInstance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Spawning)
		float lowBound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Spawning)
		float highBound;

	FTimerHandle SpawnTimer;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Spawning, meta = (AllowPrivateAccess = "true"))
		class UBoxComponent* SpawnInWorld;

	float SpawnDelay;

	void Spawn();
};
