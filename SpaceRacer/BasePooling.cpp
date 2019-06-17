#include "BasePooling.h"
#include "PoolObjectOwnerComponent.h"

ABasePooling::ABasePooling() {
	m_ObjectName = "";
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.SetTickFunctionEnable(false);

	m_fElapsedTime = 0.f;
	m_fLifeSpanTime = 0.f;
}

void ABasePooling::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	if (m_fElapsedTime >= m_fLifeSpanTime) {
		ReleaseThisObject(m_ObjectName);
		DeActivate();
	}
	m_fElapsedTime += DeltaTime;
}

void ABasePooling::Activate(AActor* Owner, bool bUseTick) {
	SetOwner(Owner);
	SetActorEnableCollision(true);
	SetActorTickEnabled(bUseTick);
	SetActorHiddenInGame(false);
	m_fElapsedTime = 0.f;
}

void ABasePooling::DeActivate() {
	SetOwner(nullptr);
	SetActorEnableCollision(false);
	SetActorTickEnabled(false);
	SetActorHiddenInGame(true);
}

void ABasePooling::ReleaseThisObject(const FName& ObjectName) {
	AActor* Owner = GetOwner();

	if (Owner) {
		UPoolObjectOwnerComponent* OwnerComponent = Cast<UPoolObjectOwnerComponent>(Owner->GetComponentByClass(UPoolObjectOwnerComponent::StaticClass()));
		if (OwnerComponent) {
			OwnerComponent->ReleasePoolObject(ObjectName, this);
		}
	}
}