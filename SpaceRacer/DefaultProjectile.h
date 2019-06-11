#pragma once

#include "CoreMinimal.h"
#include "BaseProjectile.h"
#include "DefaultProjectile.generated.h"

UCLASS()
class SPACERACER_API ADefaultProjectile : public ABaseProjectile
{
	GENERATED_BODY()
	
public:	
	ADefaultProjectile();

private:
	UFUNCTION()
		void OnComponentHit(class UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

};