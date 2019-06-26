#include "LastBossCharacter.h"
#include "MultipleHomingMissile.h"
#include "TheWorldComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "ConstructorHelpers.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "PoolObjectOwnerComponent.h"
#include "TimerManager.h"

static const float MaxLastBossHealth = 10000.f;

ALastBossCharacter::ALastBossCharacter() {
	::ConstructorHelpers::FObjectFinder<USkeletalMesh> MeshObject(L"SkeletalMesh'/Game/Deaths/Character/Mesh/SK_Mannequin.SK_Mannequin'");
	::ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstance(L"/Game/Animations/BP_LBAnimInstance");

	if (MeshObject.Succeeded()) {
		GetMesh()->SetSkeletalMesh(MeshObject.Object);
		GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -90.f));
		GetMesh()->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
		if (AnimInstance.Succeeded()) {
			GetMesh()->SetAnimInstanceClass(AnimInstance.Class);
		}
	}
	// Actor Location = (X=-4358.000000,Y=-558.000000,Z=-5360.000000)
	SetActorScale3D(FVector(200.f));

	::ConstructorHelpers::FObjectFinder<UStaticMesh> CubeObject(L"StaticMesh'/Engine/BasicShapes/Cube.Cube'");
	::ConstructorHelpers::FObjectFinder<UMaterial> CubeMaterialNoStrong(L"Material'/Game/Texture/NoStrongLastBoss_Mat.NoStrongLastBoss_Mat'");
	::ConstructorHelpers::FObjectFinder<UMaterial> CubeMaterialStrong(L"Material'/Game/Texture/LastBossImage_Mat.LastBossImage_Mat'");

	if (CubeObject.Succeeded() && CubeMaterialStrong.Succeeded() && CubeMaterialNoStrong.Succeeded()) {
		m_HeadPanelMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(L"Panel Mesh Component");
		m_HeadPanelMeshComponent->SetupAttachment(GetMesh(), "head");
		m_HeadPanelMeshComponent->SetRelativeLocation(FVector(3.f, 14.f, 0.f));
		m_HeadPanelMeshComponent->SetRelativeRotation(FRotator(-90.f, 0.f, 0.f));
		m_HeadPanelMeshComponent->SetWorldScale3D(FVector(0.25f, 0.0025f, 0.25f));
		m_HeadPanelMeshComponent->SetStaticMesh(CubeObject.Object);
		m_CubeMaterialNoStrong = CubeMaterialNoStrong.Object;
		m_CubeMaterialStrong = CubeMaterialStrong.Object;
	}

	m_PoolOwnerComponent = CreateDefaultSubobject<UPoolObjectOwnerComponent>(L"Pool Object Owner Component");
	m_PoolOwnerComponent->AddNewObjectType("MultipleHomingProjectile", 5);

	m_MultipleProjectileMuzzleRootComponent = CreateDefaultSubobject<USceneComponent>(L"Multiple Projectile Muzzle Root Component");
	m_MultipleProjectileMuzzleRootComponent->SetupAttachment(RootComponent);
	m_MultipleProjectileMuzzleRootComponent->SetRelativeLocation(FVector(0.f, 0.f, 45.f));

	m_MultipleProjectileMuzzle = CreateDefaultSubobject<USceneComponent>(L"Multiple Projectile Muzzle");
	m_MultipleProjectileMuzzle->SetupAttachment(m_MultipleProjectileMuzzleRootComponent);
	m_MultipleProjectileMuzzle->SetRelativeLocation(FVector(45.f, 0.f, 0.f));

	m_TheWorldComponent = CreateDefaultSubobject<UTheWorldComponent>(L"The World Component");

	GetCharacterMovement()->GravityScale = 0.f;

	m_BossName = L"Yoo Seung-chan T";
	m_bIsChangePhase = false;
	m_fLastTheWorldTime = 0.f;
	m_fDelayTime = FMath::FRandRange(20.f, 30.f);

	m_fHealth = MaxLastBossHealth;
	PrimaryActorTick.bCanEverTick = true;
}

void ALastBossCharacter::PostInitializeComponents() {
	Super::PostInitializeComponents();

	if (m_HeadPanelMeshComponent->IsValidLowLevelFast()) {
		m_HeadPanelMeshComponent->SetMaterial(0, UMaterialInstanceDynamic::Create(m_CubeMaterialNoStrong, nullptr));
	}
}

