#include "MultipleHomingMissile.h"
#include "PoolObjectOwnerComponent.h"
#include "HomingMissile.h"
#include "Kismet/GameplayStatics.h"

static const int32 MaxHomingProjectile = 12;

AMultipleHomingMissile::AMultipleHomingMissile() {
	m_ObjectName = "MultipleHomingProjectile";

	m_RootComponent = CreateDefaultSubobject<USceneComponent>(L"Root Component");
	RootComponent = m_RootComponent;

	m_PoolOwnerComponent = CreateDefaultSubobject<UPoolObjectOwnerComponent>(L"Pool Owner Component");
	if (m_PoolOwnerComponent) {
		m_PoolOwnerComponent->AddNewObjectType("DefaultHomingProjectile", MaxHomingProjectile);
	}

	m_fLifeSpanTime = 0.f;
	m_fMissileDistance = 500.f;
	PrimaryActorTick.bStartWithTickEnabled = false;
	PrimaryActorTick.SetTickFunctionEnable(false);
}

void AMultipleHomingMissile::Tick(float DeltaTime) {
	ABasePooling::Tick(DeltaTime);

	if (m_CurrentReleasedHomingProjectile >= MaxHomingProjectile) {
		Release();
	}
}

void AMultipleHomingMissile::Activate(AActor * Owner, bool bUseTick) {
	ABasePooling::Activate(Owner, bUseTick);

	if (m_PoolOwnerComponent) {
		auto ArrayPtr = m_PoolOwnerComponent->m_PoolObjects.Find("DefaultHomingProjectile");
		if (ArrayPtr) {
			TSharedRef<TArray<ABasePooling*>*> HomingMissilePtr = MakeShared<TArray<ABasePooling*>*>(ArrayPtr);

			const int32 HomingMissileCount = (*HomingMissilePtr)->Num();
			for (int32 i = 0; i < HomingMissileCount; i++) {
				float CurrentAngle = FMath::DegreesToRadians(i * (360.f / float(HomingMissileCount)));
				AHomingMissile* Projectile = Cast<AHomingMissile>((*HomingMissilePtr)->Pop());
				if (Projectile) {
					FVector Location = FVector(0.f, FMath::Cos(CurrentAngle) * m_fMissileDistance, FMath::Sin(CurrentAngle) * m_fMissileDistance);
					Projectile->SetActorLocationAndRotation(GetActorLocation() + Location, FRotator(0.f, GetActorRotation().Yaw, 0.f) + Location.Rotation(), false, nullptr, ETeleportType::ResetPhysics);
					Projectile->SetTarget(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
					Projectile->Activate(this);
				}
			}
		}
	}
}

void AMultipleHomingMissile::DeActivate() {
	ABasePooling::DeActivate();

	m_CurrentReleasedHomingProjectile = 0;
}