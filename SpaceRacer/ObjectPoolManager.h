#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BasePooling.h"
#include "ObjectPoolManager.generated.h"

USTRUCT()
struct FObjectPoolType {
	GENERATED_USTRUCT_BODY()

public:
	FObjectPoolType() : m_ObjectName(L""), m_ObjectClassType(nullptr), m_MaxSize(0) {};
	FObjectPoolType(const FName& ObjectName, const TSubclassOf<AActor>& ClassType, const int32& MaxSize) : m_ObjectName(ObjectName), m_ObjectClassType(ClassType), m_MaxSize(MaxSize) {};

public:
	UPROPERTY(EditAnywhere)
		FName m_ObjectName;
	UPROPERTY(EditAnywhere)
		TSubclassOf<AActor> m_ObjectClassType;
	UPROPERTY(EditAnywhere)
		int32 m_MaxSize;

private:
	UPROPERTY()
		TArray<ABasePooling*> m_PoolList;

public:
	FORCEINLINE void AddNewObjectToPoolList(const ABasePooling* Object) {
		m_PoolList.Push(const_cast<ABasePooling*>(Object));
	}
	
	FORCEINLINE void GetObjectFromPoolList(TArray<ABasePooling*>& List, const int32& Count) {
		int32 ObjectCount = GetPoolListSize() <= Count ? GetPoolListSize() : Count;
		for (int32 i = 0; i < ObjectCount; i++) {
			ABasePooling* Object = m_PoolList.Pop();
			if (IsValid(Object)) {
				List.Push(Object);
			}
		}
	}

	FORCEINLINE void ReleaseObjectToPoolList(TArray<ABasePooling*>& List) {
		for (auto Iterator : List) {
			if (IsValid(Iterator)) {
				m_PoolList.Push(Iterator);
			}
		}
		List.Empty();
	}

	FORCEINLINE int32 GetPoolListSize() const { return m_PoolList.Num(); }

public:
	~FObjectPoolType() {
		m_PoolList.Empty();
		m_ObjectClassType = nullptr;
		m_ObjectName = "";
		m_MaxSize = 0;
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
	void GetObjectFromPoolList(TArray<ABasePooling*>& List, const FName& ObjectName, const int32& Count);

};