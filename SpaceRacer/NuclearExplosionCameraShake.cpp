#include "NuclearExplosionCameraShake.h"

UNuclearExplosionCameraShake::UNuclearExplosionCameraShake() {
	OscillationDuration = 5.f;
	OscillationBlendInTime = 0.5f;
	OscillationBlendOutTime = 0.75f;

	RotOscillation.Pitch.Amplitude = FMath::FRandRange(5.f, 12.5f);
	RotOscillation.Pitch.Frequency = FMath::FRandRange(5.f, 12.5f);

	RotOscillation.Yaw.Amplitude = FMath::FRandRange(5.f, 12.5f);
	RotOscillation.Yaw.Frequency = FMath::FRandRange(5.f, 12.5f);
}