#include "World/AetherDungeonCatalog.h"

#include "World/AetherDungeonSubsystem.h"

bool UAetherDungeonCatalog::IsValid(TArray<FString>& OutErrors) const
{
    OutErrors.Reset();

    TSet<FString> DungeonIDs;
    for (const FAetherDungeonDefinition& Dungeon : Dungeons)
    {
        if (!Dungeon.IsValid(&OutErrors))
        {
            continue;
        }

        const FString Key = Dungeon.DungeonID.TrimStartAndEnd().ToLower();
        if (DungeonIDs.Contains(Key))
        {
            OutErrors.Add(FString::Printf(TEXT("Duplicate dungeon ID: %s"), *Dungeon.DungeonID));
        }
        DungeonIDs.Add(Key);
    }

    return OutErrors.Num() == 0;
}

void UAetherDungeonCatalog::RegisterInto(UAetherDungeonSubsystem* Subsystem) const
{
    if (!Subsystem)
    {
        return;
    }

    for (const FAetherDungeonDefinition& Dungeon : Dungeons)
    {
        Subsystem->RegisterDungeon(Dungeon);
    }
}