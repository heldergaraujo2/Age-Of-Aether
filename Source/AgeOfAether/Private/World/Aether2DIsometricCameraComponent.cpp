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

    if (bApplyOnBeginPlay && Profile)
    {
        ApplyProfile();
    }
}

void UAether2DIsometricCameraComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if (Profile && GetNetMode() != NM_DedicatedServer)
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
    if (!Profile || !FMath::IsFinite(AxisValue) || FMath::IsNearlyZero(AxisValue))
    {
        return;
    }

    CurrentDistance = FMath::Clamp(
        CurrentDistance - AxisValue * Profile->ZoomStep,
        Profile->MinCameraDistance,
        Profile->MaxCameraDistance);

    if (USpringArmComponent* Boom = ResolveCameraBoom())
    {
        Boom->TargetArmLength = CurrentDistance;
    }
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
        Boom->bEnableCameraLag = InProfile->bEnableCameraLag;
        Boom->CameraLagSpeed = InProfile->PositionLagSpeed;
        Boom->SetRelativeRotation(FRotator(InProfile->Pitch, InProfile->Yaw, 0.0f));
    }

    return true;
}

void UAether2DIsometricCameraComponent::ApplyCameraPolicy()
{
    if (!Profile || Profile->CameraMode != EAether2DIsometricCameraMode::FixedIsometric)
    {
        return;
    }

    if (USpringArmComponent* Boom = ResolveCameraBoom())
    {
        const FRotator Desired(Profile->Pitch, Profile->Yaw, 0.0f);
        Boom->SetRelativeRotation(Desired);
        Boom->TargetArmLength = FMath::Clamp(
            Boom->TargetArmLength,
            Profile->MinCameraDistance,
            Profile->MaxCameraDistance);
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
