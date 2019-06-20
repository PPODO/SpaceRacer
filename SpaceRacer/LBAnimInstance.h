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

public:
	FORCEINLINE void SetYawRotation(const float& YawRotation);
	FORCEINLINE void AddYawRotation(const float& YawRotation) { m_YawRotation += YawRotation; }

public:
	FORCEINLINE float GetYawRotation() const { return m_YawRotation; }
	
};