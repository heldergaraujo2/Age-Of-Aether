#include "World/AetherVerticalSliceRegistry.h"

bool FAetherVerticalSliceRegistry::RegisterSlice(const FAetherVerticalSliceDefinition& Definition)
{
    if (!Definition.IsValid())
    {
        return false;
    }

    const FString Key = Normalize(Definition.SliceID);
    if (Key.IsEmpty() || Slices.Contains(Key))
    {
        return false;
    }

    Slices.Add(Key, Definition);
    return true;
}

const FAetherVerticalSliceDefinition* FAetherVerticalSliceRegistry::FindSlice(const FString& SliceID) const
{
    return Slices.Find(Normalize(SliceID));
}

bool FAetherVerticalSliceRegistry::Validate(TArray<FString>& OutErrors) const
{
    OutErrors.Reset();

    for (const TPair<FString, FAetherVerticalSliceDefinition>& Pair : Slices)
    {
        Pair.Value.IsValid(&OutErrors);
    }

    return OutErrors.Num() == 0;
}

void FAetherVerticalSliceRegistry::Reset()
{
    Slices.Reset();
}