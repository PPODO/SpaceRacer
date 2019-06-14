#include "BasePooling.h"

ABasePooling::ABasePooling() {
	m_fLifeSpan = 0.f;

	PrimaryActorTick.bCanEverTick = false;
}

void ABasePooling::Activate() {
	SetActorEnableCollision(true);
	SetActorTickEnabled(true);
	SetActorHiddenInGame(false);
	SetLifeSpan(m_fLifeSpan);
}

void ABasePooling::DeActivate() {
	SetActorEnableCollision(false);
	SetActorTickEnabled(false);
	SetActorHiddenInGame(true);
	SetLifeSpan(0.f);
}