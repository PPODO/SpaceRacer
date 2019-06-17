// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "SpaceRacerPawn.h"
#include "SpaceRacerWheelFront.h"
#include "SpaceRacerWheelRear.h"
#include "PoolObjectOwnerComponent.h"
#include "NuclearExplosionCameraShake.h"
#include "SpaceRacerHud.h"
#include "DefaultProjectile.h"
#include "NuclearProjectile.h"
#include "SwordMaster.h"
#include "ObjectPoolManager.h"
#include "SpaceRacerGameInstance.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/AudioComponent.h"
#include "Components/InputComponent.h"
#include "Components/TextRenderComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Sound/SoundCue.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "WheeledVehicleMovementComponent4W.h"
#include "Engine/Engine.h"
#include "Engine/SkeletalMesh.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Components/ChildActorComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Perception/PawnSensingComponent.h"

const FName ASpaceRacerPawn::LookUpBinding("LookUp");
const FName ASpaceRacerPawn::LookRightBinding("LookRight");
const FName ASpaceRacerPawn::EngineAudioRPM("RPM");
const uint32 MaxDefaultProjectileCount = 50;
const uint32 MaxNuclearProjectileCount = 3;
const float DefaultSpringArmLength = 125.f;
const float AbilityOnSpringArmLength = 225.f;
const float PerceptionAngle = FMath::DegreesToRadians(50.f);

