#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "World/AetherVerticalSliceRegistry.h"
#include "AetherVerticalSliceSubsystem.generated.h"

UCLASS()
class AGEOFAETHER_API UAetherVerticalSliceSubsystem : public UWorldSubsystem
{
    GENERATED_BODY()

public:
    bool RegisterSlice(const FAetherVerticalSliceDefinition& Definition)
    {
        return Registry.RegisterSlice(Definition);
    }

    UFUNCTION(BlueprintPure, Category="Age of Aether|Vertical Slice")
    bool FindSlice(const FString& SliceID, FAetherVerticalSliceDefinition& OutDefinition) const;

    UFUNCTION(BlueprintCallable, Category="Age of Aether|Vertical Slice")
    bool ValidateSlices(TArray<FString>& OutErrors) const;

    void ResetSlices()
    {
        Registry.Reset();
    }

private:
    FAetherVerticalSliceRegistry Registry;
};