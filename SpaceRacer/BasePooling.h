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
	virtual void Tick(float DeltaTime) override;

public:
	virtual void Activate(AActor* Owner, bool bUseTick = true);
	virtual void DeActivate();

protected:
	void ReleaseThisObject(const FName& ObjectName);

protected:
	FName m_ObjectName;
	float m_fLifeSpanTime;

private:
	float m_fElapsedTime;

public:
	FORCEINLINE void Release() {
		ReleaseThisObject(m_ObjectName);
		DeActivate();
	}

public:
	FORCEINLINE void SetObjectName(const FName& ObjectName) { m_ObjectName = ObjectName; };

};