#include "World/AetherDungeonTypes.h"

bool FAetherDungeonDefinition::IsValid(TArray<FString>* OutErrors) const
{
    auto AddError = [OutErrors](const FString& Error)
    {
        if (OutErrors)
        {
            OutErrors->Add(Error);
        }
    };

    bool bValid = true;

    if (DungeonID.IsEmpty())
    {
        AddError(TEXT("DungeonID is empty."));
        bValid = false;
    }

    if (DisplayName.IsEmpty())
    {
        AddError(TEXT("Dungeon DisplayName is empty."));
        bValid = false;
    }

    if (!ZoneID.IsValid())
    {
        AddError(TEXT("Dungeon ZoneID is invalid."));
        bValid = false;
    }

    if (MapID.IsEmpty() || EntryPortalID.IsEmpty() || EntranceRoomID.IsEmpty() || ExitRoomID.IsEmpty() || BossRoomID.IsEmpty())
    {
        AddError(TEXT("Dungeon map/portal/room identity is incomplete."));
        bValid = false;
    }

    if (MinimumLevel < 1 || RecommendedLevel < MinimumLevel)
    {
        AddError(TEXT("Dungeon level requirements are invalid."));
        bValid = false;
    }

    if (Rooms.Num() == 0)
    {
        AddError(TEXT("Dungeon must define at least one room."));
        bValid = false;
    }

    TSet<FString> RoomIDs;
    for (const FAetherDungeonRoomDefinition& Room : Rooms)
    {
        if (!Room.IsValid() || RoomIDs.Contains(Room.RoomID))
        {
            AddError(FString::Printf(TEXT("Invalid or duplicate dungeon room: %s"), *Room.RoomID));
            bValid = false;
            continue;
        }

        RoomIDs.Add(Room.RoomID);
    }

    if (!RoomIDs.Contains(EntranceRoomID) || !RoomIDs.Contains(ExitRoomID) || !RoomIDs.Contains(BossRoomID))
    {
        AddError(TEXT("Entrance, exit, and boss rooms must exist in the dungeon room set."));
        bValid = false;
    }

    for (const FAetherDungeonRoomDefinition& Room : Rooms)
    {
        for (const FString& NextRoomID : Room.NextRoomIDs)
        {
            if (!RoomIDs.Contains(NextRoomID))
            {
                AddError(FString::Printf(TEXT("Room %s references missing next room %s."), *Room.RoomID, *NextRoomID));
                bValid = false;
            }
        }
    }

    return bValid;
}