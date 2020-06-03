
#include "BTTask_MoveToPlayer.h"
#include "ShootemUpEnemyAI.h"
#include "ShootemUpEnemyCharacter.h"
#include "ShootemUpCharacter.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"

EBTNodeResult::Type UBTTask_MoveToPlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AShootemUpEnemyAI* EnemyAI = Cast<AShootemUpEnemyAI>(OwnerComp.GetAIOwner());

	AShootemUpCharacter* EnemyPtr = Cast<AShootemUpCharacter>(OwnerComp.GetBlackboardComponent()->GetValue<UBlackboardKeyType_Object>(EnemyAI->EnemyKeyID));

	if (EnemyPtr)
	{
		EnemyAI->MoveToActor(EnemyPtr, 5.f, true, true, true, 0, true);
		return EBTNodeResult::Succeeded;
	}
	else
	{
		return EBTNodeResult::Failed;
	}

	EBTNodeResult::Failed;
}