ASpaceRacerPawn::ASpaceRacerPawn() : m_bIsOnAbility(false), m_CurrentSpringArmLength(DefaultSpringArmLength), m_bIsMovingSpringArmLength(false) {
	::ConstructorHelpers::FObjectFinder<USoundCue> AbilitySoundCue(L"SoundCue'/Game/ProjectileSounds/AbilitySound_Cue.AbilitySound_Cue'");
	::ConstructorHelpers::FObjectFinder<USoundCue> DefaultProjectileCue(L"SoundCue'/Game/ProjectileSounds/DefaultProjectileShotSound2_Cue.DefaultProjectileShotSound2_Cue'");

	if (AbilitySoundCue.Succeeded()) {
		m_AbilitySoundCue = AbilitySoundCue.Object;
	}

	if (DefaultProjectileCue.Succeeded()) {
		m_FireDefaultProjectileCue = DefaultProjectileCue.Object;
	}

	::ConstructorHelpers::FObjectFinder<UStaticMesh> MeshObject(TEXT("StaticMesh'/Game/VFX_Toolkit_V1/StaticMeshes/Various/SM_Vulcannon_Head.SM_Vulcannon_Head'"));
	if (MeshObject.Succeeded()) {
		m_CannonMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(L"Cannon Mesh Component");
		m_CannonMeshComponent->SetStaticMesh(MeshObject.Object);
		m_CannonMeshComponent->SetRelativeLocation(FVector(-10.f, 0.f, 75.f));
		m_CannonMeshComponent->SetRelativeRotation(FRotator(0.f));
		m_CannonMeshComponent->SetWorldScale3D(FVector(0.25f));
		m_CannonMeshComponent->SetupAttachment(RootComponent);
		m_CannonMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		
		m_CannonMuzzleComponent = CreateDefaultSubobject<USceneComponent>(L"Cannon Offset Component");
		m_CannonMuzzleComponent->SetupAttachment(m_CannonMeshComponent);
		m_CannonMuzzleComponent->SetRelativeLocation(FVector(280.f, 0.f, -7.25f));
		m_CannonMuzzleComponent->SetRelativeRotation(FRotator(0.f));
	}

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> CarMesh(TEXT("/Game/VehicleAdv/Vehicle/Vehicle_SkelMesh.Vehicle_SkelMesh"));
	GetMesh()->SetSkeletalMesh(CarMesh.Object);

	static ConstructorHelpers::FClassFinder<UObject> AnimBPClass(TEXT("/Game/VehicleAdv/Vehicle/VehicleAnimationBlueprint"));
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	GetMesh()->SetAnimInstanceClass(AnimBPClass.Class);

	static ConstructorHelpers::FObjectFinder<UPhysicalMaterial> SlipperyMat(TEXT("/Game/VehicleAdv/PhysicsMaterials/Slippery.Slippery"));
	SlipperyMaterial = SlipperyMat.Object;

	static ConstructorHelpers::FObjectFinder<UPhysicalMaterial> NonSlipperyMat(TEXT("/Game/VehicleAdv/PhysicsMaterials/NonSlippery.NonSlippery"));
	NonSlipperyMaterial = NonSlipperyMat.Object;

	::ConstructorHelpers::FObjectFinder<UParticleSystem> MuzzleEffect(L"ParticleSystem'/Game/VFX_Toolkit_V1/ParticleSystems/356Days/Par_MuzzleFlash1_01.Par_MuzzleFlash1_01'");
	if (MuzzleEffect.Succeeded()) {
		m_MuzzleEffect = MuzzleEffect.Object;
	}

	UWheeledVehicleMovementComponent4W* Vehicle4W = CastChecked<UWheeledVehicleMovementComponent4W>(GetVehicleMovement());

	check(Vehicle4W->WheelSetups.Num() == 4);

	Vehicle4W->WheelSetups[0].WheelClass = USpaceRacerWheelFront::StaticClass();
	Vehicle4W->WheelSetups[0].BoneName = FName("PhysWheel_FL");
	Vehicle4W->WheelSetups[0].AdditionalOffset = FVector(0.f, -8.f, 0.f);

	Vehicle4W->WheelSetups[1].WheelClass = USpaceRacerWheelFront::StaticClass();
	Vehicle4W->WheelSetups[1].BoneName = FName("PhysWheel_FR");
	Vehicle4W->WheelSetups[1].AdditionalOffset = FVector(0.f, 8.f, 0.f);

	Vehicle4W->WheelSetups[2].WheelClass = USpaceRacerWheelRear::StaticClass();
	Vehicle4W->WheelSetups[2].BoneName = FName("PhysWheel_BL");
	Vehicle4W->WheelSetups[2].AdditionalOffset = FVector(0.f, -8.f, 0.f);

	Vehicle4W->WheelSetups[3].WheelClass = USpaceRacerWheelRear::StaticClass();
	Vehicle4W->WheelSetups[3].BoneName = FName("PhysWheel_BR");
	Vehicle4W->WheelSetups[3].AdditionalOffset = FVector(0.f, 8.f, 0.f);

	Vehicle4W->MinNormalizedTireLoad = 0.0f;
	Vehicle4W->MinNormalizedTireLoadFiltered = 0.2f;
	Vehicle4W->MaxNormalizedTireLoad = 2.0f;
	Vehicle4W->MaxNormalizedTireLoadFiltered = 2.0f;

	Vehicle4W->MaxEngineRPM = 5700.0f;
	Vehicle4W->EngineSetup.TorqueCurve.GetRichCurve()->Reset();
	Vehicle4W->EngineSetup.TorqueCurve.GetRichCurve()->AddKey(0.0f, 400.0f);
	Vehicle4W->EngineSetup.TorqueCurve.GetRichCurve()->AddKey(1890.0f, 500.0f);
	Vehicle4W->EngineSetup.TorqueCurve.GetRichCurve()->AddKey(5730.0f, 400.0f);

	Vehicle4W->SteeringCurve.GetRichCurve()->Reset();
	Vehicle4W->SteeringCurve.GetRichCurve()->AddKey(0.0f, 1.0f);
	Vehicle4W->SteeringCurve.GetRichCurve()->AddKey(40.0f, 0.7f);
	Vehicle4W->SteeringCurve.GetRichCurve()->AddKey(120.0f, 0.6f);

	Vehicle4W->DifferentialSetup.DifferentialType = EVehicleDifferential4W::LimitedSlip_4W;

	Vehicle4W->DifferentialSetup.FrontRearSplit = 0.65;

	Vehicle4W->bDeprecatedSpringOffsetMode = true;
	Vehicle4W->TransmissionSetup.bUseGearAutoBox = true;
	Vehicle4W->TransmissionSetup.GearSwitchTime = 0.15f;
	Vehicle4W->TransmissionSetup.GearAutoBoxLatency = 1.0f;

	UPrimitiveComponent* UpdatedPrimitive = Cast<UPrimitiveComponent>(Vehicle4W->UpdatedComponent);
	if (UpdatedPrimitive) {
		UpdatedPrimitive->BodyInstance.COMNudge = FVector(8.0f, 0.0f, 0.0f);
	}

	Vehicle4W->InertiaTensorScale = FVector(1.0f, 1.333f, 1.2f);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetRelativeLocation(FVector(0.0f, 0.0f, 34.0f));
	SpringArm->SetWorldRotation(FRotator(-20.0f, 0.0f, 0.0f));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = DefaultSpringArmLength;
	SpringArm->bEnableCameraLag = false;
	SpringArm->bEnableCameraRotationLag = true;
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->bInheritPitch = true;
	SpringArm->bInheritYaw = true;
	SpringArm->bInheritRoll = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("ChaseCamera"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	Camera->SetRelativeLocation(FVector(-125.0, 0.0f, 0.0f));
	Camera->SetRelativeRotation(FRotator(10.0f, 0.0f, 0.0f));
	Camera->bUsePawnControlRotation = false;
	Camera->FieldOfView = 90.f;

	static ConstructorHelpers::FObjectFinder<USoundCue> SoundCue(TEXT("/Game/VehicleAdv/Sound/Engine_Loop_Cue.Engine_Loop_Cue"));
	EngineSoundComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("EngineSound"));
	EngineSoundComponent->SetSound(SoundCue.Object);
	EngineSoundComponent->SetupAttachment(GetMesh());

	m_SwordMasterChildActorClass = CreateDefaultSubobject<UChildActorComponent>(L"Sword Master Child Actor Class");
	m_SwordMasterChildActorClass->SetRelativeLocation(FVector(0.f));
	m_SwordMasterChildActorClass->SetRelativeRotation(FRotator(0.f));
	m_SwordMasterChildActorClass->SetChildActorClass(ASwordMaster::StaticClass());
	m_SwordMasterChildActorClass->SetupAttachment(RootComponent);

	m_PoolOwnerComponent = CreateDefaultSubobject<UPoolObjectOwnerComponent>(L"Pool Object Owner Component");
	m_PoolOwnerComponent->AddNewObjectType("DefaultProjectile", 75);
	m_PoolOwnerComponent->AddNewObjectType("NuclearProjectile", 5);

	m_NuclearSpawnOffset = FVector(0.f, 0.f, 1000.f);
	m_CannonDefaultRotation = FRotator(25.f, 0.f, 0.f);
	m_CurrentDefaultProjectileCount = ::MaxDefaultProjectileCount;
	m_CurrentNuclearProjectileCount = ::MaxNuclearProjectileCount;

	bIsLowFriction = false;
}

