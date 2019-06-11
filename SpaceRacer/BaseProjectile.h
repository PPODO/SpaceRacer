#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFrameWork/ProjectileMovementComponent.h"
#include "Sound/SoundCue.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "BaseProjectile.generated.h"

UCLASS()
class SPACERACER_API ABaseProjectile : public AActor
{
	GENERATED_BODY()
	
public:
	ABaseProjectile();

protected:
	UPROPERTY()
		class UBoxComponent* m_CollisionComponent;
	UPROPERTY()
		class UStaticMeshComponent* m_ProjectileMesh;
	UPROPERTY()
		class UProjectileMovementComponent* m_MovementComponent;
	UPROPERTY()
		class USoundCue* m_ProjectileTrailSound;
	UPROPERTY()
		class USoundCue* m_ProjectileCollidingSound;
	UPROPERTY()
		class UParticleSystem* m_TrailParticle;
	UPROPERTY()
		class UParticleSystem* m_HitParticle;
	UPROPERTY()
		TArray<class UParticleSystemComponent*> m_ParticleSystemComponents;

protected:
	float m_fBaseDamage;

};
