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
}

void UAether2DIsometricCameraComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if ((Profile || bRuntimeFallbackActive) && GetNetMode() != NM_DedicatedServer)
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

    const float MinDistance = Profile ? Profile->MinCameraDistance : 320.0f;
    const float MaxDistance = Profile ? Profile->MaxCameraDistance : 900.0f;
    const float ZoomStep = Profile ? Profile->ZoomStep : 60.0f;

    CurrentDistance = FMath::Clamp(CurrentDistance - AxisValue * ZoomStep, MinDistance, MaxDistance);

    if (USpringArmComponent* Boom = ResolveCameraBoom())
    {
        Boom->TargetArmLength = CurrentDistance;
    }
}

bool UAether2DIsometricCameraComponent::ApplyRuntimeFallback()
{
    if (GetNetMode() == NM_DedicatedServer)
    {
        return false;
    }

    CurrentDistance = 650.0f;
    bRuntimeFallbackActive = true;

    if (USpringArmComponent* Boom = ResolveCameraBoom())
    {
        Boom->TargetArmLength = CurrentDistance;
        Boom->bDoCollisionTest = true;
        Boom->bUsePawnControlRotation = false;
        Boom->bEnableCameraLag = true;
        Boom->CameraLagSpeed = 12.0f;
        Boom->SetRelativeRotation(FRotator(-55.0f, 45.0f, 0.0f));
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
        Boom->bEnableCameraLag = InProfile->bEnableCameraLag;
        Boom->CameraLagSpeed = InProfile->PositionLagSpeed;
        Boom->SetRelativeRotation(FRotator(InProfile->Pitch, InProfile->Yaw, 0.0f));
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
    if (Profile && Profile->CameraMode == EAether2DIsometricCameraMode::FixedIsometric)
    {
        if (USpringArmComponent* Boom = ResolveCameraBoom())
        {
            const FRotator Desired(Profile->Pitch, Profile->Yaw, 0.0f);
            Boom->SetRelativeRotation(Desired);
            Boom->TargetArmLength = FMath::Clamp(
                Boom->TargetArmLength,
                Profile->MinCameraDistance,
                Profile->MaxCameraDistance);
        }
        return;
    }

    if (bRuntimeFallbackActive)
    {
        if (USpringArmComponent* Boom = ResolveCameraBoom())
        {
            Boom->SetRelativeRotation(FRotator(-55.0f, 45.0f, 0.0f));
            Boom->TargetArmLength = FMath::Clamp(Boom->TargetArmLength, 320.0f, 900.0f);
        }
    }
}

USpringArmComponent* UAether2DIsometricCameraComponent::ResolveCameraBoom() const
{
    if (AActor* Owner = GetOwner())
    {
        return Owner->FindComponentByClass<USpringArmComponent>();
    }

    return nullptr;
}
