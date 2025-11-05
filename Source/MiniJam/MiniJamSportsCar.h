// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "MiniJamPawn.h"
#include "MiniJamSportsCar.generated.h"

/**
 *  Sports car wheeled vehicle implementation
 */
UCLASS(abstract)
class MINIJAM_API AMiniJamSportsCar : public AMiniJamPawn
{
	GENERATED_BODY()
	
public:

	AMiniJamSportsCar();
};
