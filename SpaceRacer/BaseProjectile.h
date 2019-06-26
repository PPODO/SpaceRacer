#pragma once

#include "CoreMinimal.h"
#include "BasePooling.h"
#include "Components/BoxComponent.h"
#include "GameFramework/DamageType.h"
#include "Components/StaticMeshComponent.h"
#include "GameFrameWork/ProjectileMovementComponent.h"
#include "Sound/SoundCue.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "BaseProjectile.generated.h"

UCLASS()
class SPACERACER_API ABaseProjectile : public ABasePooling
{
	GENERATED_BODY()
	
public:
	ABaseProjectile();

public:
	virtual void Activate(AActor* Owner, bool bUseTick = true) override;
	virtual void DeActivate() override;

protected:
	UPROPERTY(VisibleDefaultsOnly)
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
		class UParticleSystem* m_HitParticle;
	UPROPERTY()
		TArray<class UParticleSystemComponent*> m_ParticleSystemComponents;

protected:
	float m_fBaseDamage;

};