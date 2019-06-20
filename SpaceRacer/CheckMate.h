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

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

};
