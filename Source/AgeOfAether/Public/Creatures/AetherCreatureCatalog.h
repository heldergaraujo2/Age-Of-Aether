#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Creatures/AetherCreatureTypes.h"
#include "AetherCreatureCatalog.generated.h"

UCLASS(BlueprintType)
class AGEOFAETHER_API UAetherCreatureCatalog : public UDataAsset
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Creatures")
    TArray<FAetherCreatureDefinition> Creatures;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Creatures")
    TArray<FAetherCreatureSpawnDefinition> Spawns;

    bool IsValid(TArray<FString>& OutErrors) const;
    void RegisterInto(class UAetherCreatureSubsystem* Subsystem) const;
};
