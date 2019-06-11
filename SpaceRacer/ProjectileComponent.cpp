#include "ProjectileComponent.h"

UProjectileComponent::UProjectileComponent() {

	PrimaryComponentTick.bCanEverTick = true;
}

void UProjectileComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UProjectileComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

