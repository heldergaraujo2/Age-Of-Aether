#include "Characters/AetherPlayableCharacterVisualProfile.h"

#include "Animation/AnimInstance.h"
#include "Engine/SkeletalMesh.h"
#include "Materials/MaterialInterface.h"

bool UAetherPlayableCharacterVisualProfile::IsConfigured() const
{
    return VisualProfileID != NAME_None || !SkeletalMesh.IsNull() || !AnimationClass.IsNull();
}

bool UAetherPlayableCharacterVisualProfile::ValidateProfile(FString& OutError) const
{
    OutError.Reset();

    if (VisualProfileID.IsNone())
    {
        OutError = TEXT("VisualProfileID is required.");
        return false;
    }

    if (SkeletalMesh.IsNull())
    {
        OutError = TEXT("SkeletalMesh is required for a playable character visual profile.");
        return false;
    }

    for (const TSoftObjectPtr<UMaterialInterface>& Material : MaterialOverrides)
    {
        if (Material.IsNull())
        {
            OutError = TEXT("MaterialOverrides cannot contain null references.");
            return false;
        }
    }

    return true;
}
