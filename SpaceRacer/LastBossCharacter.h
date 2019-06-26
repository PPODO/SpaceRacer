#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "LBAnimInstance.h"
#include "Kismet/KismetMathLibrary.h"
#include "LastBossCharacter.generated.h"

UCLASS()
class SPACERACER_API ALastBossCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ALastBossCharacter();

protected:
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

private:
	UPROPERTY(VisibleDefaultsOnly)
		class UStaticMeshComponent* m_HeadPanelMeshComponent;
	UPROPERTY(VisibleDefaultsOnly)
		class UPoolObjectOwnerComponent* m_PoolOwnerComponent;
	UPROPERTY(VisibleDefaultsOnly)
		class USceneComponent* m_MultipleProjectileMuzzleRootComponent;
	UPROPERTY(VisibleDefaultsOnly)
		class USceneComponent* m_MultipleProjectileMuzzle;
	UPROPERTY(VisibleDefaultsOnly)
		class UTheWorldComponent* m_TheWorldComponent;

private:
	class UMaterial* m_CubeMaterialNoStrong;
	class UMaterial* m_CubeMaterialStrong;
	bool m_bIsChangePhase;

private:
	class APawn* m_PlayerPawnClass;
	FRotator m_PrevLookAtRotation;

private:
	TSharedPtr<TArray<class ABasePooling*>*> m_MultipleHomingMissilePtr;

private:
	FTimerHandle m_TH;

private:
	UFUNCTION()
		void FireMultipleHomingMissileCallback();

private:
	float m_fLastTheWorldTime;
	float m_fDelayTime;

private:
	UPROPERTY(BlueprintReadOnly, Meta = (AllowPrivateAccess = "true"))
		float m_fHealth;
	UPROPERTY(BlueprintReadOnly, Meta = (AllowPrivateAccess = "true"))
		FName m_BossName;

private:
	FORCEINLINE void UseTheWorld();

private:
	FORCEINLINE void CalculateHeadRotation(ULBAnimInstance* Anim, const float& CurrentYawRotation, const float& Target, const float& DeltaTime, const float& InterpSpeed = 1.f) {
		if (Anim) {
			float InterpResult = UKismetMathLibrary::FInterpTo(CurrentYawRotation, Target, DeltaTime, InterpSpeed);
			Anim->SetYawRotation(InterpResult);
		}
	}

};