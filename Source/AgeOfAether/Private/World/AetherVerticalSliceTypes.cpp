#include "World/AetherVerticalSliceTypes.h"

namespace
{
    void AddError(TArray<FString>* OutErrors, const FString& Error)
    {
        if (OutErrors)
        {
            OutErrors->Add(Error);
        }
    }
}

bool FAetherVerticalSliceStageDefinition::IsValid() const
{
    return !StageID.IsEmpty()
        && !DisplayName.IsEmpty()
        && ZoneID.IsValid()
        && !MapID.IsEmpty()
        && !EntryID.IsEmpty()
        && !CompletionID.IsEmpty();
}

bool FAetherVerticalSliceDefinition::IsValid(TArray<FString>* OutErrors) const
{
    bool bValid = true;

    if (SliceID.IsEmpty())
    {
        AddError(OutErrors, TEXT("Vertical slice ID is empty."));
        bValid = false;
    }

    if (DisplayName.IsEmpty())
    {
        AddError(OutErrors, TEXT("Vertical slice display name is empty."));
        bValid = false;
    }

    if (MinimumLevel < 1)
    {
        AddError(OutErrors, TEXT("Vertical slice minimum level must be positive."));
        bValid = false;
    }

    const FString* RequiredValues[] =
    {
        &SettlementZoneID,
        &OpeningQuestID,
        &EnemyCreatureID,
        &LootTableID,
        &DungeonID,
        &BossCreatureID,
        &CompletionQuestID
    };

    for (const FString* Value : RequiredValues)
    {
        if (Value->IsEmpty())
        {
            bValid = false;
        }
    }

    if (!bValid)
    {
        AddError(OutErrors, TEXT("Vertical slice integration identity is incomplete."));
    }

    if (Stages.Num() != 8)
    {
        AddError(OutErrors, TEXT("Vertical slice must define exactly eight canonical stages."));
        bValid = false;
    }

    TSet<FString> StageIDs;
    TSet<uint8> StageKinds;
    for (int32 Index = 0; Index < Stages.Num(); ++Index)
    {
        const FAetherVerticalSliceStageDefinition& StageDefinition = Stages[Index];

        if (!StageDefinition.IsValid() || StageIDs.Contains(StageDefinition.StageID))
        {
            AddError(OutErrors, FString::Printf(TEXT("Invalid or duplicate vertical slice stage: %s"), *StageDefinition.StageID));
            bValid = false;
            continue;
        }

        StageIDs.Add(StageDefinition.StageID);

        const uint8 StageKind = static_cast<uint8>(StageDefinition.Stage);
        if (StageKinds.Contains(StageKind))
        {
            AddError(OutErrors, TEXT("Vertical slice stage types must be unique."));
            bValid = false;
        }
        StageKinds.Add(StageKind);

        if (Index > 0 && static_cast<uint8>(StageDefinition.Stage) != static_cast<uint8>(Index))
        {
            AddError(OutErrors, TEXT("Vertical slice stages must follow the canonical order."));
            bValid = false;
        }
    }

    return bValid;
}

