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

private:
	UPROPERTY()
		class URadialForceComponent* m_RadialForceComponent;

private:
	UFUNCTION()
		void OnComponentHit(class UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

};