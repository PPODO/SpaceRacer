#include "MissileExplosionComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "Sound/SoundCue.h"
#include "ConstructorHelpers.h"
#include "Engine/World.h"

UMissileExplosionComponent::UMissileExplosionComponent() {
	::ConstructorHelpers::FObjectFinder<UParticleSystem> ExplosionParticle(L"ParticleSystem'/Game/ProjectilesPack/Particles/Effects/P_ExplosionWithShrapnel.P_ExplosionWithShrapnel'");
	if (ExplosionParticle.Succeeded()) {
		m_ExplosionParticle = ExplosionParticle.Object;
	}

	::ConstructorHelpers::FObjectFinder<USoundCue> ExplosionCue(L"SoundCue'/Game/ProjectilesPack/Sounds/Common/Explosion_02_Cue.Explosion_02_Cue'");
	if (ExplosionCue.Succeeded()) {
		m_ExplosionCue = ExplosionCue.Object;
	}

	bWantsBeginPlay = false;
	PrimaryComponentTick.bCanEverTick = false;
}

void UMissileExplosionComponent::Excute(const FVector & Location) {
	if (m_ExplosionParticle) {
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), m_ExplosionParticle, Location);
	}
	if (m_ExplosionCue) {
		UGameplayStatics::SpawnSoundAtLocation(GetWorld(), m_ExplosionCue, Location);
	}
}