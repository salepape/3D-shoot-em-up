//#include "ShootemUp.h"
#include "BTService_FindPlayer.h"
#include "ShootemUpCharacter.h"
#include "ShootemUpEnemyAI.h"
#include "ShootemUpEnemyCharacter.h"

#include "Engine/World.h"
//#include "Engine/Engine.h"
#include "GameFramework/PlayerController.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"

UBTService_FindPlayer::UBTService_FindPlayer()
{
	bCreateNodeInstance = true;
}

void UBTService_FindPlayer::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	AShootemUpEnemyAI* EnemyAI = Cast<AShootemUpEnemyAI>(OwnerComp.GetAIOwner());

	if (EnemyAI)
	{
		AShootemUpCharacter* EnemyPtr = Cast<AShootemUpCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());

		if (EnemyPtr)
		{
			OwnerComp.GetBlackboardComponent()->SetValue<UBlackboardKeyType_Object>(EnemyAI->EnemyKeyID, EnemyPtr);
		}
	}
}