#include "HomingMissile.h"
#include "MultipleHomingMissile.h"
#include "MissileExplosionComponent.h"
#include "ConstructorHelpers.h"

AHomingMissile::AHomingMissile() {
	m_ObjectName = "DefaultHomingProjectile";

	if (m_CollisionComponent) {
		m_CollisionComponent->SetBoxExtent(FVector(100.f, 15.f, 15.f));
		m_CollisionComponent->OnComponentHit.AddDynamic(this, &AHomingMissile::OnComponentHit);
		m_CollisionComponent->GetBodyInstance()->SetCollisionProfileName(L"Missile");
		RootComponent = m_CollisionComponent;
	}

	::ConstructorHelpers::FObjectFinder<UStaticMesh> RocketMesh(L"StaticMesh'/Game/ProjectilesPack/Meshes/Projectiles/Rockets/SM_Rocket_05.SM_Rocket_05'");
	if (RocketMesh.Succeeded()) {
		m_ProjectileMesh->SetRelativeLocation(FVector(-100.f, 0.f, 0.f));
		m_ProjectileMesh->SetWorldScale3D(FVector(2.f));
		m_ProjectileMesh->SetStaticMesh(RocketMesh.Object);
		m_ProjectileMesh->SetupAttachment(RootComponent);
	}

	::ConstructorHelpers::FObjectFinder<UParticleSystem> FlareParticle(L"ParticleSystem'/Game/ProjectilesPack/Particles/Effects/P_Flare.P_Flare'");
	if (FlareParticle.Succeeded()) {
		UParticleSystemComponent* ParticleComponent = CreateDefaultSubobject<UParticleSystemComponent>(L"Particle System Component 2");
		ParticleComponent->SetRelativeLocation(FVector(-100.f, 0.f, 0.f));
		ParticleComponent->SetWorldScale3D(FVector(2.f));
		ParticleComponent->SetTemplate(FlareParticle.Object);
		ParticleComponent->SetupAttachment(RootComponent);

		m_ParticleSystemComponents.Add(ParticleComponent);
	}

	::ConstructorHelpers::FObjectFinder<UParticleSystem> TrailParticle(L"ParticleSystem'/Game/ProjectilesPack/Particles/Effects/P_Smoke_Trail.P_Smoke_Trail'");
	if (TrailParticle.Succeeded()) {
		UParticleSystemComponent* ParticleComponent = CreateDefaultSubobject<UParticleSystemComponent>(L"Particle System Component 1");
		ParticleComponent->SetRelativeLocation(FVector(-100.f, 0.f, 0.f));
		ParticleComponent->SetWorldScale3D(FVector(1.5f));
		ParticleComponent->SetTemplate(TrailParticle.Object);
		ParticleComponent->SetupAttachment(RootComponent);

		m_ParticleSystemComponents.Add(ParticleComponent);
	}

	if (m_MovementComponent) {
		m_MovementComponent->ProjectileGravityScale = 0.f;
		m_MovementComponent->InitialSpeed = 10000.f;
		m_MovementComponent->MaxSpeed = 10000.f;
		m_MovementComponent->HomingAccelerationMagnitude = 20000.f;
		m_MovementComponent->bInterpRotation = true;
		m_MovementComponent->bRotationFollowsVelocity = true;
	}

	m_ExplosionComponent = CreateDefaultSubobject<UMissileExplosionComponent>(L"Missile Explosion Component");

	m_fLifeSpanTime = 10.f;
	PrimaryActorTick.bCanEverTick = false;
}

void AHomingMissile::Activate(AActor * Owner, bool bUseTick) {
	ABaseProjectile::Activate(Owner, bUseTick);

	if (m_ProjectileMesh->IsValidLowLevelFast()) {
		m_ProjectileMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		m_ProjectileMesh->SetHiddenInGame(false);
	}
	for (auto Iterator : m_ParticleSystemComponents) {
		if (Iterator->IsValidLowLevelFast()) {
			Iterator->SetActive(true, true);
		}
	}
}

void AHomingMissile::DeActivate() {
	if (AMultipleHomingMissile* Owner = Cast<AMultipleHomingMissile>(GetOwner())) {
		Owner->AddReleasedProjectileCount();
	}
	ABaseProjectile::DeActivate();

	SetActorHiddenInGame(false);
	if (m_ProjectileMesh->IsValidLowLevelFast()) {
		m_ProjectileMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		m_ProjectileMesh->SetHiddenInGame(true);
	}
	for (auto Iterator : m_ParticleSystemComponents) {
		if (Iterator->IsValidLowLevelFast()) {
			Iterator->DeactivateSystem();
		}
	}
}

void AHomingMissile::OnComponentHit(UPrimitiveComponent * HitComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, FVector NormalImpulse, const FHitResult & Hit) {
	if (OtherActor != this) {
		if (m_ExplosionComponent) {
			m_ExplosionComponent->Excute(GetActorLocation());
		}
		Release();
	}
}

void AHomingMissile::SetTarget(const AActor * NewTarget) {
	if (IsValid(m_MovementComponent) && IsValid(NewTarget)) {
		m_MovementComponent->HomingTargetComponent = Cast<USceneComponent>(NewTarget->GetComponentByClass(USceneComponent::StaticClass()));
		m_MovementComponent->bIsHomingProjectile = true;
	}
}