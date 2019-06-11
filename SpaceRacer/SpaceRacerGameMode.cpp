// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "SpaceRacerGameMode.h"
#include "SpaceRacerPawn.h"
#include "SpaceRacerHud.h"

ASpaceRacerGameMode::ASpaceRacerGameMode() {
	DefaultPawnClass = ASpaceRacerPawn::StaticClass();
	HUDClass = ASpaceRacerHud::StaticClass();
}