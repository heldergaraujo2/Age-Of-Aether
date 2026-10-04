#pragma once

#include "CoreMinimal.h"
#include "World/AetherVerticalSliceTypes.h"

class FAetherVerticalSliceRegistry
{
public:
    bool RegisterSlice(const FAetherVerticalSliceDefinition& Definition);
    const FAetherVerticalSliceDefinition* FindSlice(const FString& SliceID) const;
    bool Validate(TArray<FString>& OutErrors) const;
    void Reset();
    int32 NumSlices() const { return Slices.Num(); }

private:
    static FString Normalize(const FString& Value)
    {
        return Value.TrimStartAndEnd().ToLower();
    }

    TMap<FString, FAetherVerticalSliceDefinition> Slices;
};