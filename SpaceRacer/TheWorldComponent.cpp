#include "TheWorldComponent.h"
#include "CheckMate.h"
#include "Components/TimelineComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Curves/CurveFloat.h"
#include "ConstructorHelpers.h"
#include "Materials/MaterialParameterCollection.h"
#include "Materials/MaterialParameterCollectionInstance.h"
#include "Engine/PostProcessVolume.h"
#include "Engine/World.h"
#include "Sound/SoundCue.h"

UTheWorldComponent::UTheWorldComponent() {
	::ConstructorHelpers::FObjectFinder<USoundCue> AbilitySound(L"SoundCue'/Game/ProjectileSounds/TheWorld_Cue.TheWorld_Cue'");
	if (AbilitySound.Succeeded()) {
		m_AbilitySound = AbilitySound.Object;
	}

	::ConstructorHelpers::FObjectFinder<USoundCue> TimeReverseSound(L"SoundCue'/Game/ProjectileSounds/TimeReverse_Cue.TimeReverse_Cue'");
	if (TimeReverseSound.Succeeded()) {
		m_TimeReverseSound = TimeReverseSound.Object;
	}

	::ConstructorHelpers::FObjectFinder<USoundCue> CheckMateSound(L"SoundCue'/Game/ProjectileSounds/CheckMate_Cue.CheckMate_Cue'");
	if (CheckMateSound.Succeeded()) {
		m_CheckMateSound = CheckMateSound.Object;
	}

	::ConstructorHelpers::FObjectFinder<UMaterialParameterCollection> ParamCollection(L"'/Game/Blueprint/Materials/TheWorldMaterialParam'");
	if (ParamCollection.Succeeded()) {
		m_ParamCollection = ParamCollection.Object;
	}

	::ConstructorHelpers::FObjectFinder<UCurveFloat> EnableCurve(L"CurveFloat'/Game/Blueprint/Timeline/EnableStopTimeCurve.EnableStopTimeCurve'");
	if (EnableCurve.Succeeded()) {
		FOnTimelineFloat EnableTimeline;
		EnableTimeline.BindUFunction(this, L"UpdateWorldTime");
		FOnTimelineEvent EnableTimelineFinishedEvent;
		EnableTimelineFinishedEvent.BindUFunction(this, L"EndEnableTheWorld");

		m_EnableTheWorldComponent = CreateDefaultSubobject<UTimelineComponent>(L"Enable The World Component");
		m_EnableTheWorldComponent->AddInterpFloat(EnableCurve.Object, EnableTimeline);
		m_EnableTheWorldComponent->SetTimelineFinishedFunc(EnableTimelineFinishedEvent);
	}

	::ConstructorHelpers::FObjectFinder<UCurveFloat> DisableCurve(L"CurveFloat'/Game/Blueprint/Timeline/DisableStopTimeCurve.DisableStopTimeCurve'");
	if (DisableCurve.Succeeded()) {
		FOnTimelineFloat DisableTimeline;
		DisableTimeline.BindUFunction(this, L"UpdateWorldTime");
		FOnTimelineEvent DisableTimelineFinishedEvent;
		DisableTimelineFinishedEvent.BindUFunction(this, L"EndDisableTheWorld");

		m_DisableTheWorldComponent = CreateDefaultSubobject<UTimelineComponent>(L"Disable The World Component");
		m_DisableTheWorldComponent->AddInterpFloat(DisableCurve.Object, DisableTimeline);
		m_DisableTheWorldComponent->SetTimelineFinishedFunc(DisableTimelineFinishedEvent);
	}

	::ConstructorHelpers::FObjectFinder<UCurveFloat> EnableMaterialCurve(L"CurveFloat'/Game/Blueprint/Timeline/EnableCutoffCurve.EnableCutoffCurve'");
	if (EnableMaterialCurve.Succeeded()) {
		FOnTimelineFloat EnableTimeline;
		EnableTimeline.BindUFunction(this, L"UpdateMaterialParam");
		FOnTimelineEvent EnableTimelineFinishedEvent;
		EnableTimelineFinishedEvent.BindUFunction(this, L"EndEnableMaterialParam");

		m_EnableTheWorldMaterial = CreateDefaultSubobject<UTimelineComponent>(L"Enable The World Component Of Material");
		m_EnableTheWorldMaterial->AddInterpFloat(EnableMaterialCurve.Object, EnableTimeline);
		m_EnableTheWorldMaterial->SetTimelineFinishedFunc(EnableTimelineFinishedEvent);
	}

	::ConstructorHelpers::FObjectFinder<UCurveFloat> DisableMaterialCurve(L"CurveFloat'/Game/Blueprint/Timeline/DisableCutoffCurve.DisableCutoffCurve'");
	if (DisableMaterialCurve.Succeeded()) {
		FOnTimelineFloat DisableTimeline;
		DisableTimeline.BindUFunction(this, L"UpdateMaterialParam");
		FOnTimelineEvent DisableTimelineFinishedEvent;
		DisableTimelineFinishedEvent.BindUFunction(this, L"StartControlWorldTime");

		m_DisableTheWorldMaterial = CreateDefaultSubobject<UTimelineComponent>(L"Disable The World Component Of Material");
		m_DisableTheWorldMaterial->AddInterpFloat(DisableMaterialCurve.Object, DisableTimeline);
		m_DisableTheWorldMaterial->SetTimelineFinishedFunc(DisableTimelineFinishedEvent);
	}

	m_bIsSpawnCheckMate = false;
	m_bIsReverse = false;
	m_bIsEnableTheWorld = false;

	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = false;
}

