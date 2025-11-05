// Copyright Epic Games, Inc. All Rights Reserved.

#include "MiniJamGameMode.h"
#include "MiniJamPlayerController.h"

AMiniJamGameMode::AMiniJamGameMode()
{
	PlayerControllerClass = AMiniJamPlayerController::StaticClass();
}
