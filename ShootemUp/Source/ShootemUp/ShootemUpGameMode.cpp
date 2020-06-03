
#include "ShootemUpGameMode.h"
#include "ShootemUpCharacter.h"
#include "UObject/ConstructorHelpers.h"

AShootemUpGameMode::AShootemUpGameMode()
{
	// Set default pawn class to BP_Mannequin blueprint
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Mannequin/BP_Mannequin"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}