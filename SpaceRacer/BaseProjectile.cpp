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

	InitialLifeSpan = 7.5f;
	PrimaryActorTick.bCanEverTick = false;
}