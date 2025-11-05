// Copyright Epic Games, Inc. All Rights Reserved.

#include "MiniJamWheelFront.h"
#include "UObject/ConstructorHelpers.h"

UMiniJamWheelFront::UMiniJamWheelFront()
{
	AxleType = EAxleType::Front;
	bAffectedBySteering = true;
	MaxSteerAngle = 40.f;
}