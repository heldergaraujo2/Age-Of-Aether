#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Aether2DDepthLightingProfile.generated.h"

UENUM(BlueprintType)
enum class EAether2DDepthLayer : uint8
{
    Background,
    Far,
    World,
    Midground,
    Foreground,
    Overlay
};

UCLASS(BlueprintType)
class AGEOFAETHER_API UAether2DDepthLightingProfile : public UDataAsset
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Age of Aether|2D Depth")
    FName ProfileID;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Age of Aether|2D Depth")
    EAether2DDepthLayer DepthLayer = EAether2DDepthLayer::World;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Age of Aether|2D Depth")
    int32 RenderLayer = 0;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Age of Aether|2D Depth")
    float ParallaxFactor = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Age of Aether|2D Depth")
    float DepthOffset = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Age of Aether|2D Depth")
    bool bCastShadow = false;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Age of Aether|2D Depth")
    bool bEnableParallax = false;

    UFUNCTION(BlueprintPure, Category = "Age of Aether|2D Depth")
    bool IsConfigured() const;

    UFUNCTION(BlueprintCallable, Category = "Age of Aether|2D Depth")
    bool ValidateProfile(FString& OutError) const;
};
