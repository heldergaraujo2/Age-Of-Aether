#include "World/Aether2DIsometricCameraComponent.h"

#include "GameFramework/SpringArmComponent.h"

UAether2DIsometricCameraComponent::UAether2DIsometricCameraComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
    PrimaryComponentTick.TickGroup = TG_PostUpdateWork;
}

void UAether2DIsometricCameraComponent::BeginPlay()
{
    Super::BeginPlay();

    if (Profile && bApplyOnBeginPlay)
    {
        ApplyProfile();
    }
    else if (!Profile && bUseRuntimeFallbackProfile)
    {
        ApplyRuntimeFallback();
    }

    if (!bHasInitialCameraView)
    {
        CaptureInitialCameraView(ResolveCameraBoom(), false);
    }
}

void UAether2DIsometricCameraComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if ((Profile || bRuntimeFallbackActive || bFreeOrbitModeEnabled || bHoldLastOrbitView)
        && GetNetMode() != NM_DedicatedServer)
    {
        ApplyCameraPolicy();
    }
}

bool UAether2DIsometricCameraComponent::ApplyProfile()
{
    if (!Profile || GetNetMode() == NM_DedicatedServer)
    {
        return false;
    }

    bRuntimeFallbackActive = false;
    return ApplyLoadedProfile(Profile);
}

bool UAether2DIsometricCameraComponent::ApplyProfileAsset(UAether2DIsometricCameraProfile* InProfile)
{
    if (!InProfile)
    {
        return false;
    }

    Profile = InProfile;
    return ApplyProfile();
}

void UAether2DIsometricCameraComponent::AddZoomInput(float AxisValue)
{
    if (!FMath::IsFinite(AxisValue) || FMath::IsNearlyZero(AxisValue))
    {
        return;
    }

    const bool bUsingOrbitRange = bFreeOrbitModeEnabled || bHoldLastOrbitView;
    const float MinDistance = bUsingOrbitRange
        ? OrbitMinimumDistance
        : FMath::Min(
            Profile ? Profile->MinCameraDistance : CloseInspectionMinimumDistance,
            CloseInspectionMinimumDistance);
    const float MaxDistance = bUsingOrbitRange
        ? OrbitMaximumDistance
        : (Profile ? Profile->MaxCameraDistance : 3600.0f);
    const float ZoomStep = Profile ? Profile->ZoomStep : 180.0f;

    CurrentDistance = FMath::Clamp(CurrentDistance - AxisValue * ZoomStep, MinDistance, MaxDistance);
    if (bUsingOrbitRange)
    {
        OrbitDistance = CurrentDistance;
    }

    if (USpringArmComponent* Boom = ResolveCameraBoom())
    {
        Boom->TargetArmLength = CurrentDistance;
    }
}

void UAether2DIsometricCameraComponent::ToggleFreeOrbitMode()
{
    if (GetNetMode() == NM_DedicatedServer)
    {
        return;
    }

    USpringArmComponent* Boom = ResolveCameraBoom();
    if (!Boom)
    {
        return;
    }
    if (!bHasInitialCameraView)
    {
        CaptureInitialCameraView(Boom, false);
    }
    if (!bHasSavedOrbitView)
    {
        const FRotator CurrentRotation = Boom->GetRelativeRotation();
        OrbitYaw = FRotator::NormalizeAxis(CurrentRotation.Yaw);
        OrbitPitch = FMath::Clamp(CurrentRotation.Pitch, -85.0f, -5.0f);
        OrbitDistance = FMath::Clamp(Boom->TargetArmLength, OrbitMinimumDistance, OrbitMaximumDistance);
        bHasSavedOrbitView = true;
    }

    bFreeOrbitModeEnabled = !bFreeOrbitModeEnabled;
    if (bFreeOrbitModeEnabled)
    {
        bHoldLastOrbitView = false;
        ApplyUserOrbitView(Boom);
        UE_LOG(LogTemp, Log, TEXT("Free 3D camera enabled. Hold middle mouse and drag to orbit; F6 resets the view."));
    }
    else
    {
        // Keep the final orbit and zoom pose. The next F5 resumes from here.
        bHoldLastOrbitView = true;
        UE_LOG(LogTemp, Log, TEXT("Free camera controls disabled; keeping the current camera view."));
    }
}

