#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "World/AetherDungeonRegistry.h"
#include "AetherDungeonSubsystem.generated.h"

UCLASS()
class AGEOFAETHER_API UAetherDungeonSubsystem : public UWorldSubsystem
{
    GENERATED_BODY()

public:
    bool RegisterDungeon(const FAetherDungeonDefinition& Definition)
    {
        return Registry.RegisterDungeon(Definition);
    }

    UFUNCTION(BlueprintPure, Category="Age of Aether|Dungeon")
    bool FindDungeon(const FString& DungeonID, FAetherDungeonDefinition& OutDefinition) const;

    UFUNCTION(BlueprintCallable, Category="Age of Aether|Dungeon")
    bool ValidateDungeons(TArray<FString>& OutErrors) const;

    void ResetDungeons()
    {
        Registry.Reset();
    }

private:
    FAetherDungeonRegistry Registry;
};