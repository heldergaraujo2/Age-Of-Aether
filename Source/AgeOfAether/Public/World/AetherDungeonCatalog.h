#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "World/AetherDungeonTypes.h"
#include "AetherDungeonCatalog.generated.h"

UCLASS(BlueprintType)
class AGEOFAETHER_API UAetherDungeonCatalog : public UDataAsset
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TArray<FAetherDungeonDefinition> Dungeons;

    bool IsValid(TArray<FString>& OutErrors) const;
    void RegisterInto(class UAetherDungeonSubsystem* Subsystem) const;
};