void ALastBossCharacter::BeginPlay() {
	Super::BeginPlay();

	m_PlayerPawnClass = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

	if (m_PoolOwnerComponent->IsValidLowLevelFast()) {
		m_MultipleHomingMissilePtr = MakeShared<TArray<ABasePooling*>*>(m_PoolOwnerComponent->m_PoolObjects.Find("MultipleHomingProjectile"));

		if (m_MultipleHomingMissilePtr && m_MultipleHomingMissilePtr.Get()) {
			GetWorld()->GetTimerManager().SetTimer(m_TH, this, &ALastBossCharacter::FireMultipleHomingMissileCallback, 5.f, true);
		}
	}
}

void ALastBossCharacter::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	if (IsValid(m_PlayerPawnClass) && GetMesh()) {
		if (ULBAnimInstance* Anim = Cast<ULBAnimInstance>(GetMesh()->GetAnimInstance())) {
			FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), m_PlayerPawnClass->GetActorLocation());
			float CurrentYawRotation = Anim->GetYawRotation();
			
			if (m_MultipleProjectileMuzzleRootComponent->IsValidLowLevelFast()) {
				m_MultipleProjectileMuzzleRootComponent->SetRelativeRotation(FRotator(0.f, LookAtRotation.Yaw, 0.f));
			}
			if (UKismetMathLibrary::InRange_FloatFloat(LookAtRotation.Yaw, -75.f, 75.f)) {
				CalculateHeadRotation(Anim, CurrentYawRotation, LookAtRotation.Yaw * -1, DeltaTime, 1.5f);
			}
			else {
				if (!FMath::IsNearlyZero(CurrentYawRotation)) {
					CalculateHeadRotation(Anim, CurrentYawRotation, 0.f, DeltaTime);
				}
			}
		}
	}
	UseTheWorld();
}

float ALastBossCharacter::TakeDamage(float DamageAmount, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser) {
	float Damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	
	if (m_fHealth - Damage <= 0.f) {
		if (m_TH.IsValid()) {
			GetWorld()->GetTimerManager().ClearTimer(m_TH);
		}
		SetActorLocation(FVector(GetActorLocation().X, GetActorLocation().Y, 10000.f));
		if (ULBAnimInstance* LBAnim = Cast<ULBAnimInstance>(GetMesh()->GetAnimInstance())) {
			LBAnim->SetIsDead(true);
		}
		SetActorTickEnabled(false);
		SetActorEnableCollision(false);
	}

	if (!m_bIsChangePhase) {
		if (m_fHealth <= (::MaxLastBossHealth / 2.f)) {
			if (m_HeadPanelMeshComponent->IsValidLowLevelFast()) {
				m_HeadPanelMeshComponent->SetMaterial(0, UMaterialInstanceDynamic::Create(m_CubeMaterialStrong, nullptr));
				m_BossName = L"Very strong Yoo Seung-chan T";
				m_bIsChangePhase = true;
			}
		}
	}

	m_fHealth -= Damage;
	return Damage;
}

void ALastBossCharacter::FireMultipleHomingMissileCallback() {
	if ((*m_MultipleHomingMissilePtr)->Num() > 0 && m_MultipleProjectileMuzzle) {
		AMultipleHomingMissile* Actor = Cast<AMultipleHomingMissile>((*m_MultipleHomingMissilePtr)->Pop());
		if (Actor) {
			Actor->SetActorLocationAndRotation(m_MultipleProjectileMuzzle->GetComponentLocation(), m_MultipleProjectileMuzzle->GetComponentLocation().Rotation());
			Actor->Activate(this, true);
		}
	}
}

void ALastBossCharacter::UseTheWorld() {
	if (GetWorld()->GetTimeSeconds() - m_fLastTheWorldTime > m_fDelayTime) {
		if (m_TheWorldComponent->IsValidLowLevelFast()) {
			m_TheWorldComponent->StartTheWorld(GetActorLocation());
		}
		m_fLastTheWorldTime = GetWorld()->GetTimeSeconds();
		m_fDelayTime = FMath::FRandRange(20.f, 30.f);
	}
}