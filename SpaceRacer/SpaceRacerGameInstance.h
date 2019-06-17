#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "SpaceRacerGameInstance.generated.h"

UCLASS()
class SPACERACER_API USpaceRacerGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	USpaceRacerGameInstance();

protected:
	virtual void Init() override;

public:
	static class AObjectPoolManager* m_ObjectPoolManagerInstance;

public:
	static AObjectPoolManager* GetObjectPoolInst() {
		if (m_ObjectPoolManagerInstance) {
			return m_ObjectPoolManagerInstance;
		}
		return nullptr;
	}

};