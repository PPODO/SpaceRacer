#pragma once

#include "CoreMinimal.h"
#include "BaseProjectile.h"
#include "NuclearProjectile.generated.h"

UCLASS()
class SPACERACER_API ANuclearProjectile : public ABaseProjectile
{
	GENERATED_BODY()
	
public:
	ANuclearProjectile();

public:
	virtual void Activate(AActor* Owner, bool bUseTick = true) override;
	virtual void DeActivate() override;

private:
	UPROPERTY()
		class URadialForceComponent* m_RadialForceComponent;
	UPROPERTY()
		TSubclassOf<class UNuclearExplosionCameraShake> m_ExplosionShakeClass;

private:
	UFUNCTION()
		void OnComponentHit(class UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

};