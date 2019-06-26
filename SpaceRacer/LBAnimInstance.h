#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "LBAnimInstance.generated.h"

UCLASS()
class SPACERACER_API ULBAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

private:
	UPROPERTY(BlueprintReadOnly, Meta = (AllowPrivateAccess = "true"))
		float m_YawRotation;
	UPROPERTY(BlueprintReadOnly, Meta = (AllowPrivateAccess = "true"))
		bool m_bIsDead;

public:
	FORCEINLINE void SetYawRotation(const float& YawRotation) { m_YawRotation = YawRotation; }
	FORCEINLINE void AddYawRotation(const float& YawRotation) { m_YawRotation += YawRotation; }

	FORCEINLINE void SetIsDead(const bool& Dead) { m_bIsDead = Dead; }

public:
	FORCEINLINE float GetYawRotation() const { return m_YawRotation; }
	
};