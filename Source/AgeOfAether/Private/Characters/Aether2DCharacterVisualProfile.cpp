#include "Characters/Aether2DCharacterVisualProfile.h"

#include "PaperFlipbook.h"

bool UAether2DCharacterVisualProfile::HasState(EAether2DCharacterVisualState State) const
{
    const TSoftObjectPtr<UPaperFlipbook>* Found = Flipbooks.Find(State);
    return Found && !Found->IsNull();
}

bool UAether2DCharacterVisualProfile::IsConfigured() const
{
    return !VisualProfileID.IsNone() && HasState(EAether2DCharacterVisualState::Idle);
}

bool UAether2DCharacterVisualProfile::ValidateProfile(FString& OutError) const
{
    OutError.Reset();

    if (VisualProfileID.IsNone())
    {
        OutError = TEXT("VisualProfileID is required.");
        return false;
    }

    if (!HasState(EAether2DCharacterVisualState::Idle))
    {
        OutError = TEXT("Idle Flipbook is required.");
        return false;
    }

    for (const TPair<EAether2DCharacterVisualState, TSoftObjectPtr<UPaperFlipbook>>& Entry : Flipbooks)
    {
        if (Entry.Value.IsNull())
        {
            OutError = TEXT("Flipbooks cannot contain null references.");
            return false;
        }
    }

    if (VisualScale.X <= 0.0f || VisualScale.Y <= 0.0f)
    {
        OutError = TEXT("VisualScale must be positive.");
        return false;
    }

    return true;
}
