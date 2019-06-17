#include "SpaceRacerGameInstance.h"
#include "ObjectPoolManager.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"

AObjectPoolManager* USpaceRacerGameInstance::m_ObjectPoolManagerInstance = nullptr;

USpaceRacerGameInstance::USpaceRacerGameInstance() {
}

void USpaceRacerGameInstance::Init() {
	Super::Init();

	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AObjectPoolManager::StaticClass(), Actors);

	if (Actors.Num() > 0) {
		AActor* Actor = Actors.Pop();
		if (Actor) {
			m_ObjectPoolManagerInstance = Actor->IsA<AObjectPoolManager>() ? Cast<AObjectPoolManager>(Actor) : nullptr;
		}
	}
}