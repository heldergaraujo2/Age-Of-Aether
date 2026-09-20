#include "Creatures/AetherCreatureCatalog.h"
#include "Creatures/AetherCreatureSubsystem.h"

bool UAetherCreatureCatalog::IsValid(TArray<FString>& OutErrors) const
{
    OutErrors.Reset();

    TSet<FString> CreatureIds;
    for (const FAetherCreatureDefinition& Creature : Creatures)
    {
        if (!Creature.IsValid())
        {
            OutErrors.Add(FString::Printf(TEXT("Invalid creature definition: %s"), *Creature.CreatureID));
            continue;
        }

        const FString Key = Creature.CreatureID.TrimStartAndEnd().ToLower();
        if (CreatureIds.Contains(Key))
            OutErrors.Add(FString::Printf(TEXT("Duplicate creature ID: %s"), *Key));
        CreatureIds.Add(Key);
    }

    TSet<FString> SpawnIds;
    for (const FAetherCreatureSpawnDefinition& Spawn : Spawns)
    {
        if (!Spawn.IsValid())
        {
            OutErrors.Add(FString::Printf(TEXT("Invalid spawn definition: %s"), *Spawn.SpawnID));
            continue;
        }

        const FString SpawnKey = Spawn.SpawnID.TrimStartAndEnd().ToLower();
        if (SpawnIds.Contains(SpawnKey))
            OutErrors.Add(FString::Printf(TEXT("Duplicate spawn ID: %s"), *SpawnKey));
        SpawnIds.Add(SpawnKey);

        if (!CreatureIds.Contains(Spawn.CreatureID.TrimStartAndEnd().ToLower()))
            OutErrors.Add(FString::Printf(TEXT("Spawn references missing creature: %s"), *Spawn.CreatureID));
    }

    return OutErrors.IsEmpty();
}

void UAetherCreatureCatalog::RegisterInto(UAetherCreatureSubsystem* Subsystem) const
{
    if (!Subsystem) return;
    for (const FAetherCreatureDefinition& Creature : Creatures)
        Subsystem->RegisterCreature(Creature);
    for (const FAetherCreatureSpawnDefinition& Spawn : Spawns)
        Subsystem->RegisterSpawn(Spawn);
}
