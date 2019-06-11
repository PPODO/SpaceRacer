// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "SpaceRacerHud.h"
#include "SpaceRacerPawn.h"
#include "CanvasItem.h"
#include "Engine/Canvas.h"
#include "Engine/Font.h"
#include "Engine/Engine.h"
#include "UObject/ConstructorHelpers.h"

ASpaceRacerHud::ASpaceRacerHud() {
	static ConstructorHelpers::FObjectFinder<UFont> Font(TEXT("/Engine/EngineFonts/RobotoDistanceField"));
	HUDFont = Font.Object;


}

void ASpaceRacerHud::DrawHUD() {
	Super::DrawHUD();

	m_CountTextLocation = FVector2D(Canvas->SizeX, Canvas->SizeY);

	ASpaceRacerPawn* Character = Cast<ASpaceRacerPawn>(GetOwningPawn());

	if (IsValid(Character)) {
		{
			FCanvasTextItem ProjectileCountItem(m_CountTextLocation, FText::AsNumber(Character->GetCurrentDefaultProjectileCount()), HUDFont, FLinearColor::White);
			ProjectileCountItem.Scale = FVector2D(5.f, 5.f);

			Canvas->DrawItem(ProjectileCountItem);
		}
	}
}