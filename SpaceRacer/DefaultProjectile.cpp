#include "DefaultProjectile.h"
#include "ConstructorHelpers.h"
#include "PoolObjectOwnerComponent.h"
#include "GameFramework/DamageType.h"

ADefaultProjectile::ADefaultProjectile() {
	::ConstructorHelpers::FObjectFinder<UStaticMesh> MeshObject(L"StaticMesh'/Game/ProjectilesPack/Meshes/Projectiles/Bullets/02/SM_Bullet_02_1.SM_Bullet_02_1'");
	::ConstructorHelpers::FObjectFinder<UParticleSystem> HitParticle(L"ParticleSystem'/Game/ProjectilesPack/Particles/Bullet/P_BulletImpact.P_BulletImpact'");
	
	if (HitParticle.Succeeded()) {
		m_HitParticle = HitParticle.Object;
	}

	RootComponent = m_CollisionComponent;

	if (m_CollisionComponent->IsValidLowLevelFast()) {
		m_CollisionComponent->SetBoxExtent(FVector(15.f, 2.5f, 2.5f));
		m_CollisionComponent->OnComponentHit.AddDynamic(this, &ADefaultProjectile::OnComponentHit);
	}

	if(MeshObject.Succeeded() && m_ProjectileMesh->IsValidLowLevelFast()) {
		m_ProjectileMesh->SetStaticMesh(MeshObject.Object);
		m_ProjectileMesh->SetRelativeLocation(FVector(-15.f, 0.f, 0.f));
		m_ProjectileMesh->SetWorldScale3D(FVector(2.f));
	}

	if (m_MovementComponent->IsValidLowLevelFast()) {
		m_MovementComponent->InitialSpeed = 5000.f;
		m_MovementComponent->MaxSpeed = 5000.f;
		m_MovementComponent->ProjectileGravityScale = 0.f;
	}

	m_fBaseDamage = 15.f;
	m_fLifeSpanTime = 1.5f;
	PrimaryActorTick.bCanEverTick = false;
}

void ADefaultProjectile::OnComponentHit(UPrimitiveComponent * HitComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, FVector NormalImpulse, const FHitResult & Hit) {
	if (IsValid(m_HitParticle)) {
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), m_HitParticle, GetActorLocation());
	}
	if (OtherActor && this != OtherActor && OtherActor != GetOwner()) {
		UGameplayStatics::ApplyDamage(OtherActor, m_fBaseDamage, nullptr, this, UDamageType::StaticClass());
	}
	Release();
}