void ASpaceRacerPawn::BeginPlay() {
	Super::BeginPlay();

	EngineSoundComponent->Play();

	if (IsValid(m_SwordMasterChildActorClass)) {
		m_SwordMasterClass = Cast<ASwordMaster>(m_SwordMasterChildActorClass->GetChildActor());
		m_SwordMasterClass->SetOwner(this);
	}

	if (IsValid(m_PoolOwnerComponent)) {
		m_DefaultProjectilePtr = MakeShared<TArray<ABasePooling*>*>(m_PoolOwnerComponent->m_PoolObjects.Find("DefaultProjectile"));
		m_NuclearProjectilePtr = MakeShared<TArray<ABasePooling*>*>(m_PoolOwnerComponent->m_PoolObjects.Find("NuclearProjectile"));
	}
}

void ASpaceRacerPawn::Tick(float Delta) {
	Super::Tick(Delta);
	
	FRotator CurrentCannonRotation = GetControlRotation() + m_CannonDefaultRotation;

	UpdatePhysicsMaterial();
	UpdateCannonRotation(CurrentCannonRotation);
	FireProjectile(CurrentCannonRotation);
	UpdateSpringArmLength(Delta);
	UpdateEngineAudioSound();

	m_ElapsedTime += Delta;
}

void ASpaceRacerPawn::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) {
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	check(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ASpaceRacerPawn::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ASpaceRacerPawn::MoveRight);
	PlayerInputComponent->BindAxis(LookUpBinding, this, &ASpaceRacerPawn::LookUp);
	PlayerInputComponent->BindAxis(LookRightBinding, this, &ASpaceRacerPawn::LookRight);

	PlayerInputComponent->BindAction("Handbrake", IE_Pressed, this, &ASpaceRacerPawn::OnHandbrakePressed);
	PlayerInputComponent->BindAction("Handbrake", IE_Released, this, &ASpaceRacerPawn::OnHandbrakeReleased);
	PlayerInputComponent->BindAction("FireProjectile", IE_Pressed, this, &ASpaceRacerPawn::OnFireProjectilePressed);
	PlayerInputComponent->BindAction("FireProjectile", IE_Released, this, &ASpaceRacerPawn::OnFireProjectileReleased);
	PlayerInputComponent->BindAction("FireNuclear", IE_Pressed, this, &ASpaceRacerPawn::OnFireNuclearPressed);
	PlayerInputComponent->BindAction("UseAbility", IE_Pressed, this, &ASpaceRacerPawn::OnUseAbilityPressed);
}

void ASpaceRacerPawn::MoveForward(float Val) {
	GetVehicleMovementComponent()->SetThrottleInput(Val);
}

void ASpaceRacerPawn::MoveRight(float Val) {
	GetVehicleMovementComponent()->SetSteeringInput(Val);
}

void ASpaceRacerPawn::LookUp(float Val) {
	AddControllerPitchInput(Val * m_fLookUpRate);
}

void ASpaceRacerPawn::LookRight(float Val) {
	AddControllerYawInput(Val * m_fLookRightRate);
}

