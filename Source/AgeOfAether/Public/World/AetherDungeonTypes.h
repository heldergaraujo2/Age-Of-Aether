#pragma once

#include "CoreMinimal.h"
#include "World/AetherWorldTypes.h"
#include "AetherDungeonTypes.generated.h"

UENUM(BlueprintType)
enum class EAetherDungeonRoomType : uint8
{
    Entrance,
    Corridor,
    Combat,
    Elite,
    Puzzle,
    Treasure,
    Boss,
    Exit,
    Checkpoint
};

UENUM(BlueprintType)
enum class EAetherDungeonEncounterType : uint8
{
    None,
    Creature,
    EliteCreature,
    Boss,
    TriggeredEvent
};

USTRUCT(BlueprintType)
struct FAetherDungeonEncounterDefinition
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FString EncounterID;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    EAetherDungeonEncounterType Type = EAetherDungeonEncounterType::None;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FString TargetID;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    int32 RequiredCount = 1;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    bool bRequiredForProgression = true;

    bool IsValid() const
    {
        return !EncounterID.IsEmpty()
            && Type != EAetherDungeonEncounterType::None
            && !TargetID.IsEmpty()
            && RequiredCount > 0;
    }
};

USTRUCT(BlueprintType)
struct FAetherDungeonRoomDefinition
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FString RoomID;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FString DisplayName;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    EAetherDungeonRoomType Type = EAetherDungeonRoomType::Corridor;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TArray<FString> NextRoomIDs;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TArray<FAetherDungeonEncounterDefinition> Encounters;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    bool bOptional = false;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    bool bCheckpoint = false;

    bool IsValid() const
    {
        if (RoomID.IsEmpty() || DisplayName.IsEmpty())
        {
            return false;
        }

        TSet<FString> EncounterIDs;
        for (const FAetherDungeonEncounterDefinition& Encounter : Encounters)
        {
            if (!Encounter.IsValid() || EncounterIDs.Contains(Encounter.EncounterID))
            {
                return false;
            }
            EncounterIDs.Add(Encounter.EncounterID);
        }

        return true;
    }
};

USTRUCT(BlueprintType)
struct FAetherDungeonDefinition
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FString DungeonID;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FString DisplayName;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FAetherWorldZoneId ZoneID;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FString MapID;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FString EntryPortalID;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FString EntranceRoomID;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FString ExitRoomID;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FString BossRoomID;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FString CompletionQuestID;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FString LootTableID;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FString RewardDefinitionID;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    int32 MinimumLevel = 1;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    int32 RecommendedLevel = 1;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TArray<FAetherDungeonRoomDefinition> Rooms;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    bool bEnabled = true;

    bool IsValid(TArray<FString>* OutErrors = nullptr) const;
};