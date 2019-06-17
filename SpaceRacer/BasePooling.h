#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BasePooling.generated.h"

UCLASS()
class SPACERACER_API ABasePooling : public AActor
{
	GENERATED_BODY()
	
public:
	ABasePooling();

public:
	virtual void Activate(AActor* Owner, bool bUseTick = true);
	virtual void DeActivate();

};