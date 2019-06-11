// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "SpaceRacerHud.generated.h"

UCLASS(config = Game)
class ASpaceRacerHud : public AHUD
{
	GENERATED_BODY()

public:
	ASpaceRacerHud();

protected:
	virtual void DrawHUD() override;

private:
	UPROPERTY()
		UFont* HUDFont;

private:
	FVector2D m_CountTextLocation;

};