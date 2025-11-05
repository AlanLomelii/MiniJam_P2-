// Copyright Epic Games, Inc. All Rights Reserved.

#include "MiniJamWheelRear.h"
#include "UObject/ConstructorHelpers.h"

UMiniJamWheelRear::UMiniJamWheelRear()
{
	AxleType = EAxleType::Rear;
	bAffectedByHandbrake = true;
	bAffectedByEngine = true;
}