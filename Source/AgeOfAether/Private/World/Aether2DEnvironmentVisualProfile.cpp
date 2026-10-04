#include "World/Aether2DEnvironmentVisualProfile.h"

#include "PaperSprite.h"

bool UAether2DEnvironmentVisualProfile::IsConfigured() const
{
    FString Error;
    return ValidateProfile(Error);
}

bool UAether2DEnvironmentVisualProfile::ValidateProfile(FString& OutError) const
{
    if (VisualProfileID.IsNone())
    {
        OutError = TEXT("VisualProfileID is required.");
        return false;
    }

    if (FamilyID.IsNone())
    {
        OutError = TEXT("FamilyID is required.");
        return false;
    }

    if (Sprite.IsNull())
    {
        OutError = TEXT("Sprite is required.");
        return false;
    }

    if (VisualScale.X <= 0.0f || VisualScale.Y <= 0.0f)
    {
        OutError = TEXT("VisualScale must be positive.");
        return false;
    }

    return true;
}
