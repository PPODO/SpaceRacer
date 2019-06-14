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

protected:
	float m_fLifeSpan;

public:
	virtual void Activate();
	virtual void DeActivate();

};