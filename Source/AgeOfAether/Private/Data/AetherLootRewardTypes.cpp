#include "Data/AetherLootRewardTypes.h"

namespace
{
    bool IsFiniteNonNegative(const double Value)
    {
        return FMath::IsFinite(Value) && Value >= 0.0;
    }
}

bool FAetherDataLootEntry::IsStructurallyValid(FString& OutError) const
{
    OutError.Reset();
    if (ItemID.TrimStartAndEnd().IsEmpty())
    {
        OutError = TEXT("ItemID is required.");
        return false;
    }
    if (MinQuantity <= 0 || MaxQuantity < MinQuantity)
    {
        OutError = TEXT("Loot quantities must be positive and MaxQuantity must be >= MinQuantity.");
        return false;
    }
    if (!IsFiniteNonNegative(Weight) || !IsFiniteNonNegative(Chance))
    {
        OutError = TEXT("Loot Weight and Chance must be finite and non-negative.");
        return false;
    }
    if (Chance > 1.0)
    {
        OutError = TEXT("Loot Chance must be in [0,1].");
        return false;
    }
    return true;
}

bool FAetherLootTableDefinition::IsStructurallyValid(FString& OutError) const
{
    OutError.Reset();
    if (DefinitionID.TrimStartAndEnd().IsEmpty() || DisplayName.TrimStartAndEnd().IsEmpty())
    {
        OutError = TEXT("Loot table ID and display name are required.");
        return false;
    }
    if (RollCount <= 0 || Entries.Num() <= 0)
    {
        OutError = TEXT("Loot table must have a positive RollCount and at least one entry.");
        return false;
    }
    if (MinimumLevel < 0 || MaximumLevel < 0 || (MaximumLevel > 0 && MaximumLevel < MinimumLevel))
    {
        OutError = TEXT("Loot table level bounds are invalid.");
        return false;
    }
    for (const FAetherDataLootEntry& Entry : Entries)
    {
        if (!Entry.IsStructurallyValid(OutError))
        {
            return false;
        }
    }
    if (SelectionMode == EAetherLootSelectionMode::WeightedSingle)
    {
        double TotalWeight = 0.0;
        for (const FAetherDataLootEntry& Entry : Entries) { TotalWeight += Entry.Weight; }
        if (!FMath::IsFinite(TotalWeight) || TotalWeight <= 0.0)
        {
            OutError = TEXT("WeightedSingle requires a positive total weight.");
            return false;
        }
    }
    if (SelectionMode == EAetherLootSelectionMode::ChanceEach)
    {
        bool HasPositiveChance = false;
        for (const FAetherDataLootEntry& Entry : Entries) { HasPositiveChance |= Entry.Chance > 0.0; }
        if (!HasPositiveChance)
        {
            OutError = TEXT("ChanceEach requires at least one positive chance.");
            return false;
        }
    }
    return true;
}

bool FAetherDropRuleDefinition::IsStructurallyValid(FString& OutError) const
{
    OutError.Reset();
    if (DefinitionID.TrimStartAndEnd().IsEmpty() || SourceWorldActorID.TrimStartAndEnd().IsEmpty() || LootTableID.TrimStartAndEnd().IsEmpty())
    {
        OutError = TEXT("Drop rule ID, SourceWorldActorID and LootTableID are required.");
        return false;
    }
    if (!IsFiniteNonNegative(Chance) || Chance > 1.0)
    {
        OutError = TEXT("Drop rule Chance must be in [0,1].");
        return false;
    }
    if (MinimumLevel < 0 || MaximumLevel < 0 || (MaximumLevel > 0 && MaximumLevel < MinimumLevel))
    {
        OutError = TEXT("Drop rule level bounds are invalid.");
        return false;
    }
    return true;
}

bool FAetherSpawnGroupDefinition::IsStructurallyValid(FString& OutError) const
{
    OutError.Reset();
    if (DefinitionID.TrimStartAndEnd().IsEmpty() || WorldActorID.TrimStartAndEnd().IsEmpty() || RespawnDefinitionID.TrimStartAndEnd().IsEmpty())
    {
        OutError = TEXT("Spawn group ID, WorldActorID and RespawnDefinitionID are required.");
        return false;
    }
    if (InitialCount < 0 || MaximumCount <= 0 || InitialCount > MaximumCount)
    {
        OutError = TEXT("Spawn group counts are invalid.");
        return false;
    }
    return true;
}

bool FAetherRewardItem::IsStructurallyValid(FString& OutError) const
{
    OutError.Reset();
    if (ItemID.TrimStartAndEnd().IsEmpty() || Quantity <= 0)
    {
        OutError = TEXT("Reward item requires an ItemID and positive quantity.");
        return false;
    }
    return true;
}

bool FAetherRewardDefinition::IsStructurallyValid(FString& OutError) const
{
    OutError.Reset();
    if (DefinitionID.TrimStartAndEnd().IsEmpty() || DisplayName.TrimStartAndEnd().IsEmpty())
    {
        OutError = TEXT("Reward ID and display name are required.");
        return false;
    }
    if (!IsFiniteNonNegative(Experience))
    {
        OutError = TEXT("Experience must be finite and non-negative.");
        return false;
    }
    for (const TPair<FString, int64>& Pair : Currency)
    {
        if (Pair.Key.TrimStartAndEnd().IsEmpty() || Pair.Value < 0)
        {
            OutError = TEXT("Currency reward keys must be non-empty and values non-negative.");
            return false;
        }
    }
    for (const FAetherRewardItem& Item : Items)
    {
        if (!Item.IsStructurallyValid(OutError))
        {
            return false;
        }
    }
    if (Experience <= 0.0 && Currency.Num() == 0 && Items.Num() == 0 && LootTableIDs.Num() == 0)
    {
        OutError = TEXT("Reward definition must contain at least one reward.");
        return false;
    }
    return true;
}

bool FAetherRespawnDefinition::IsStructurallyValid(FString& OutError) const
{
    OutError.Reset();
    if (DefinitionID.TrimStartAndEnd().IsEmpty() || WorldActorID.TrimStartAndEnd().IsEmpty())
    {
        OutError = TEXT("Respawn definition and WorldActorID are required.");
        return false;
    }
    if (!IsFiniteNonNegative(RespawnSeconds) || !IsFiniteNonNegative(RespawnJitterSeconds))
    {
        OutError = TEXT("Respawn timing must be finite and non-negative.");
        return false;
    }
    if (RespawnJitterSeconds > RespawnSeconds && RespawnSeconds > 0.0)
    {
        OutError = TEXT("Respawn jitter cannot exceed the base respawn time.");
        return false;
    }
    if (MaxAlive <= 0)
    {
        OutError = TEXT("MaxAlive must be positive.");
        return false;
    }
    if (!bRespawnEnabled && (RespawnSeconds > 0.0 || RespawnJitterSeconds > 0.0))
    {
        OutError = TEXT("Disabled respawn cannot define active respawn timing.");
        return false;
    }
    return true;
}
