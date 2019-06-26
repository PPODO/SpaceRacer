#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BasePooling.h"
#include "PoolObjectOwnerComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SPACERACER_API UPoolObjectOwnerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPoolObjectOwnerComponent();

protected:
	virtual void InitializeComponent() override;
	virtual void BeginDestroy() override;

private:
	class AObjectPoolManager* m_PoolManager;
	TArray<TPair<FName, uint32>> m_ObjectInformation;

public:
	TMap<FName, TArray<ABasePooling*>> m_PoolObjects;

public:
	void AddNewObjectType(const FName& ObjectName, const uint32& Count);
	void ReleasePoolObject(const FName& ObjectName, ABasePooling* Object);

};