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
		class USphereComponent* m_ShieldComponent;
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = "true"))
		TArray<class UStaticMeshComponent*> m_SwordMeshes;

private:
	UPROPERTY()
		class UParticleSystem* m_ActivateSwordMasterEffect;
	UPROPERTY()
		class UParticleSystem* m_OverlappedEffect;

private:
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = "true"))
		float m_fSwordDistance = 150.f;
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = "true"))
		float m_fInterpSpeed = 0.75f;

private:
	float m_fCurrentSpinSpeed;

private:
	UFUNCTION()
		void OnShieldComponentOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:
	void ActivateSwordMater(bool bActivate, bool bSpawnParticle = true);

public:
	FORCEINLINE void SetSwordDistance(const float& Distance) { m_fSwordDistance = Distance; }
	FORCEINLINE void SetInterpSpeed(const float& Speed) { m_fInterpSpeed = Speed; }

};