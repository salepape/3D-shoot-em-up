
#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"

#include "BTService_FindPlayer.generated.h"

UCLASS()
class UBTService_FindPlayer : public UBTService
{
	GENERATED_BODY()

	UBTService_FindPlayer();

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};