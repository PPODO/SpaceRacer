#include "PoolObjectOwnerComponent.h"
#include "ObjectPoolManager.h"
#include "Engine/Engine.h"
#include "Kismet/GameplayStatics.h"

UPoolObjectOwnerComponent::UPoolObjectOwnerComponent() {
	PrimaryComponentTick.bCanEverTick = false;
	bWantsInitializeComponent = true;
}

void UPoolObjectOwnerComponent::InitializeComponent() {
	Super::InitializeComponent();

	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AObjectPoolManager::StaticClass(), Actors);
	m_PoolManager = Actors.Num() > 0 ? Cast<AObjectPoolManager>(Actors.Pop()) : nullptr;

	if (m_PoolManager) {
		for (auto Iterator : m_ObjectInformation) {
			TArray<ABasePooling*> Actors;
			m_PoolManager->GetObjectFromPoolList(Actors, Iterator.Key, Iterator.Value);
			if (Actors.Num() > 0) {
				m_PoolObjects.Add(Iterator.Key, Actors);
			}
		}
	}
}

void UPoolObjectOwnerComponent::AddNewObjectType(const FName & ObjectName, const uint32& Count) {
	m_ObjectInformation.Add(TPair<FName, uint32>(ObjectName, Count));
}

void UPoolObjectOwnerComponent::ReleasePoolObject(const FName & ObjectName, ABasePooling* Object) {
	auto Iterator = m_PoolObjects.Find(ObjectName);
	if (Iterator) {
		Iterator->Add(Object);
	}
}