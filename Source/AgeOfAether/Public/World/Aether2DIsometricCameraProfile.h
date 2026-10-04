#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Aether2DIsometricCameraProfile.generated.h"

UENUM(BlueprintType)
enum class EAether2DIsometricCameraMode : uint8
{
    FixedIsometric,
    Orbit
};

UCLASS(BlueprintType)
class AGEOFAETHER_API UAether2DIsometricCameraProfile : public UDataAsset
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Age of Aether|2D Camera")
    FName ProfileID;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Age of Aether|2D Camera")
    EAether2DIsometricCameraMode CameraMode = EAether2DIsometricCameraMode::FixedIsometric;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Age of Aether|2D Camera")
    float Yaw = 45.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Age of Aether|2D Camera")
    float Pitch = -55.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Age of Aether|2D Camera")
    float CameraDistance = 600.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Age of Aether|2D Camera")
    float MinCameraDistance = 300.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Age of Aether|2D Camera")
    float MaxCameraDistance = 900.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Age of Aether|2D Camera")
    float ZoomStep = 60.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Age of Aether|2D Camera")
    float PositionLagSpeed = 12.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Age of Aether|2D Camera")
    bool bEnableCameraLag = true;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Age of Aether|2D Camera")
    bool bEnableCollisionTest = true;

    UFUNCTION(BlueprintPure, Category = "Age of Aether|2D Camera")
    bool IsConfigured() const;

    UFUNCTION(BlueprintCallable, Category = "Age of Aether|2D Camera")
    bool ValidateProfile(FString& OutError) const;
};
