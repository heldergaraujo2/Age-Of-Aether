#include "Characters/AetherCharacterAnimationProfile.h"

bool UAetherCharacterAnimationProfile::IsConfigured() const
{
    return !AnimationProfileID.IsNone();
}

bool UAetherCharacterAnimationProfile::ValidateProfile(FString& OutError) const
{
    OutError.Reset();

    if (AnimationProfileID.IsNone())
    {
        OutError = TEXT("AnimationProfileID is required.");
        return false;
    }

    if (!FMath::IsFinite(WalkSpeedThreshold) || WalkSpeedThreshold < 0.0f)
    {
        OutError = TEXT("WalkSpeedThreshold must be finite and non-negative.");
        return false;
    }

    if (!FMath::IsFinite(RunSpeedThreshold) || RunSpeedThreshold < WalkSpeedThreshold)
    {
        OutError = TEXT("RunSpeedThreshold must be finite and >= WalkSpeedThreshold.");
        return false;
    }

    return true;
}