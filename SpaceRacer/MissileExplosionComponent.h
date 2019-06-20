#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MissileExplosionComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SPACERACER_API UMissileExplosionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UMissileExplosionComponent();
	
private:
	UPROPERTY(VisibleDefaultsOnly)
		class UParticleSystem* m_ExplosionParticle;

	UPROPERTY(VisibleDefaultsOnly)
		class USoundCue* m_ExplosionCue;

public:
	void Excute(const FVector& Location);

};