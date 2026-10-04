#include "World/AetherDungeonSubsystem.h"

bool UAetherDungeonSubsystem::FindDungeon(const FString& DungeonID, FAetherDungeonDefinition& OutDefinition) const
{
    const FAetherDungeonDefinition* Found = Registry.FindDungeon(DungeonID);
    if (!Found)
    {
        return false;
    }

    OutDefinition = *Found;
    return true;
}

bool UAetherDungeonSubsystem::ValidateDungeons(TArray<FString>& OutErrors) const
{
    return Registry.Validate(OutErrors);
}