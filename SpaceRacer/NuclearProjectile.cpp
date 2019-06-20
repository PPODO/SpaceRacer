#include "NuclearProjectile.h"
#include "NuclearExplosionCameraShake.h"
#include "PhysicsEngine/RadialForceComponent.h"
#include "PoolObjectOwnerComponent.h"
#include "ConstructorHelpers.h"
#include "Sound/SoundClass.h"

ANuclearProjectile::ANuclearProjectile() {
	::ConstructorHelpers::FObjectFinder<UStaticMesh> MeshObject(TEXT("StaticMesh'/Game/ProjectilesPack/Meshes/Projectiles/Bombs/Bomb_01/SM_Bomb_01.SM_Bomb_01'"));
	//::ConstructorHelpers::FObjectFinder<USoundCue> TrailSound(TEXT(""));
	::ConstructorHelpers::FObjectFinder<USoundCue> HitSound(TEXT("SoundCue'/Game/ProjectilesPack/Sounds/Common/Explosion_02_Cue.Explosion_02_Cue'"));
	::ConstructorHelpers::FObjectFinder<UParticleSystem> TrailParticle(TEXT("ParticleSystem'/Game/VFX_Toolkit_V1/ParticleSystems/356Days/Par_MissileTrails2.Par_MissileTrails2'"));
	::ConstructorHelpers::FObjectFinder<UParticleSystem> HitParticle(TEXT("ParticleSystem'/Game/VFX_Toolkit_V1/ParticleSystems/356Days/Par_DivineLaser.Par_DivineLaser'"));
	::ConstructorHelpers::FClassFinder<UNuclearExplosionCameraShake> CameraShake(L"Class'/Script/SpaceRacer.NuclearExplosionCameraShake'");

	if (CameraShake.Succeeded()) {
		m_ExplosionShakeClass = CameraShake.Class;
	}
	//	if (TrailSound.Succeeded()) {
	//		m_ProjectileTrailSound = TrailSound.Object;
	//	}
	if (HitSound.Succeeded()) {
		m_ProjectileCollidingSound = HitSound.Object;
	}
	if (HitParticle.Succeeded()) {
		m_HitParticle = HitParticle.Object;
	}
	RootComponent = m_CollisionComponent;

	if (m_CollisionComponent->IsValidLowLevelFast()) {
		m_CollisionComponent->SetBoxExtent(FVector(100.f, 30.f, 30.f));
		m_CollisionComponent->OnComponentHit.AddDynamic(this, &ANuclearProjectile::OnComponentHit);
	}

	if (MeshObject.Succeeded() && m_ProjectileMesh->IsValidLowLevelFast()) {
		m_ProjectileMesh->SetStaticMesh(MeshObject.Object);
		m_ProjectileMesh->SetRelativeLocation(FVector(-100.f, 0.f, 0.f));
		m_ProjectileMesh->SetWorldScale3D(FVector(0.25f));
	}

	if (m_MovementComponent->IsValidLowLevelFast()) {
		m_MovementComponent->InitialSpeed = 2000.f;
		m_MovementComponent->MaxSpeed = 2000.f;
		m_MovementComponent->ProjectileGravityScale = 0.75f;
		m_MovementComponent->bRotationFollowsVelocity = true;
		m_MovementComponent->bInterpRotation = true;
	}

	m_RadialForceComponent = CreateDefaultSubobject<URadialForceComponent>("Radial Force Component");
	m_RadialForceComponent->Radius = 2500.f;
	m_RadialForceComponent->SetRelativeLocation(FVector(0.f));
	m_RadialForceComponent->SetRelativeRotation(FRotator(0.f));
	m_RadialForceComponent->SetupAttachment(RootComponent);
	m_RadialForceComponent->ImpulseStrength = 300000.f;
	m_RadialForceComponent->ForceStrength = 300000.f;
	m_RadialForceComponent->Falloff = ERadialImpulseFalloff::RIF_Linear;
	m_RadialForceComponent->SetupAttachment(RootComponent);

	if (TrailParticle.Succeeded()) {
		UParticleSystemComponent* ParticleSysComp1 = CreateDefaultSubobject<UParticleSystemComponent>("Particle System Component 1");
		ParticleSysComp1->SetTemplate(TrailParticle.Object);
		ParticleSysComp1->SetRelativeLocation(FVector(-75.f, 0.f, 0.f));
		ParticleSysComp1->SetWorldScale3D(FVector(1.5f));
		ParticleSysComp1->SetupAttachment(RootComponent);

		m_ParticleSystemComponents.Add(ParticleSysComp1);
	}

	m_fLifeSpanTime = 3.5f;
	PrimaryActorTick.bCanEverTick = false;
}

void ANuclearProjectile::Activate(AActor * Owner, bool bUseTick) {
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

void ANuclearProjectile::DeActivate() {
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

void ANuclearProjectile::OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) {
	FVector SpawnEffectsLocation = GetActorLocation();
	if (OtherActor && this != OtherActor && OtherActor != GetOwner()) {
		FHitResult HitResult;
		FCollisionQueryParams Param;
		Param.AddIgnoredActor(this);
		Param.AddIgnoredActor(OtherActor);

		if (GetWorld()->LineTraceSingleByChannel(HitResult, GetActorLocation(), GetActorLocation() + (GetActorUpVector() * -10000), ECC_WorldStatic, Param)) {
			SpawnEffectsLocation = HitResult.ImpactPoint;
		}
	}
	if (IsValid(m_HitParticle) && IsValid(m_ProjectileCollidingSound)) {
		UGameplayStatics::SpawnSoundAtLocation(GetWorld(), m_ProjectileCollidingSound, SpawnEffectsLocation);
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), m_HitParticle, SpawnEffectsLocation);
	}
	auto PC = GetWorld()->GetFirstPlayerController();
	if (PC->IsValidLowLevelFast()) {
		PC->PlayerCameraManager->PlayCameraShake(m_ExplosionShakeClass);
	}

	m_RadialForceComponent->FireImpulse();
	Release();
}