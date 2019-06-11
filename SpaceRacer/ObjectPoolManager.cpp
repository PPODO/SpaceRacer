#include "ObjectPoolManager.h"
#include "Engine/World.h"

AObjectPoolManager::AObjectPoolManager() {
	PrimaryActorTick.bCanEverTick = false;
}

void AObjectPoolManager::PostInitializeComponents() {
	Super::PostInitializeComponents();

	for (auto Iterator : m_PoolingType) {
		for (uint32 i = 0; i < Iterator.m_MaxSize; i++) {
			AActor* Data = GetWorld()->SpawnActor(Iterator.m_ObjectClassType);
			check(Data);
			// Deactivate Actor

			Iterator.AddNewObject(MakeShared<AActor*>(Data));
		}
	}
}

void AObjectPoolManager::GetObjectFromPoolList(TArray<TWeakPtr<AActor*>>& List, const FName & ObjectName, const uint32 & Count) {


}