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
	UPROPERTY()
		class USceneComponent* m_CannonMuzzleComponent;
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = "true"))
		class UChildActorComponent* m_SwordMasterChildActorClass;
	UPROPERTY()
		class UPoolObjectOwnerComponent* m_PoolOwnerComponent;

	UPROPERTY(EditAnywhere, Category = "Camera", Meta = (AllowPrivateAccess = "true"))
		float m_fLookUpRate = 0.5f;
	UPROPERTY(EditAnywhere, Category = "Camera", Meta = (AllowPrivateAccess = "true"))
		float m_fLookRightRate = 0.5f;
	UPROPERTY(EditAnywhere, Category = "Projectile Setting", Meta = (AllowPrivateAccess = "true"))
		float m_fFireProjectileDelay = 0.125f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectile Count", Meta = (AllowPrivateAccess = "true"))
		bool m_bIsInfiniteDefaultProjectile;

private:
	TMap<uint32, APawn*> m_SensingPawns;

private:
	UPROPERTY(BlueprintReadOnly, Meta = (AllowPrivateAccess = "true"))
		float m_fHealth;
	UPROPERTY(BlueprintReadOnly, Meta = (AllowPrivateAccess = "true"))
		float m_fMana;
	UPROPERTY(BlueprintReadOnly, Meta = (AllowPrivateAccess = "true"))
		int32 m_CurrentDefaultProjectileCount;
	UPROPERTY(BlueprintReadOnly, Meta = (AllowPrivateAccess = "true"))
		int32 m_CurrentLeftDefaultProjectileCount;
	UPROPERTY(BlueprintReadOnly, Meta = (AllowPrivateAccess = "true"))
		int32 m_CurrentNuclearProjectileCount;
	UPROPERTY(BlueprintReadOnly, Meta = (AllowPrivateAccess = "true"))
		bool m_bIsDead;

	UPROPERTY()
		class USoundCue* m_FireDefaultProjectileCue;
	UPROPERTY()
		class UParticleSystem* m_MuzzleEffect;
	UPROPERTY()
		class USoundCue* m_AbilitySoundCue;

private:
	FVector m_NuclearSpawnOffset;
	FRotator m_CannonDefaultRotation;
	bool m_bIsOnFire;
	bool m_bIsOnAbility;
	float m_ElapsedTime;

private:
	float m_CurrentSpringArmLength;
	bool m_bIsMovingSpringArmLength;
	bool m_InfinityHealth;
	bool m_InfinityMana;

private:
	FORCEINLINE void UpdateCannonRotation(const FRotator& CannonRotation);
	FORCEINLINE void FireProjectile(const FRotator& CannonRotation);
	FORCEINLINE void UpdateSpringArmLength(const float& Delta);
	FORCEINLINE void UpdateEngineAudioSound();

private:
	FORCEINLINE bool CanMove();

private:
	FORCEINLINE class ASwordMaster* GetSwordMasterChildActor();
	
protected:
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	virtual void Tick(float Delta) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

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
	void OnInfinityHealthPressed();
	void OnInfinityManaPressed();
	void OnInfinityProjectilePressed();

	static const FName LookUpBinding;
	static const FName LookRightBinding;
	static const FName EngineAudioRPM;

private:
	bool bIsLowFriction;
	UPhysicalMaterial* SlipperyMaterial;
	UPhysicalMaterial* NonSlipperyMaterial;

private:
	TSharedPtr<TArray<class ABasePooling*>*> m_DefaultProjectilePtr;
	TSharedPtr<TArray<class ABasePooling*>*> m_NuclearProjectilePtr;

public:
	FORCEINLINE USpringArmComponent* GetSpringArm() const { return SpringArm; }
	FORCEINLINE UCameraComponent* GetCamera() const { return Camera; }
	FORCEINLINE UAudioComponent* GetEngineSoundComponent() const { return EngineSoundComponent; }
	FORCEINLINE uint32 GetCurrentDefaultProjectileCount() const { return m_CurrentDefaultProjectileCount; }

};