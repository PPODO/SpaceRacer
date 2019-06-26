#pragma once

#include "CoreMinimal.h"
#include "BaseProjectile.h"
#include "KnifeProjectile.generated.h"

UCLASS()
class SPACERACER_API AKnifeProjectile : public ABaseProjectile
{
	GENERATED_BODY()
	
public:
	AKnifeProjectile();

public:
	virtual void DeActivate() override;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

private:
	float m_fCurrentVelocity;
	float m_fInterpSpeed;

private:
	UFUNCTION()
		void OnComponentHit(class UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

};