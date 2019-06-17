#include "ObjectPoolManager.h"
#include "Engine/World.h"

AObjectPoolManager::AObjectPoolManager() {
	PrimaryActorTick.bCanEverTick = false;
}

void AObjectPoolManager::PostInitializeComponents() {
	Super::PostInitializeComponents();

	for (auto& Iterator : m_PoolingType) {
		for (int32 i = 0; i < Iterator.m_MaxSize; i++) {
			ABasePooling* Data = Cast<ABasePooling>(GetWorld()->SpawnActor(Iterator.m_ObjectClassType));
			check(Data);
			Data->SetObjectName(Iterator.m_ObjectName);
			Data->DeActivate();

			Iterator.AddNewObjectToPoolList(Data);
		}
	}
}

void AObjectPoolManager::AddNewObjectPoolType(const FObjectPoolType & NewObjectType) {
	m_PoolingType.Add(NewObjectType);
}