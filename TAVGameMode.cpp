// Copyright Epic Games, Inc. All Rights Reserved.

#include "TAVGameMode.h"
#include "TAVPlayerController.h"
#include "TAVCharacter.h"
#include "UObject/ConstructorHelpers.h"

ATAVGameMode::ATAVGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = ATAVPlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDownCPP/Blueprints/TopDownCharacter"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}