#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TheWorldComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SPACERACER_API UTheWorldComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UTheWorldComponent();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleDefaultsOnly)
		class UTimelineComponent* m_EnableTheWorldComponent;
	UPROPERTY(VisibleDefaultsOnly)
		class UTimelineComponent* m_DisableTheWorldComponent;
	UPROPERTY(VisibleDefaultsOnly)
		class UTimelineComponent* m_EnableTheWorldMaterial;
	UPROPERTY(VisibleDefaultsOnly)
		class UTimelineComponent* m_DisableTheWorldMaterial;

private:
	UPROPERTY()
		class USoundCue* m_AbilitySound;
	UPROPERTY()
		class USoundCue* m_TimeReverseSound;
	UPROPERTY()
		class USoundCue* m_CheckMateSound;

private:
	TSharedPtr<class APostProcessVolume*> m_PostProcessClass;

private:
	class UMaterialParameterCollection* m_ParamCollection;

private:
	TArray<AActor*> m_ExceptionActors;

private:
	bool m_bIsSpawnCheckMate;
	bool m_bIsReverse;
	bool m_bIsEnableTheWorld;

private:
	UFUNCTION()
		void UpdateWorldTime(float Value);
	UFUNCTION()
		void EndDisableTheWorld();
	UFUNCTION()
		void EndEnableTheWorld();

private:
	UFUNCTION()
		void UpdateMaterialParam(float Value);
	UFUNCTION()
		void EndEnableMaterialParam();
	UFUNCTION()
		void StartControlWorldTime();

public:
	void StartTheWorld(const FVector& WorldLocation);

};