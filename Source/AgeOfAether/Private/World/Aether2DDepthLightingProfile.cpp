#include "World/Aether2DDepthLightingProfile.h"

bool UAether2DDepthLightingProfile::IsConfigured() const
{
    FString Error;
    return ValidateProfile(Error);
}

bool UAether2DDepthLightingProfile::ValidateProfile(FString& OutError) const
{
    if (ProfileID.IsNone())
    {
        OutError = TEXT("ProfileID is required.");
        return false;
    }

    if (ParallaxFactor < 0.0f || ParallaxFactor > 1.0f)
    {
        OutError = TEXT("ParallaxFactor must be between 0 and 1.");
        return false;
    }

    if (RenderLayer < -32768 || RenderLayer > 32767)
    {
        OutError = TEXT("RenderLayer is outside the supported translucency sort range.");
        return false;
    }

    return true;
}
