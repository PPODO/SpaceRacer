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

private:
	UPROPERTY(VisibleDefaultsOnly)
		class UStaticMeshComponent* m_HeadPanelMeshComponent;
	UPROPERTY(VisibleDefaultsOnly)
		class UPoolObjectOwnerComponent* m_PoolOwnerComponent;
	UPROPERTY(VisibleDefaultsOnly)
		class USceneComponent* m_MultipleProjectileMuzzle;

private:
	class UMaterial* m_CubeMaterial;

private:
	class APawn* m_PlayerPawnClass;
	FRotator m_PrevLookAtRotation;

private:
	TSharedPtr<TArray<class ABasePooling*>*> m_MultipleHomingMissilePtr;

private:
	FTimerHandle m_TH;

private:
	UFUNCTION()
		void TimerCallback();

private:
	FORCEINLINE void CalculateHeadRotation(ULBAnimInstance* Anim, const float& CurrentYawRotation, const float& Target, const float& DeltaTime, const float& InterpSpeed = 1.f) {
		if (Anim) {
			float InterpResult = UKismetMathLibrary::FInterpTo(CurrentYawRotation, Target, DeltaTime, InterpSpeed);
			Anim->SetYawRotation(InterpResult);
		}
	}

};