void ASpaceRacerPawn::OnHandbrakePressed() {
	GetVehicleMovementComponent()->SetHandbrakeInput(true);
}

void ASpaceRacerPawn::OnHandbrakeReleased() {
	GetVehicleMovementComponent()->SetHandbrakeInput(false);
}

void ASpaceRacerPawn::OnFireProjectilePressed() {
	m_bIsOnFire = true;
}

void ASpaceRacerPawn::OnFireProjectileReleased() {
	m_bIsOnFire = false;
}

void ASpaceRacerPawn::OnFireNuclearPressed() {
	if (m_CurrentNuclearProjectileCount > 0 && m_NuclearProjectilePtr.IsValid() && (*m_NuclearProjectilePtr)->Num() > 0) {
		ANuclearProjectile* Projectile = Cast<ANuclearProjectile>((*m_NuclearProjectilePtr)->Pop());
		if (IsValid(Projectile)) {
			Projectile->SetActorLocationAndRotation(GetActorLocation() + m_NuclearSpawnOffset, FRotator(0.f, GetControlRotation().Yaw, 0.f), false, nullptr, ETeleportType::TeleportPhysics);
			Projectile->Activate(this);

			m_CurrentNuclearProjectileCount--;
		}
	}
}

void ASpaceRacerPawn::OnUseAbilityPressed() {
	m_bIsMovingSpringArmLength = true;
	m_bIsOnAbility = !m_bIsOnAbility;
	if (IsValid(m_SwordMasterClass)) {
		m_SwordMasterClass->ActivateSwordMater(m_bIsOnAbility);
	}
	if (m_bIsOnAbility && IsValid(m_AbilitySoundCue)) {
		UGameplayStatics::PlaySound2D(GetWorld(), m_AbilitySoundCue);
	}
}

void ASpaceRacerPawn::UpdatePhysicsMaterial() {
	if (GetActorUpVector().Z < 0) {
		if (bIsLowFriction == true) {
			GetMesh()->SetPhysMaterialOverride(NonSlipperyMaterial);
			bIsLowFriction = false;
		}
		else {
			GetMesh()->SetPhysMaterialOverride(SlipperyMaterial);
			bIsLowFriction = true;
		}
	}
}

void ASpaceRacerPawn::UpdateCannonRotation(const FRotator& CannonRotation) {
	if (IsValid(m_CannonMeshComponent)) {
		m_CannonMeshComponent->SetWorldRotation(CannonRotation);
	}
}

void ASpaceRacerPawn::FireProjectile(const FRotator& CannonRotation) {
	if (m_bIsOnFire) {
		if (m_ElapsedTime > m_fFireProjectileDelay && m_CurrentDefaultProjectileCount > 0 && m_DefaultProjectilePtr.IsValid() && (*m_DefaultProjectilePtr)->Num() > 0 && IsValid(m_CannonMuzzleComponent)) {
			FVector MuzzleLocation = m_CannonMuzzleComponent->GetComponentLocation();
			ADefaultProjectile* ProjectileObject = Cast<ADefaultProjectile>((*m_DefaultProjectilePtr)->Pop());
			ProjectileObject->SetActorLocationAndRotation(MuzzleLocation, CannonRotation);
			ProjectileObject->Activate(this);

			UGameplayStatics::SpawnEmitterAttached(m_MuzzleEffect, m_CannonMuzzleComponent);
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), m_FireDefaultProjectileCue, GetActorLocation());
			
			if (ProjectileObject->IsValidLowLevelFast()) {
				m_CurrentDefaultProjectileCount--;
			}
			m_ElapsedTime = 0.f;
		}
	}
}

void ASpaceRacerPawn::UpdateSpringArmLength(const float& Delta) {
	if (m_bIsMovingSpringArmLength && IsValid(SpringArm)) {
		float TargetLength = m_bIsOnAbility ? AbilityOnSpringArmLength : DefaultSpringArmLength;
		if (!FMath::IsNearlyEqual(m_CurrentSpringArmLength, TargetLength)) {
			m_CurrentSpringArmLength = UKismetMathLibrary::FInterpTo(m_CurrentSpringArmLength, TargetLength, Delta, 1.f);
			SpringArm->TargetArmLength = m_CurrentSpringArmLength;
		}
		else {
			m_bIsMovingSpringArmLength = false;
		}
	}
}

void ASpaceRacerPawn::UpdateEngineAudioSound() {
	float RPMToAudioScale = 2500.0f / GetVehicleMovement()->GetEngineMaxRotationSpeed();
	EngineSoundComponent->SetFloatParameter(EngineAudioRPM, GetVehicleMovement()->GetEngineRotationSpeed()*RPMToAudioScale);
}