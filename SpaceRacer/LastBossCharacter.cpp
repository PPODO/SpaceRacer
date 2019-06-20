#include "LastBossCharacter.h"
#include "MultipleHomingMissile.h"
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
	::ConstructorHelpers::FObjectFinder<UMaterial> CubeMaterial(L"Material'/Game/Texture/LB_Head_Mat.LB_Head_Mat'");

	if (CubeObject.Succeeded() && CubeMaterial.Succeeded()) {
		m_HeadPanelMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(L"Panel Mesh Component");
		m_HeadPanelMeshComponent->SetupAttachment(GetMesh(), "head");
		m_HeadPanelMeshComponent->SetRelativeLocation(FVector(3.f, 14.f, 0.f));
		m_HeadPanelMeshComponent->SetRelativeRotation(FRotator(-90.f, 0.f, 0.f));
		m_HeadPanelMeshComponent->SetWorldScale3D(FVector(0.25f, 0.0025f, 0.25f));
		m_HeadPanelMeshComponent->SetStaticMesh(CubeObject.Object);
		m_CubeMaterial = CubeMaterial.Object;
	}

	m_PoolOwnerComponent = CreateDefaultSubobject<UPoolObjectOwnerComponent>(L"Pool Object Owner Component");
	m_PoolOwnerComponent->AddNewObjectType("MultipleHomingProjectile", 1);

	m_MultipleProjectileMuzzle = CreateDefaultSubobject<USceneComponent>(L"Multiple Projectile Muzzle");
	m_MultipleProjectileMuzzle->SetupAttachment(RootComponent);
	m_MultipleProjectileMuzzle->SetRelativeLocation(FVector(-45.f, 0.f, 45.f));

	GetCharacterMovement()->GravityScale = 0.f;

	PrimaryActorTick.bCanEverTick = true;
}

void ALastBossCharacter::PostInitializeComponents() {
	Super::PostInitializeComponents();

	if (m_HeadPanelMeshComponent->IsValidLowLevelFast()) {
		m_HeadPanelMeshComponent->SetMaterial(0, UMaterialInstanceDynamic::Create(m_CubeMaterial, nullptr));
	}
}

void ALastBossCharacter::BeginPlay() {
	Super::BeginPlay();

	m_PlayerPawnClass = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

	if (m_PoolOwnerComponent->IsValidLowLevelFast()) {
		m_MultipleHomingMissilePtr = MakeShared<TArray<ABasePooling*>*>(m_PoolOwnerComponent->m_PoolObjects.Find("MultipleHomingProjectile"));

		if (m_MultipleHomingMissilePtr && m_MultipleHomingMissilePtr.Get()) {
			GetWorld()->GetTimerManager().SetTimer(m_TH, this, &ALastBossCharacter::TimerCallback, 5.f, true);
		}
	}
}

void ALastBossCharacter::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	if (m_PlayerPawnClass && GetMesh()) {
		if (ULBAnimInstance* Anim = Cast<ULBAnimInstance>(GetMesh()->GetAnimInstance())) {
			FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), m_PlayerPawnClass->GetActorLocation());
			float CurrentYawRotation = Anim->GetYawRotation();
			
			if (m_MultipleProjectileMuzzle) {
				m_MultipleProjectileMuzzle->SetWorldRotation(FRotator(0.f, LookAtRotation.Yaw * -1, 0.f));
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
}

void ALastBossCharacter::TimerCallback() {
	if ((*m_MultipleHomingMissilePtr)->Num() > 0 && m_MultipleProjectileMuzzle) {
		AMultipleHomingMissile* Actor = Cast<AMultipleHomingMissile>((*m_MultipleHomingMissilePtr)->Pop());
		if (Actor) {
			Actor->SetActorLocationAndRotation(m_MultipleProjectileMuzzle->GetComponentLocation(), m_MultipleProjectileMuzzle->GetComponentRotation());
			Actor->Activate(this, true);
		}
	}
}