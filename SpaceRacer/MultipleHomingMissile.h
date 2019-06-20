#pragma once

#include "CoreMinimal.h"
#include "BasePooling.h"
#include "MultipleHomingMissile.generated.h"

UCLASS()
class SPACERACER_API AMultipleHomingMissile : public ABasePooling
{
	GENERATED_BODY()
	
public:
	AMultipleHomingMissile();

protected:
	virtual void Tick(float DeltaTime) override;

public:
	virtual void Activate(AActor* Owner, bool bUseTick = true) override;
	virtual void DeActivate() override;

private:
	UPROPERTY(VisibleDefaultsOnly)
		class USceneComponent* m_RootComponent;

	UPROPERTY(VisibleDefaultsOnly)
		class UPoolObjectOwnerComponent* m_PoolOwnerComponent;

private:
	int32 m_CurrentReleasedHomingProjectile;

private:
	float m_fMissileDistance = 200.f;

public:
	FORCEINLINE void SetMissileDistance(const float& NewDistance) { m_fMissileDistance = NewDistance; }

public:
	FORCEINLINE void AddReleasedProjectileCount() { m_CurrentReleasedHomingProjectile++; };

};
