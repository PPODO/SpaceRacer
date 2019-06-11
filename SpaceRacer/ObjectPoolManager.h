#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ObjectPoolManager.generated.h"

USTRUCT(BlueprintType)
struct FObjectPoolType {
	GENERATED_USTRUCT_BODY()

public:
	FObjectPoolType() : m_ObjectName(L""), m_ObjectClassType(nullptr), m_MaxSize(0) {};
	FObjectPoolType(const FName& ObjectName, const TSubclassOf<AActor>& ClassType, const uint32& MaxSize) : m_ObjectName(ObjectName), m_ObjectClassType(ClassType), m_MaxSize(MaxSize) {};

public:
	UPROPERTY(EditAnywhere)
		FName m_ObjectName;
	UPROPERTY(EditAnywhere)
		TSubclassOf<AActor> m_ObjectClassType;
	UPROPERTY(EditAnywhere)
		uint32 m_MaxSize;

private:
	UPROPERTY()
		TArray<TWeakPtr<AActor*>> m_PoolList;

public:
	FORCEINLINE void AddNewObject(const TWeakPtr<AActor*>& Object) {
		m_PoolList.Add(Object);
	}

};

UCLASS()
class SPACERACER_API AObjectPoolManager : public AActor
{
	GENERATED_BODY()
	
public:
	AObjectPoolManager();

protected:
	virtual void PostInitializeComponents() override;

private:
	UPROPERTY(EditAnywhere, Category = "Pooling Object", Meta = (AllowPrivateAccess = "true"))
		TArray<FObjectPoolType> m_PoolingType;

public:
	UFUNCTION(BlueprintCallable)
		void GetObjectFromPoolList(TArray<TWeakPtr<AActor*>>& List, const FName& ObjectName, const uint32& Count);


};