void UAether2DIsometricCameraComponent::ResetCameraView()
{
    USpringArmComponent* Boom = ResolveCameraBoom();
    if (!Boom)
    {
        return;
    }
    if (!bHasInitialCameraView)
    {
        CaptureInitialCameraView(Boom, false);
    }

    OrbitYaw = FRotator::NormalizeAxis(InitialCameraRotation.Yaw);
    OrbitPitch = InitialCameraRotation.Pitch;
    OrbitDistance = InitialCameraDistance;
    CurrentDistance = InitialCameraDistance;
    bHasSavedOrbitView = true;
    bHoldLastOrbitView = false;

    if (bFreeOrbitModeEnabled)
    {
        ApplyUserOrbitView(Boom);
    }
    else
    {
        Boom->bUsePawnControlRotation = bInitialUsesPawnControlRotation;
        Boom->SetUsingAbsoluteRotation(bInitialUsesAbsoluteRotation);
        Boom->bDoCollisionTest = bInitialCollisionTest;
        Boom->bEnableCameraLag = bInitialCameraLag;
        Boom->CameraLagSpeed = InitialCameraLagSpeed;
        Boom->SetRelativeRotation(InitialCameraRotation);
        Boom->TargetArmLength = InitialCameraDistance;
    }

    UE_LOG(LogTemp, Log, TEXT("Camera view reset to its original pitch, yaw and distance."));
}

void UAether2DIsometricCameraComponent::AddOrbitInput(float YawDelta, float PitchDelta)
{
    if (!bFreeOrbitModeEnabled || !FMath::IsFinite(YawDelta) || !FMath::IsFinite(PitchDelta))
    {
        return;
    }

    OrbitYaw = FRotator::NormalizeAxis(OrbitYaw + YawDelta * OrbitRotationSensitivity);
    OrbitPitch = FMath::Clamp(
        OrbitPitch - PitchDelta * OrbitRotationSensitivity,
        -85.0f,
        -5.0f);

    if (USpringArmComponent* Boom = ResolveCameraBoom())
    {
        ApplyUserOrbitView(Boom);
    }
}

bool UAether2DIsometricCameraComponent::ApplyRuntimeFallback()
{
    if (GetNetMode() == NM_DedicatedServer)
    {
        return false;
    }

    CurrentDistance = 2100.0f;
    bRuntimeFallbackActive = true;

    if (USpringArmComponent* Boom = ResolveCameraBoom())
    {
        Boom->TargetArmLength = CurrentDistance;
        Boom->bDoCollisionTest = false;
        Boom->bUsePawnControlRotation = false;
        Boom->SetUsingAbsoluteRotation(true);
        Boom->bEnableCameraLag = true;
        Boom->CameraLagSpeed = 12.0f;
        Boom->SetRelativeRotation(FRotator(-55.0f, 45.0f, 0.0f));
        CaptureInitialCameraView(Boom, true);
    }

    return true;
}

bool UAether2DIsometricCameraComponent::ApplyLoadedProfile(UAether2DIsometricCameraProfile* InProfile)
{
    FString ValidationError;
    if (!InProfile->ValidateProfile(ValidationError))
    {
        UE_LOG(LogTemp, Warning, TEXT("Aether 2D isometric camera profile rejected: %s"), *ValidationError);
        return false;
    }

    CurrentDistance = InProfile->CameraDistance;

    if (USpringArmComponent* Boom = ResolveCameraBoom())
    {
        Boom->TargetArmLength = CurrentDistance;
        Boom->bDoCollisionTest = InProfile->bEnableCollisionTest;
        Boom->bUsePawnControlRotation = InProfile->CameraMode == EAether2DIsometricCameraMode::Orbit;
        Boom->SetUsingAbsoluteRotation(InProfile->CameraMode == EAether2DIsometricCameraMode::FixedIsometric);
        Boom->bEnableCameraLag = InProfile->bEnableCameraLag;
        Boom->CameraLagSpeed = InProfile->PositionLagSpeed;
        Boom->SetRelativeRotation(FRotator(InProfile->Pitch, InProfile->Yaw, 0.0f));
        CaptureInitialCameraView(
            Boom,
            InProfile->CameraMode == EAether2DIsometricCameraMode::FixedIsometric);
    }

    return true;
}

