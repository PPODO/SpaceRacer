#pragma once

#include "CoreMinimal.h"
#include "BaseProjectile.h"
#include "HomingMissile.generated.h"

UCLASS()
class SPACERACER_API AHomingMissile : public ABaseProjectile
{
	GENERATED_BODY()
	
public:
	AHomingMissile();

public:
	virtual void Activate(AActor * Owner, bool bUseTick = true) override;
	virtual void DeActivate() override;

private:
	UPROPERTY(VisibleDefaultsOnly)
		class UMissileExplosionComponent* m_ExplosionComponent;

private:
	UFUNCTION()
		void OnComponentHit(class UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

public:
	void SetTarget(const AActor* NewTarget);

};