void UTheWorldComponent::BeginPlay() {
	Super::BeginPlay();

	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APostProcessVolume::StaticClass(), Actors);
	if (Actors.Num() > 0) {
		if (APostProcessVolume* Volume = Cast<APostProcessVolume>(Actors.Pop())) {
			m_PostProcessClass = MakeShared<APostProcessVolume*>(Volume);
		}
	}
}

void UTheWorldComponent::UpdateWorldTime(float Value) {
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), Value);

	if (!m_bIsEnableTheWorld) {
		if (!m_bIsSpawnCheckMate && Value != 0.f) {
			if (APawn* Pawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0)) {
				if (ACheckMate* SpawnActor = GetWorld()->SpawnActor<ACheckMate>(ACheckMate::StaticClass(), Pawn->GetActorLocation() + FVector(0.f, 0.f, 50.f), FRotator::ZeroRotator)) {
					UGameplayStatics::PlaySound2D(GetWorld(), m_CheckMateSound);
					m_ExceptionActors.Add(SpawnActor);
				}
			}
			m_bIsSpawnCheckMate = true;
		}

		if (!m_bIsReverse && m_TimeReverseSound && Value > 0.01f) {
			UGameplayStatics::PlaySound2D(GetWorld(), m_TimeReverseSound);
			m_bIsReverse = true;
		}
	}

	for (auto Iterator : m_ExceptionActors) {
		Iterator->CustomTimeDilation = 1.f / Value;
	}
}

void UTheWorldComponent::EndDisableTheWorld() {
	m_ExceptionActors.Empty();
	m_bIsSpawnCheckMate = false;
	m_bIsReverse = false;
}

void UTheWorldComponent::EndEnableTheWorld() {
	if (m_DisableTheWorldComponent->IsValidLowLevelFast()) {
		m_DisableTheWorldComponent->PlayFromStart();
	}
	m_bIsEnableTheWorld = false;
}

void UTheWorldComponent::UpdateMaterialParam(float Value) {
	if (m_ParamCollection) {
		if (UMaterialParameterCollectionInstance* Instance = GetWorld()->GetParameterCollectionInstance(m_ParamCollection)) {
			Instance->SetScalarParameterValue(L"Radius", Value);
		}
	}
}

void UTheWorldComponent::EndEnableMaterialParam() {
	if (m_DisableTheWorldMaterial->IsValidLowLevelFast()) {
		m_DisableTheWorldMaterial->PlayFromStart();
	}
}

void UTheWorldComponent::StartControlWorldTime() {
	m_bIsEnableTheWorld = true;
	if (m_EnableTheWorldComponent->IsValidLowLevelFast()) {
		m_EnableTheWorldComponent->PlayFromStart();
	}
}

void UTheWorldComponent::StartTheWorld(const FVector& WorldLocation) {
	if (m_EnableTheWorldMaterial->IsValidLowLevelFast()) {
		m_EnableTheWorldMaterial;
		m_EnableTheWorldMaterial->PlayFromStart();
	}
	if (m_ParamCollection) {
		if (UMaterialParameterCollectionInstance* Instance = GetWorld()->GetParameterCollectionInstance(m_ParamCollection)) {
			Instance->SetVectorParameterValue(L"Position", WorldLocation);
		}
	}
	if (m_AbilitySound) {
		UGameplayStatics::PlaySound2D(GetWorld(), m_AbilitySound);
	}
}