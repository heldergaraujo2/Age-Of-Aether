#pragma once

#include "CoreMinimal.h"
#include "Creatures/AetherCreatureTypes.h"

class FAetherCreatureRegistry
{
public:
    bool RegisterCreature(const FAetherCreatureDefinition& Definition);
    bool RegisterSpawn(const FAetherCreatureSpawnDefinition& Definition);
    const FAetherCreatureDefinition* FindCreature(const FString& CreatureID) const;
    const FAetherCreatureSpawnDefinition* FindSpawn(const FString& SpawnID) const;
    bool Validate(TArray<FString>& OutErrors) const;
    void Reset();

    int32 NumCreatures() const { return Creatures.Num(); }
    int32 NumSpawns() const { return Spawns.Num(); }

private:
    static FString Normalize(const FString& Value);
    TMap<FString, FAetherCreatureDefinition> Creatures;
    TMap<FString, FAetherCreatureSpawnDefinition> Spawns;
};
