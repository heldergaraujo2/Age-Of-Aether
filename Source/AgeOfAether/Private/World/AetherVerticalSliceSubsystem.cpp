#include "World/AetherVerticalSliceSubsystem.h"

bool UAetherVerticalSliceSubsystem::FindSlice(
    const FString& SliceID,
    FAetherVerticalSliceDefinition& OutDefinition) const
{
    const FAetherVerticalSliceDefinition* Found = Registry.FindSlice(SliceID);
    if (!Found)
    {
        return false;
    }

    OutDefinition = *Found;
    return true;
}

bool UAetherVerticalSliceSubsystem::ValidateSlices(TArray<FString>& OutErrors) const
{
    return Registry.Validate(OutErrors);
}