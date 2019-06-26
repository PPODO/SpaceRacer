#include "KnifeProjectile.h"
#include "ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

AKnifeProjectile::AKnifeProjectile() {
	if (m_CollisionComponent) {
		m_CollisionComponent->SetBoxExtent(FVector(30.f, 5.f, 0.75f));
		m_CollisionComponent->OnComponentHit.AddDynamic(this, &AKnifeProjectile::OnComponentHit);
		RootComponent = m_CollisionComponent;
	}

	::ConstructorHelpers::FObjectFinder<UStaticMesh> MeshObject(L"StaticMesh'/Game/FPS_Weapon_Bundle/Weapons/Meshes/M9_Knife/SM_M9_Knife_X.SM_M9_Knife_X'");
	if (MeshObject.Succeeded() && m_ProjectileMesh) {
		m_ProjectileMesh->SetRelativeLocation(FVector(-15.f, 0.f, 0.f));
		m_ProjectileMesh->SetRelativeRotation(FRotator(0.f, 90.f, 90.f));
		m_ProjectileMesh->SetWorldScale3D(FVector(2.f));
		m_ProjectileMesh->SetStaticMesh(MeshObject.Object);
	}

	if (m_MovementComponent) {
		m_MovementComponent->InitialSpeed = 5000.f;
		m_MovementComponent->MaxSpeed = 5000.f;
		m_MovementComponent->ProjectileGravityScale = 0.f;
	}

	m_fBaseDamage = 10.f;
	PrimaryActorTick.bCanEverTick = true;
}

void AKnifeProjectile::BeginPlay() {
	Super::BeginPlay();

	if (APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0)) {
		SetActorRotation(UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), PlayerPawn->GetActorLocation()));
	}
	m_fCurrentVelocity = FMath::FRandRange(10.f, 15.f);
	m_fInterpSpeed = FMath::FRandRange(3.f, 5.5f);
}

void AKnifeProjectile::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	if (!FMath::IsNearlyZero(m_fCurrentVelocity)) {
		m_fCurrentVelocity = UKismetMathLibrary::FInterpTo(m_fCurrentVelocity, 0.f, 0.013f, m_fInterpSpeed);
		SetActorLocation(GetActorLocation() + (GetActorForwardVector() * m_fCurrentVelocity));
	}
}

void AKnifeProjectile::OnComponentHit(UPrimitiveComponent * HitComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, FVector NormalImpulse, const FHitResult & Hit) {
	if (OtherActor != this) {
		UGameplayStatics::ApplyDamage(OtherActor, m_fBaseDamage, nullptr, this, UDamageType::StaticClass());
	}
	DeActivate();
}

void AKnifeProjectile::DeActivate() {
	Destroy();
}