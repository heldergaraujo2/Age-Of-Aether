#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "AetherPlayableCharacterVisualProfile.generated.h"

class UAnimInstance;
class UAetherCharacterAnimationProfile;
class UMaterialInterface;
class USkeletalMesh;

/**
 * Small, editor-friendly visual contract for a playable character.
 * Gameplay authority stays in AAetherCharacter; this asset only describes presentation.
 */
UCLASS(BlueprintType)
class AGEOFAETHER_API UAetherPlayableCharacterVisualProfile : public UDataAsset
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Age of Aether|Visual", meta = (DisplayName = "Stable Visual Profile ID"))
    FName VisualProfileID;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Age of Aether|Visual")
    TSoftObjectPtr<USkeletalMesh> SkeletalMesh;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Age of Aether|Animation")
    TSoftClassPtr<UAnimInstance> AnimationClass;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Age of Aether|Animation")
    TObjectPtr<UAetherCharacterAnimationProfile> AnimationProfile;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Age of Aether|Visual")
    TArray<TSoftObjectPtr<UMaterialInterface>> MaterialOverrides;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Age of Aether|Visual")
    FTransform MeshRelativeTransform = FTransform::Identity;

    UFUNCTION(BlueprintPure, Category = "Age of Aether|Visual")
    bool IsConfigured() const;

    UFUNCTION(BlueprintCallable, Category = "Age of Aether|Visual")
    bool ValidateProfile(FString& OutError) const;
};
