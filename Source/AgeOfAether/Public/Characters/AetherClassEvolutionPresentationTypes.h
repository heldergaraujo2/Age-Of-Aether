#pragma once
#include "CoreMinimal.h"
#include "AetherClassEvolutionPresentationTypes.generated.h"
class UAetherPlayableCharacterVisualProfile; class UAetherCharacterAnimationProfile; class USoundBase; class UTexture2D; class UFXSystemAsset;
USTRUCT(BlueprintType)
struct AGEOFAETHER_API FAetherClassEvolutionPresentationDefinition
{
 GENERATED_BODY()
 UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Age of Aether|Class Presentation") FName ClassID;
 UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Age of Aether|Class Presentation") FName EvolutionID;
 UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Age of Aether|Class Presentation") FText DisplayName;
 UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Age of Aether|Class Presentation") FText PathTitle;
 UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Age of Aether|Class Presentation") int32 EvolutionStage=0;
 UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Age of Aether|Class Presentation") FName FallbackEvolutionID;
 UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Age of Aether|Class Presentation") TObjectPtr<UAetherPlayableCharacterVisualProfile> VisualProfile=nullptr;
 UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Age of Aether|Class Presentation") TObjectPtr<UAetherCharacterAnimationProfile> AnimationProfile=nullptr;
 UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Age of Aether|Class Presentation") TSoftObjectPtr<UTexture2D> Icon;
 UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Age of Aether|Class Presentation") TSoftObjectPtr<UFXSystemAsset> PresentationVFX;
 UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Age of Aether|Class Presentation") TSoftObjectPtr<USoundBase> PresentationSFX;
 bool IsIdentityValid() const;
};