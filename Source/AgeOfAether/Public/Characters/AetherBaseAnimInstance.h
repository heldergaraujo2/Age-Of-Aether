#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Characters/AetherCharacterAnimationProfile.h"
#include "AetherBaseAnimInstance.generated.h"

UCLASS(Blueprintable)
class AGEOFAETHER_API UAetherBaseAnimInstance : public UAnimInstance
{
    GENERATED_BODY()

public:
    virtual void NativeUpdateAnimation(float DeltaSeconds) override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Age of Aether|Animation")
    TObjectPtr<UAetherCharacterAnimationProfile> AnimationProfile;

    UPROPERTY(BlueprintReadOnly, Category="Age of Aether|Animation")
    float GroundSpeed = 0.0f;

    UPROPERTY(BlueprintReadOnly, Category="Age of Aether|Animation")
    float Direction = 0.0f;

    UPROPERTY(BlueprintReadOnly, Category="Age of Aether|Animation")
    float VerticalSpeed = 0.0f;

    UPROPERTY(BlueprintReadOnly, Category="Age of Aether|Animation")
    bool bIsInAir = false;

    UPROPERTY(BlueprintReadOnly, Category="Age of Aether|Animation")
    bool bIsSprinting = false;

    UPROPERTY(BlueprintReadOnly, Category="Age of Aether|Animation")
    EAetherAnimationMovementState MovementState = EAetherAnimationMovementState::Idle;

    UFUNCTION(BlueprintPure, Category="Age of Aether|Animation")
    EAetherAnimationMovementState GetMovementState() const { return MovementState; }

private:
    void RefreshMovementState(const class ACharacter* Character);
};