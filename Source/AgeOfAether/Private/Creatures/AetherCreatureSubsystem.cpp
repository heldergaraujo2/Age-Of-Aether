#include "Creatures/AetherCreatureSubsystem.h"
#include "Creatures/AetherCreatureActor.h"
#include "Engine/World.h"

bool UAetherCreatureSubsystem::RegisterCreature(const FAetherCreatureDefinition& Definition)
{
    return Registry.RegisterCreature(Definition);
}

bool UAetherCreatureSubsystem::RegisterSpawn(const FAetherCreatureSpawnDefinition& Definition)
{
    return Registry.RegisterSpawn(Definition);
}

AAetherCreatureActor* UAetherCreatureSubsystem::SpawnCreature(const FString& SpawnID)
{
    if (!GetWorld()) return nullptr;

    const FAetherCreatureSpawnDefinition* Spawn = Registry.FindSpawn(SpawnID);
    if (!Spawn || !Spawn->bEnabled) return nullptr;

    const FAetherCreatureDefinition* Creature = Registry.FindCreature(Spawn->CreatureID);
    if (!Creature) return nullptr;

    int32 AliveCount = 0;
    for (AAetherCreatureActor* Actor : SpawnedCreatures)
    {
        if (IsValid(Actor) && Actor->GetCreatureID().Equals(Creature->CreatureID, ESearchCase::IgnoreCase) &&
            Actor->IsAlive())
            ++AliveCount;
    }

    if (AliveCount >= Spawn->MaxAlive) return nullptr;

    FActorSpawnParameters Params;
    Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

    AAetherCreatureActor* Actor = GetWorld()->SpawnActor<AAetherCreatureActor>(
        AAetherCreatureActor::StaticClass(), Spawn->Location, Spawn->Rotation, Params);

    if (!Actor || !Actor->ApplyDefinition(*Creature))
    {
        if (Actor) Actor->Destroy();
        return nullptr;
    }

    SpawnedCreatures.Add(Actor);
    return Actor;
}

int32 UAetherCreatureSubsystem::GetAliveCountForSpawn(const FString& SpawnID) const
{
    const FAetherCreatureSpawnDefinition* Spawn = Registry.FindSpawn(SpawnID);
    if (!Spawn) return 0;

    int32 Count = 0;
    for (AAetherCreatureActor* Actor : SpawnedCreatures)
    {
        if (IsValid(Actor) && Actor->IsAlive() &&
            Actor->GetCreatureID().Equals(Spawn->CreatureID, ESearchCase::IgnoreCase))
            ++Count;
    }
    return Count;
}

void UAetherCreatureSubsystem::DespawnAllCreatures()
{
    for (AAetherCreatureActor* Actor : SpawnedCreatures)
        if (IsValid(Actor)) Actor->Destroy();
    SpawnedCreatures.Reset();
}

const FAetherCreatureDefinition* UAetherCreatureSubsystem::FindCreature(const FString& CreatureID) const
{
    return Registry.FindCreature(CreatureID);
}

bool UAetherCreatureSubsystem::ValidateRegistry(TArray<FString>& OutErrors) const
{
    return Registry.Validate(OutErrors);
}

void UAetherCreatureSubsystem::ResetRegistry()
{
    DespawnAllCreatures();
    Registry.Reset();
}
