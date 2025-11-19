// Copyright Epic Games, Inc. All Rights Reserved.


#include "MiniJamUI.h"

void UMiniJamUI::UpdateSpeed(float NewSpeed)
{
	// format the speed to KPH or MPH
	float FormattedSpeed = FMath::Abs(NewSpeed) * (bIsMPH ? 0.022f : 0.036f);

	// call the Blueprint handler
	OnSpeedUpdate(FormattedSpeed);
}

void UMiniJamUI::UpdateGear(int32 NewGear)
{
	// call the Blueprint handler
	OnGearUpdate(NewGear);
}

void UMiniJamUI::UpdateEnergy(float CurrentEnergy, float MaxEnergy)
{
	float Percent = 0.0f;
	if (MaxEnergy > 0.0f)
	{
		Percent = CurrentEnergy / MaxEnergy;
	}

	Percent = FMath::Clamp(Percent, 0.0f, 1.0f);

	OnEnergyUpdate(Percent);
}