#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BasePooling.h"
#include "Engine/World.h"
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
	
	template<typename T>
	FORCEINLINE void GetObjectFromPoolList(TArray<T*>& List, const int32& Count) {
		if (m_PoolList.Num() > 0) {
			int32 ObjectCount = GetPoolListSize() < Count ? GetPoolListSize() : Count;

			for (int32 i = 0; i < ObjectCount; i++) {
				T* Object = Cast<T>(m_PoolList.Pop());
				if (IsValid(Object)) {
					List.Push(Object);
				}
			}
		}
	}

	template<typename T>
	FORCEINLINE void ReleaseObjectToPoolList(TArray<T*>& List) {
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
	void AddNewObjectPoolType(const FObjectPoolType& NewObjectType);

public:
	template<typename T>
	void GetObjectFromPoolList(TArray<T*>& List, const FName& ObjectName, const int32& Count) {
		FObjectPoolType* Result = m_PoolingType.FindByPredicate([&ObjectName](const FObjectPoolType& Value) { if (Value.m_ObjectName == ObjectName) { return true; } return false; });
		if (Result) {
			Result->GetObjectFromPoolList<T>(List, (Count > 0 ? Count : 0));
		}
	}

	template<typename T>
	void ReleaseObjectToPoolList(TArray<T*>& List, const FName& ObjectName) {
		FObjectPoolType* Result = m_PoolingType.FindByPredicate([&ObjectName](const FObjectPoolType& Value) { if (Value.m_ObjectName == ObjectName) { return true; } return false; });
		if (Result) {
			Result->ReleaseObjectToPoolList<T>(List);
		}
	}

};