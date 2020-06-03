
#pragma once

#include "CoreMinimal.h"
#include "AIController.h"

#include "ShootemUpEnemyAI.generated.h"

UCLASS()
class AShootemUpEnemyAI : public AAIController
{
	GENERATED_BODY()

	UPROPERTY(transient)
		class UBlackboardComponent* BBComponent;

	UPROPERTY(transient)
		class UBehaviorTreeComponent* BTComponent;

public:
	AShootemUpEnemyAI();

	virtual void OnPossess(APawn* InPawn) override;

	uint8 EnemyKeyID;
};
