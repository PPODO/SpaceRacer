#include "BaseProjectile.h"

ABaseProjectile::ABaseProjectile() {
	m_CollisionComponent = CreateDefaultSubobject<UBoxComponent>(L"Box Collision Component");
	m_CollisionComponent->SetRelativeLocation(FVector(0.f));
	m_CollisionComponent->SetRelativeRotation(FRotator(0.f));
	m_CollisionComponent->SetupAttachment(RootComponent);
	m_CollisionComponent->GetBodyInstance()->SetCollisionProfileName("Projectile");

	m_ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(L"Projectile Mesh Component");
	m_ProjectileMesh->SetRelativeRotation(FRotator(-90.f, 0.f, 0.f));
	m_ProjectileMesh->SetupAttachment(m_CollisionComponent);

	m_MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(L"Projectile Movement Component");

	PrimaryActorTick.bCanEverTick = false;
}

void ABaseProjectile::Activate(AActor* Owner, bool bUseTick) {
	ABasePooling::Activate(Owner, bUseTick);

	if (m_MovementComponent->IsValidLowLevelFast()) {
		m_MovementComponent->SetActive(true, true);
		m_MovementComponent->Velocity = GetActorForwardVector() * m_MovementComponent->InitialSpeed;
	}
}

void ABaseProjectile::DeActivate() {
	ABasePooling::DeActivate();

	if (m_MovementComponent->IsValidLowLevelFast()) {
		m_MovementComponent->SetActive(false);
	}
}