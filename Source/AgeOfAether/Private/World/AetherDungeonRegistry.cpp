#include "World/AetherDungeonRegistry.h"

bool FAetherDungeonRegistry::RegisterDungeon(const FAetherDungeonDefinition& Definition)
{
    if (!Definition.IsValid())
    {
        return false;
    }

    const FString Key = Normalize(Definition.DungeonID);
    if (Key.IsEmpty() || Dungeons.Contains(Key))
    {
        return false;
    }

    Dungeons.Add(Key, Definition);
    return true;
}

const FAetherDungeonDefinition* FAetherDungeonRegistry::FindDungeon(const FString& DungeonID) const
{
    const FString Key = Normalize(DungeonID);
    return Dungeons.Find(Key);
}

bool FAetherDungeonRegistry::Validate(TArray<FString>& OutErrors) const
{
    OutErrors.Reset();

    for (const TPair<FString, FAetherDungeonDefinition>& Pair : Dungeons)
    {
        Pair.Value.IsValid(&OutErrors);
    }

    return OutErrors.Num() == 0;
}

void FAetherDungeonRegistry::Reset()
{
    Dungeons.Reset();
}