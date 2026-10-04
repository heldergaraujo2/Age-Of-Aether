#include "World/AetherVerticalSliceCatalog.h"

#include "World/AetherVerticalSliceSubsystem.h"

bool UAetherVerticalSliceCatalog::IsValid(TArray<FString>& OutErrors) const
{
    OutErrors.Reset();

    TSet<FString> SliceIDs;
    for (const FAetherVerticalSliceDefinition& Slice : Slices)
    {
        if (!Slice.IsValid(&OutErrors))
        {
            continue;
        }

        const FString Key = Slice.SliceID.TrimStartAndEnd().ToLower();
        if (SliceIDs.Contains(Key))
        {
            OutErrors.Add(FString::Printf(TEXT("Duplicate vertical slice ID: %s"), *Slice.SliceID));
        }
        SliceIDs.Add(Key);
    }

    return OutErrors.Num() == 0;
}

void UAetherVerticalSliceCatalog::RegisterInto(UAetherVerticalSliceSubsystem* Subsystem) const
{
    if (!Subsystem)
    {
        return;
    }

    for (const FAetherVerticalSliceDefinition& Slice : Slices)
    {
        Subsystem->RegisterSlice(Slice);
    }
}