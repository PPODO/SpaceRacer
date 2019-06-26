#include "CheckMate.h"
#include "KnifeProjectile.h"
#include "Components/ChildActorComponent.h"

static const int32 MaxSwordCount = 36;
static const int32 RepleteCount = 3;

ACheckMate::ACheckMate() {
	m_SceneComponent = CreateDefaultSubobject<USceneComponent>(L"Root Component");
	RootComponent = m_SceneComponent;

	m_fKnifeDistance = 700.f;

	for (int32 i = 0; i < RepleteCount; i++) {
		for (int32 j = 0; j < MaxSwordCount; j++) {
			float CurrentAngle = FMath::DegreesToRadians((float)j * (360.f / (float)MaxSwordCount));
			UChildActorComponent* NewChildActorClass = CreateDefaultSubobject<UChildActorComponent>(*FString::Printf(L"Child Actor Component %d", j + (i * MaxSwordCount)));
			if (NewChildActorClass) {
				float KnifeDistance = m_fKnifeDistance - (75.f * (float)i);
				NewChildActorClass->SetRelativeLocation(FVector(FMath::Cos(CurrentAngle) * KnifeDistance, FMath::Sin(CurrentAngle) * KnifeDistance, 50.f * (float)i));
				NewChildActorClass->SetRelativeRotation(NewChildActorClass->RelativeLocation.Rotation() + FRotator(-180.f, 0.f, 0.f));
				NewChildActorClass->SetupAttachment(RootComponent);
				NewChildActorClass->SetChildActorClass(AKnifeProjectile::StaticClass());
				m_ChildActorComponents.Add(NewChildActorClass);
			}
		}
	}

	InitialLifeSpan = 2.5f;
	PrimaryActorTick.bCanEverTick = true;
}