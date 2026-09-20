#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "AetherCharacterAnimationProfile.generated.h"

class UAnimInstance;
class UAnimSequenceBase;

UENUM(BlueprintType)
enum class EAetherAnimationMovementState : uint8
{
    Idle,
    Walk,
    Run,
    Jump,
    Fall
};

UCLASS(BlueprintType)
class AGEOFAETHER_API UAetherCharacterAnimationProfile : public UDataAsset
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Age of Aether|Animation")
    FName AnimationProfileID;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Age of Aether|Animation")
    TSoftClassPtr<UAnimInstance> AnimationClass;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Age of Aether|Animation", meta=(ClampMin="0.0"))
    float WalkSpeedThreshold = 5.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Age of Aether|Animation", meta=(ClampMin="0.0"))
    float RunSpeedThreshold = 500.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Age of Aether|Animation")
    TSoftObjectPtr<UAnimSequenceBase> IdleAnimation;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Age of Aether|Animation")
    TSoftObjectPtr<UAnimSequenceBase> WalkAnimation;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Age of Aether|Animation")
    TSoftObjectPtr<UAnimSequenceBase> RunAnimation;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Age of Aether|Animation")
    TSoftObjectPtr<UAnimSequenceBase> JumpAnimation;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Age of Aether|Animation")
    TSoftObjectPtr<UAnimSequenceBase> FallAnimation;

    UFUNCTION(BlueprintPure, Category="Age of Aether|Animation")
    bool IsConfigured() const;

    UFUNCTION(BlueprintCallable, Category="Age of Aether|Animation")
    bool ValidateProfile(FString& OutError) const;
};