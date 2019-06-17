#include "PoolObjectOwnerComponent.h"
#include "SpaceRacerGameInstance.h"
#include "ObjectPoolManager.h"

UPoolObjectOwnerComponent::UPoolObjectOwnerComponent() {
	PrimaryComponentTick.bCanEverTick = false;
}

void UPoolObjectOwnerComponent::BeginPlay() {
	Super::BeginPlay();

	for (auto Iterator : m_ObjectInformation) {
		TArray<ABasePooling*> Actors;
		USpaceRacerGameInstance::GetObjectPoolInst()->GetObjectFromPoolList(Actors, Iterator.Key, Iterator.Value);

		if (Actors.Num() > 0) {
			m_PoolObjects.Add(Iterator.Key, Actors);
		}
	}
}

void UPoolObjectOwnerComponent::AddNewObjectType(const FName & ObjectName, const uint32& Count) {
	m_ObjectInformation.Add(TPair<FName, uint32>(ObjectName, Count));
}

void UPoolObjectOwnerComponent::ReleasePoolObject(const FName & ObjectName, ABasePooling * Object) {
	auto Iterator = m_PoolObjects.Find(ObjectName);
	if (Iterator) {
		Iterator->Add(Object);
	}
}