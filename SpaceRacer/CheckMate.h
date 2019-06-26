#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CheckMate.generated.h"

UCLASS()
class SPACERACER_API ACheckMate : public AActor
{
	GENERATED_BODY()
	
public:
	ACheckMate();

private:
	UPROPERTY(VisibleDefaultsOnly)
		class USceneComponent* m_SceneComponent;

	UPROPERTY()
		TArray<class UChildActorComponent*> m_ChildActorComponents;

private:
	float m_fKnifeDistance;

};
