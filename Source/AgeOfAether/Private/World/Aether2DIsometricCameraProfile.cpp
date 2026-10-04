#include "World/Aether2DIsometricCameraProfile.h"

bool UAether2DIsometricCameraProfile::IsConfigured() const
{
    FString Error;
    return ValidateProfile(Error);
}

bool UAether2DIsometricCameraProfile::ValidateProfile(FString& OutError) const
{
    if (ProfileID.IsNone())
    {
        OutError = TEXT("ProfileID is required.");
        return false;
    }

    if (Pitch >= 0.0f || Pitch < -89.0f)
    {
        OutError = TEXT("Pitch must be between -89 and 0 degrees.");
        return false;
    }

    if (CameraDistance <= 0.0f || MinCameraDistance <= 0.0f || MaxCameraDistance < MinCameraDistance)
    {
        OutError = TEXT("Camera distance bounds are invalid.");
        return false;
    }

    if (CameraDistance < MinCameraDistance || CameraDistance > MaxCameraDistance)
    {
        OutError = TEXT("CameraDistance must be within the configured bounds.");
        return false;
    }

    if (ZoomStep <= 0.0f)
    {
        OutError = TEXT("ZoomStep must be positive.");
        return false;
    }

    return true;
}
