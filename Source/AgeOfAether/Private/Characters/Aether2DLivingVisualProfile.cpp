#include "Characters/Aether2DLivingVisualProfile.h"

#include "PaperFlipbook.h"

bool UAether2DLivingVisualProfile::HasState(EAether2DCharacterVisualState State) const
{
    return Flipbooks.Contains(State) && !Flipbooks.FindChecked(State).IsNull();
}

bool UAether2DLivingVisualProfile::IsConfigured() const
{
    FString Error;
    return ValidateProfile(Error);
}

bool UAether2DLivingVisualProfile::ValidateProfile(FString& OutError) const
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

    if (!HasState(EAether2DCharacterVisualState::Idle))
    {
        OutError = TEXT("Idle Flipbook is required.");
        return false;
    }

    if (VisualScale.X <= 0.0f || VisualScale.Y <= 0.0f)
    {
        OutError = TEXT("VisualScale must be positive.");
        return false;
    }

    for (const TPair<EAether2DCharacterVisualState, TSoftObjectPtr<UPaperFlipbook>>& Entry : Flipbooks)
    {
        if (Entry.Value.IsNull())
        {
            OutError = TEXT("Flipbook map contains a null asset reference.");
            return false;
        }
    }

    return true;
}
