#include "ObjectPoolManager.h"
#include "Engine/World.h"

AObjectPoolManager::AObjectPoolManager() {
	PrimaryActorTick.bCanEverTick = false;
}

void AObjectPoolManager::PostInitializeComponents() {
	Super::PostInitializeComponents();

	for (auto Iterator : m_PoolingType) {
		for (int32 i = 0; i < Iterator.m_MaxSize; i++) {
			ABasePooling* Data = Cast<ABasePooling>(GetWorld()->SpawnActor(Iterator.m_ObjectClassType));
			check(Data);
			Data->DeActivate();

			Iterator.AddNewObjectToPoolList(Data);
		}
	}
}

void AObjectPoolManager::GetObjectFromPoolList(TArray<ABasePooling*>& List, const FName & ObjectName, const int32 & Count) {
	auto Result = m_PoolingType.FindByPredicate([&ObjectName](const FObjectPoolType& Value) { if (Value.m_ObjectName == ObjectName) { return true; } return false; });
	if (Result) {
		Result->GetObjectFromPoolList(List, (Count < 0 ? 0 : Count));
	}
}