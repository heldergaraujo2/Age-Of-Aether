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