FAetherVerticalSliceDefinition FAetherVerticalSliceDefinition::CreateFirstPermanentSlice()
{
    FAetherVerticalSliceDefinition Definition;
    Definition.SliceID = TEXT("AOA.VerticalSlice.FirstPermanent");
    Definition.DisplayName = TEXT("First Permanent Vertical Slice");
    Definition.MinimumLevel = 1;
    Definition.SettlementZoneID = TEXT("Region.FirstPermanent.Settlement");
    Definition.OpeningQuestID = TEXT("AOA.Quest.FirstRegion.FirstHunt");
    Definition.EnemyCreatureID = TEXT("AOA.Creature.FirstRegion.WildHound");
    Definition.LootTableID = TEXT("AOA.Loot.FirstRegion.WildHound");
    Definition.DungeonID = TEXT("AOA.Dungeon.FirstRegion.HollowedWatch");
    Definition.BossCreatureID = TEXT("AOA.Creature.FirstRegion.HollowedWatchWarden");
    Definition.CompletionQuestID = TEXT("AOA.Quest.FirstRegion.HollowedWatch");

    auto AddStage = [&Definition](
        const TCHAR* StageID,
        EAetherVerticalSliceStage Stage,
        const TCHAR* DisplayName,
        const TCHAR* ZoneID,
        const TCHAR* MapID,
        const TCHAR* EntryID,
        const TCHAR* CompletionID)
    {
        FAetherVerticalSliceStageDefinition StageDefinition;
        StageDefinition.StageID = StageID;
        StageDefinition.Stage = Stage;
        StageDefinition.DisplayName = DisplayName;
        StageDefinition.ZoneID = FAetherWorldZoneId{FString(ZoneID)};
        StageDefinition.MapID = MapID;
        StageDefinition.EntryID = EntryID;
        StageDefinition.CompletionID = CompletionID;
        Definition.Stages.Add(MoveTemp(StageDefinition));
    };

    AddStage(
        TEXT("AOA.VerticalSlice.Stage.Settlement"),
        EAetherVerticalSliceStage::Settlement,
        TEXT("Settlement"),
        TEXT("Region.FirstPermanent.Settlement"),
        TEXT("AOA.Map.FirstRegion.Settlement"),
        TEXT("AOA.Point.FirstRegion.SettlementEntry"),
        TEXT("AOA.Point.FirstRegion.QuestHub"));

    AddStage(
        TEXT("AOA.VerticalSlice.Stage.Quest"),
        EAetherVerticalSliceStage::Quest,
        TEXT("Opening Quest"),
        TEXT("Region.FirstPermanent.Settlement"),
        TEXT("AOA.Map.FirstRegion.Settlement"),
        TEXT("AOA.Quest.FirstRegion.FirstHunt"),
        TEXT("AOA.Quest.FirstRegion.FirstHunt"));

    AddStage(
        TEXT("AOA.VerticalSlice.Stage.Exploration"),
        EAetherVerticalSliceStage::Exploration,
        TEXT("Frontier Exploration"),
        TEXT("Region.FirstPermanent.Wilderness"),
        TEXT("AOA.Map.FirstRegion.Wilderness"),
        TEXT("AOA.Point.FirstRegion.ForestEntry"),
        TEXT("AOA.Point.FirstRegion.CombatFrontier"));

    AddStage(
        TEXT("AOA.VerticalSlice.Stage.Combat"),
        EAetherVerticalSliceStage::Combat,
        TEXT("First Combat"),
        TEXT("Region.FirstPermanent.CombatFrontier"),
        TEXT("AOA.Map.FirstRegion.Wilderness"),
        TEXT("AOA.Spawn.FirstRegion.WildHound.Frontier"),
        TEXT("AOA.Creature.FirstRegion.WildHound"));

    AddStage(
        TEXT("AOA.VerticalSlice.Stage.Reward"),
        EAetherVerticalSliceStage::Reward,
        TEXT("First Reward"),
        TEXT("Region.FirstPermanent.CombatFrontier"),
        TEXT("AOA.Map.FirstRegion.Wilderness"),
        TEXT("AOA.Loot.FirstRegion.WildHound"),
        TEXT("AOA.Reward.FirstRegion.WildHound"));

    AddStage(
        TEXT("AOA.VerticalSlice.Stage.Dungeon"),
        EAetherVerticalSliceStage::Dungeon,
        TEXT("Dungeon Approach"),
        TEXT("Region.FirstPermanent.Dungeon"),
        TEXT("AOA.Map.FirstRegion.HollowedWatch"),
        TEXT("AOA.Portal.FirstRegion.DungeonEntrance"),
        TEXT("AOA.Dungeon.FirstRegion.HollowedWatch"));

    AddStage(
        TEXT("AOA.VerticalSlice.Stage.Boss"),
        EAetherVerticalSliceStage::Boss,
        TEXT("Dungeon Boss"),
        TEXT("Region.FirstPermanent.Dungeon"),
        TEXT("AOA.Map.FirstRegion.HollowedWatch"),
        TEXT("AOA.Creature.FirstRegion.HollowedWatchWarden"),
        TEXT("AOA.Quest.FirstRegion.HollowedWatch"));

    AddStage(
        TEXT("AOA.VerticalSlice.Stage.Return"),
        EAetherVerticalSliceStage::Return,
        TEXT("Return to Settlement"),
        TEXT("Region.FirstPermanent.Settlement"),
        TEXT("AOA.Map.FirstRegion.Settlement"),
        TEXT("AOA.Point.FirstRegion.SettlementGate"),
        TEXT("AOA.Quest.FirstRegion.HollowedWatch"));

    return Definition;
}
