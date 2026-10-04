#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "World/AetherVerticalSliceTypes.h"
#include "AetherVerticalSliceCatalog.generated.h"

UCLASS(BlueprintType)
class AGEOFAETHER_API UAetherVerticalSliceCatalog : public UDataAsset
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TArray<FAetherVerticalSliceDefinition> Slices;

    bool IsValid(TArray<FString>& OutErrors) const;
    void RegisterInto(class UAetherVerticalSliceSubsystem* Subsystem) const;
};