bool UAether2DIsometricCameraComponent::AllowsFreeLook() const
{
    if (bRuntimeFallbackActive)
    {
        return false;
    }

    return !Profile || Profile->CameraMode == EAether2DIsometricCameraMode::Orbit;
}

void UAether2DIsometricCameraComponent::ApplyCameraPolicy()
{
    if (bFreeOrbitModeEnabled || bHoldLastOrbitView)
    {
        if (USpringArmComponent* Boom = ResolveCameraBoom())
        {
            ApplyUserOrbitView(Boom);
        }
        return;
    }

    if (Profile && Profile->CameraMode == EAether2DIsometricCameraMode::FixedIsometric)
    {
        if (USpringArmComponent* Boom = ResolveCameraBoom())
        {
            const FRotator Desired(Profile->Pitch, Profile->Yaw, 0.0f);
            Boom->SetRelativeRotation(Desired);
            Boom->TargetArmLength = FMath::Clamp(
                Boom->TargetArmLength,
                FMath::Min(Profile->MinCameraDistance, CloseInspectionMinimumDistance),
                Profile->MaxCameraDistance);
        }
        return;
    }

    if (bRuntimeFallbackActive)
    {
        if (USpringArmComponent* Boom = ResolveCameraBoom())
        {
            Boom->SetRelativeRotation(FRotator(-55.0f, 45.0f, 0.0f));
            Boom->TargetArmLength = FMath::Clamp(
                Boom->TargetArmLength,
                CloseInspectionMinimumDistance,
                3600.0f);
        }
    }
}

void UAether2DIsometricCameraComponent::CaptureInitialCameraView(
    USpringArmComponent* Boom,
    bool bUsesAbsoluteRotation)
{
    if (!Boom)
    {
        return;
    }

    InitialCameraRotation = Boom->GetRelativeRotation();
    InitialCameraDistance = FMath::Max(Boom->TargetArmLength, 1.0f);
    InitialCameraLagSpeed = Boom->CameraLagSpeed;
    bInitialUsesAbsoluteRotation = bUsesAbsoluteRotation;
    bInitialUsesPawnControlRotation = Boom->bUsePawnControlRotation;
    bInitialCollisionTest = Boom->bDoCollisionTest;
    bInitialCameraLag = Boom->bEnableCameraLag;
    bHasInitialCameraView = true;

    CurrentDistance = InitialCameraDistance;
    OrbitDistance = InitialCameraDistance;
    OrbitYaw = FRotator::NormalizeAxis(InitialCameraRotation.Yaw);
    OrbitPitch = InitialCameraRotation.Pitch;
    bHasSavedOrbitView = false;
    bFreeOrbitModeEnabled = false;
    bHoldLastOrbitView = false;
}

void UAether2DIsometricCameraComponent::ApplyUserOrbitView(USpringArmComponent* Boom)
{
    if (!Boom)
    {
        return;
    }

    Boom->bUsePawnControlRotation = false;
    Boom->SetUsingAbsoluteRotation(true);
    Boom->bDoCollisionTest = true;
    Boom->bEnableCameraLag = false;
    Boom->SetRelativeRotation(FRotator(OrbitPitch, OrbitYaw, 0.0f));
    Boom->TargetArmLength = FMath::Clamp(OrbitDistance, OrbitMinimumDistance, OrbitMaximumDistance);
    CurrentDistance = Boom->TargetArmLength;
    OrbitDistance = CurrentDistance;
}

USpringArmComponent* UAether2DIsometricCameraComponent::ResolveCameraBoom() const
{
    if (AActor* Owner = GetOwner())
    {
        return Owner->FindComponentByClass<USpringArmComponent>();
    }

    return nullptr;
}
