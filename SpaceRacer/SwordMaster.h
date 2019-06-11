#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SwordMaster.generated.h"

UCLASS()
class SPACERACER_API ASwordMaster : public AActor
{
	GENERATED_BODY()
	
public:
	ASwordMaster();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = "true"))
		class USceneComponent* m_RootComponent;
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = "true"))
		TArray<class UStaticMeshComponent*> m_SwordMeshes;

private:
	UPROPERTY()
		class UParticleSystem* m_ActivateSwordMasterEffect;

private:
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = "true"))
		float m_fSwordDistance = 150.f;
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = "true"))
		float m_fInterpSpeed = 0.75f;

private:
	float m_fCurrentSpinSpeed;

public:
	FORCEINLINE void ActivateSwordMater(bool bActivate, bool bSpawnParticle = true);

public:
	FORCEINLINE void SetSwordDistance(const float& Distance) { m_fSwordDistance = Distance; }
	FORCEINLINE void SetInterpSpeed(const float& Speed) { m_fInterpSpeed = Speed; }

};