#include "BasePooling.h"

ABasePooling::ABasePooling() {
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.SetTickFunctionEnable(false);
}

void ABasePooling::Activate(AActor* Owner, bool bUseTick) {
	SetOwner(Owner);
	SetActorEnableCollision(true);
	SetActorTickEnabled(bUseTick);
	SetActorHiddenInGame(false);
}

void ABasePooling::DeActivate() {
	SetOwner(nullptr);
	SetActorEnableCollision(false);
	SetActorTickEnabled(false);
	SetActorHiddenInGame(true);
}