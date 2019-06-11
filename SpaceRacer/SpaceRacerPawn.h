// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "WheeledVehicle.h"
#include "SpaceRacerPawn.generated.h"

class UPhysicalMaterial;
class UCameraComponent;
class USpringArmComponent;
class UTextRenderComponent;
class UInputComponent;
class UAudioComponent;

UCLASS(config=Game)
class ASpaceRacerPawn : public AWheeledVehicle
{
	GENERATED_BODY()

private:
	UPROPERTY(Category = Camera, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* SpringArm;

	UPROPERTY(Category = Camera, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* Camera;

	UPROPERTY(Category = Display, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UAudioComponent* EngineSoundComponent;

public:
	ASpaceRacerPawn();

private:
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* m_CannonMeshComponent;
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = "true"))
		class USceneComponent* m_CannonMuzzleComponent;
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = "true"))
		class UChildActorComponent* m_SwordMasterChildActorClass;

	UPROPERTY(EditAnywhere, Category = "Camera", Meta = (AllowPrivateAccess = "true"))
		float m_fLookUpRate = 0.5f;
	UPROPERTY(EditAnywhere, Category = "Camera", Meta = (AllowPrivateAccess = "true"))
		float m_fLookRightRate = 0.5f;
	UPROPERTY(EditAnywhere, Category = "Projectile Setting", Meta = (AllowPrivateAccess = "true"))
		float m_fFireProjectileDelay = 0.25f;

private:
	TMap<uint32, APawn*> m_SensingPawns;

private:
	uint32 m_CurrentDefaultProjectileCount;
	uint32 m_CurrentNuclearProjectileCount;

	UPROPERTY()
		class USoundCue* m_FireDefaultProjectileCue;
	UPROPERTY()
		class UParticleSystem* m_MuzzleEffect;

	class ASwordMaster* m_SwordMasterClass;

private:
	FVector m_NuclearSpawnOffset;
	FRotator m_CannonDefaultRotation;
	bool m_bIsOnFire;
	bool m_bIsOnAbility;
	float m_ElapsedTime;

private:
	float m_CurrentSpringArmLength;
	bool m_bIsMovingSpringArmLength;

private:
	FORCEINLINE void UpdateCannonRotation(const FRotator& CannonRotation);
	FORCEINLINE void FireProjectile(const FRotator& CannonRotation);
	FORCEINLINE void UpdateSpringArmLength(const float& Delta);
	FORCEINLINE void UpdateEngineAudioSound();
	
protected:
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	virtual void Tick(float Delta) override;

private:
	void MoveForward(float Val);
	void MoveRight(float Val);
	void LookUp(float Val);
	void LookRight(float Val);

	void UpdatePhysicsMaterial();

	void OnHandbrakePressed();
	void OnHandbrakeReleased();
	void OnFireProjectilePressed();
	void OnFireProjectileReleased();
	void OnFireNuclearPressed();
	void OnUseAbilityPressed();

	static const FName LookUpBinding;
	static const FName LookRightBinding;
	static const FName EngineAudioRPM;

private:
	bool bIsLowFriction;
	UPhysicalMaterial* SlipperyMaterial;
	UPhysicalMaterial* NonSlipperyMaterial;

public:
	FORCEINLINE USpringArmComponent* GetSpringArm() const { return SpringArm; }
	FORCEINLINE UCameraComponent* GetCamera() const { return Camera; }
	FORCEINLINE UAudioComponent* GetEngineSoundComponent() const { return EngineSoundComponent; }
	FORCEINLINE uint32 GetCurrentDefaultProjectileCount() const { return m_CurrentDefaultProjectileCount; }

};