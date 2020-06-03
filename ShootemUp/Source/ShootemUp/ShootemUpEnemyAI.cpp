
#include "ShootemUpEnemyAI.h"
#include "ShootemUpEnemyCharacter.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"


AShootemUpEnemyAI::AShootemUpEnemyAI()
{
	BBComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("Blackboard Component"));
	BTComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("Behavior Tree Component"));
}

void AShootemUpEnemyAI::OnPossess(APawn * InPawn)
{
	Super::OnPossess(InPawn);

	AShootemUpEnemyCharacter* CharacterPtr = Cast<AShootemUpEnemyCharacter>(InPawn);

	if (CharacterPtr && CharacterPtr->AIBehaviour)
	{
		BBComponent->InitializeBlackboard(*CharacterPtr->AIBehaviour->BlackboardAsset);
		EnemyKeyID = BBComponent->GetKeyID("Target");
		BTComponent->StartTree(*CharacterPtr->AIBehaviour);
	}
}
