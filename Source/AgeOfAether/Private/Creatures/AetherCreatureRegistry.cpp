#include "Creatures/AetherCreatureRegistry.h"

FString FAetherCreatureRegistry::Normalize(const FString& Value)
{
    return Value.TrimStartAndEnd().ToLower();
}

bool FAetherCreatureRegistry::RegisterCreature(const FAetherCreatureDefinition& Definition)
{
    if (!Definition.IsValid()) return false;
    const FString Key = Normalize(Definition.CreatureID);
    if (Key.IsEmpty() || Creatures.Contains(Key)) return false;

    FAetherCreatureDefinition Copy = Definition;
    Copy.CreatureID = Key;
    Creatures.Add(Key, MoveTemp(Copy));
    return true;
}

bool FAetherCreatureRegistry::RegisterSpawn(const FAetherCreatureSpawnDefinition& Definition)
{
    if (!Definition.IsValid()) return false;
    const FString Key = Normalize(Definition.SpawnID);
    const FString CreatureKey = Normalize(Definition.CreatureID);
    if (Key.IsEmpty() || Spawns.Contains(Key) || !Creatures.Contains(CreatureKey)) return false;

    FAetherCreatureSpawnDefinition Copy = Definition;
    Copy.SpawnID = Key;
    Copy.CreatureID = CreatureKey;
    Spawns.Add(Key, MoveTemp(Copy));
    return true;
}

const FAetherCreatureDefinition* FAetherCreatureRegistry::FindCreature(const FString& CreatureID) const
{
    return Creatures.Find(Normalize(CreatureID));
}

const FAetherCreatureSpawnDefinition* FAetherCreatureRegistry::FindSpawn(const FString& SpawnID) const
{
    return Spawns.Find(Normalize(SpawnID));
}

bool FAetherCreatureRegistry::Validate(TArray<FString>& OutErrors) const
{
    OutErrors.Reset();

    for (const TPair<FString, FAetherCreatureDefinition>& Pair : Creatures)
    {
        if (!Pair.Value.IsValid())
            OutErrors.Add(FString::Printf(TEXT("Invalid creature: %s"), *Pair.Key));
    }

    for (const TPair<FString, FAetherCreatureSpawnDefinition>& Pair : Spawns)
    {
        if (!Pair.Value.IsValid())
            OutErrors.Add(FString::Printf(TEXT("Invalid spawn: %s"), *Pair.Key));
        if (!Creatures.Contains(Normalize(Pair.Value.CreatureID)))
            OutErrors.Add(FString::Printf(TEXT("Spawn references missing creature: %s"), *Pair.Key));
    }

    return OutErrors.IsEmpty();
}

void FAetherCreatureRegistry::Reset()
{
    Creatures.Reset();
    Spawns.Reset();
}
