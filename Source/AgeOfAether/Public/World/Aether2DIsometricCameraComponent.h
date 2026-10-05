#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "World/Aether2DIsometricCameraProfile.h"
#include "Aether2DIsometricCameraComponent.generated.h"

class USpringArmComponent;

UCLASS(ClassGroup = (AgeOfAether), BlueprintType, Blueprintable, meta = (BlueprintSpawnableComponent))
class AGEOFAETHER_API UAether2DIsometricCameraComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UAether2DIsometricCameraComponent();

    virtual void BeginPlay() override;
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    UFUNCTION(BlueprintCallable, Category = "Age of Aether|2D Camera")
    bool ApplyProfile();

    UFUNCTION(BlueprintCallable, Category = "Age of Aether|2D Camera")
    bool ApplyProfileAsset(UAether2DIsometricCameraProfile* InProfile);

    UFUNCTION(BlueprintCallable, Category = "Age of Aether|2D Camera")
    void AddZoomInput(float AxisValue);

    UFUNCTION(BlueprintCallable, Category = "Age of Aether|2D Camera")
    void ToggleFreeOrbitMode();

    UFUNCTION(BlueprintCallable, Category = "Age of Aether|2D Camera")
    void ResetCameraView();

    UFUNCTION(BlueprintCallable, Category = "Age of Aether|2D Camera")
    void AddOrbitInput(float YawDelta, float PitchDelta);

    UFUNCTION(BlueprintPure, Category = "Age of Aether|2D Camera")
    bool IsFreeOrbitModeEnabled() const { return bFreeOrbitModeEnabled; }

    UFUNCTION(BlueprintPure, Category = "Age of Aether|2D Camera")
    bool HasUserOrbitView() const { return bFreeOrbitModeEnabled || bHoldLastOrbitView; }

    UFUNCTION(BlueprintPure, Category = "Age of Aether|2D Camera")
    bool AllowsFreeLook() const;

    UFUNCTION(BlueprintPure, Category = "Age of Aether|2D Camera")
    UAether2DIsometricCameraProfile* GetProfile() const { return Profile; }

protected:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Age of Aether|2D Camera")
    TObjectPtr<UAether2DIsometricCameraProfile> Profile;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Age of Aether|2D Camera")
    bool bApplyOnBeginPlay = true;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Age of Aether|2D Camera")
    bool bUseRuntimeFallbackProfile = true;

private:
    bool ApplyLoadedProfile(UAether2DIsometricCameraProfile* InProfile);
    bool ApplyRuntimeFallback();
    USpringArmComponent* ResolveCameraBoom() const;
    void ApplyCameraPolicy();
    void CaptureInitialCameraView(USpringArmComponent* Boom, bool bUsesAbsoluteRotation);
    void ApplyUserOrbitView(USpringArmComponent* Boom);

    float CurrentDistance = 0.0f;
    float InitialCameraDistance = 2100.0f;
    float OrbitDistance = 2100.0f;
    float OrbitYaw = 45.0f;
    float OrbitPitch = -55.0f;
    float OrbitRotationSensitivity = 0.8f;
    float CloseInspectionMinimumDistance = 250.0f;
    float OrbitMinimumDistance = 120.0f;
    float OrbitMaximumDistance = 3600.0f;
    float InitialCameraLagSpeed = 12.0f;
    FRotator InitialCameraRotation = FRotator(-55.0f, 45.0f, 0.0f);
    bool bInitialUsesAbsoluteRotation = true;
    bool bInitialUsesPawnControlRotation = false;
    bool bInitialCollisionTest = false;
    bool bInitialCameraLag = true;
    bool bHasInitialCameraView = false;
    bool bHasSavedOrbitView = false;
    bool bFreeOrbitModeEnabled = false;
    bool bHoldLastOrbitView = false;
    bool bRuntimeFallbackActive = false;
};
