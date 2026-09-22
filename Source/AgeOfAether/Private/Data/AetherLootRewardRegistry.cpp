#include "Data/AetherLootRewardRegistry.h"
#include "Data/AetherContentRegistry.h"
#include "Data/AetherAssetRegistry.h"
#include "Data/AetherItemRegistry.h"
#include "Data/AetherWorldActorRegistry.h"

namespace
{
    FString NormalizeID(const FString& ID) { return ID.TrimStartAndEnd(); }
}

void FAetherLootRewardRegistry::AddIssue(TArray<FAetherLootRewardValidationIssue>& OutIssues,
    const FString& ID, const FString& Code, const FString& Message) const
{
    FAetherLootRewardValidationIssue Issue;
    Issue.DefinitionID = ID;
    Issue.Code = Code;
    Issue.Message = Message;
    OutIssues.Add(Issue);
}

bool FAetherLootRewardRegistry::IsIDAvailable(const FString& ID) const
{
    const FString Normalized = NormalizeID(ID);
    return !Normalized.IsEmpty() && !LootTables.Contains(Normalized) && !Rewards.Contains(Normalized) && !Respawns.Contains(Normalized) && !DropRules.Contains(Normalized) && !SpawnGroups.Contains(Normalized);
}

bool FAetherLootRewardRegistry::RegisterLootTable(const FAetherLootTableDefinition& Definition, FString& OutError)
{
    OutError.Reset();
    FAetherLootTableDefinition Copy = Definition;
    Copy.DefinitionID = NormalizeID(Copy.DefinitionID);
    if (!Copy.IsStructurallyValid(OutError) || !IsIDAvailable(Copy.DefinitionID))
    {
        if (OutError.IsEmpty()) { OutError = TEXT("DefinitionID is already registered or invalid."); }
        return false;
    }
    LootTables.Add(Copy.DefinitionID, Copy);
    return true;
}

bool FAetherLootRewardRegistry::RegisterReward(const FAetherRewardDefinition& Definition, FString& OutError)
{
    OutError.Reset();
    FAetherRewardDefinition Copy = Definition;
    Copy.DefinitionID = NormalizeID(Copy.DefinitionID);
    if (!Copy.IsStructurallyValid(OutError) || !IsIDAvailable(Copy.DefinitionID))
    {
        if (OutError.IsEmpty()) { OutError = TEXT("DefinitionID is already registered or invalid."); }
        return false;
    }
    Rewards.Add(Copy.DefinitionID, Copy);
    return true;
}

bool FAetherLootRewardRegistry::RegisterRespawn(const FAetherRespawnDefinition& Definition, FString& OutError)
{
    OutError.Reset();
    FAetherRespawnDefinition Copy = Definition;
    Copy.DefinitionID = NormalizeID(Copy.DefinitionID);
    if (!Copy.IsStructurallyValid(OutError) || !IsIDAvailable(Copy.DefinitionID))
    {
        if (OutError.IsEmpty()) { OutError = TEXT("DefinitionID is already registered or invalid."); }
        return false;
    }
    Respawns.Add(Copy.DefinitionID, Copy);
    return true;
}

bool FAetherLootRewardRegistry::RegisterDropRule(const FAetherDropRuleDefinition& Definition, FString& OutError)
{
    OutError.Reset();
    FAetherDropRuleDefinition Copy = Definition;
    Copy.DefinitionID = NormalizeID(Copy.DefinitionID);
    if (!Copy.IsStructurallyValid(OutError) || !IsIDAvailable(Copy.DefinitionID))
    {
        if (OutError.IsEmpty()) { OutError = TEXT("DefinitionID is already registered or invalid."); }
        return false;
    }
    DropRules.Add(Copy.DefinitionID, Copy);
    return true;
}

bool FAetherLootRewardRegistry::RegisterSpawnGroup(const FAetherSpawnGroupDefinition& Definition, FString& OutError)
{
    OutError.Reset();
    FAetherSpawnGroupDefinition Copy = Definition;
    Copy.DefinitionID = NormalizeID(Copy.DefinitionID);
    if (!Copy.IsStructurallyValid(OutError) || !IsIDAvailable(Copy.DefinitionID))
    {
        if (OutError.IsEmpty()) { OutError = TEXT("DefinitionID is already registered or invalid."); }
        return false;
    }
    SpawnGroups.Add(Copy.DefinitionID, Copy);
    return true;
}

bool FAetherLootRewardRegistry::ResolveLootTable(const FString& ID, FAetherLootTableDefinition& OutDefinition) const
{
    const FAetherLootTableDefinition* Found = LootTables.Find(NormalizeID(ID));
    if (!Found) { return false; }
    OutDefinition = *Found;
    return true;
}

bool FAetherLootRewardRegistry::ResolveReward(const FString& ID, FAetherRewardDefinition& OutDefinition) const
{
    const FAetherRewardDefinition* Found = Rewards.Find(NormalizeID(ID));
    if (!Found) { return false; }
    OutDefinition = *Found;
    return true;
}

bool FAetherLootRewardRegistry::ResolveRespawn(const FString& ID, FAetherRespawnDefinition& OutDefinition) const
{
    const FAetherRespawnDefinition* Found = Respawns.Find(NormalizeID(ID));
    if (!Found) { return false; }
    OutDefinition = *Found;
    return true;
}

bool FAetherLootRewardRegistry::ResolveDropRule(const FString& ID, FAetherDropRuleDefinition& OutDefinition) const
{
    const FAetherDropRuleDefinition* Found = DropRules.Find(NormalizeID(ID));
    if (!Found) { return false; }
    OutDefinition = *Found;
    return true;
}

bool FAetherLootRewardRegistry::ResolveSpawnGroup(const FString& ID, FAetherSpawnGroupDefinition& OutDefinition) const
{
    const FAetherSpawnGroupDefinition* Found = SpawnGroups.Find(NormalizeID(ID));
    if (!Found) { return false; }
    OutDefinition = *Found;
    return true;
}

bool FAetherLootRewardRegistry::Contains(const FString& ID) const
{
    const FString Normalized = NormalizeID(ID);
    return LootTables.Contains(Normalized) || Rewards.Contains(Normalized) || Respawns.Contains(Normalized) || DropRules.Contains(Normalized) || SpawnGroups.Contains(Normalized);
}

void FAetherLootRewardRegistry::GetDefinitionIDs(TArray<FString>& OutIDs) const
{
    OutIDs.Reset();
    for (const auto& Pair : LootTables) { OutIDs.Add(Pair.Key); }
    for (const auto& Pair : Rewards) { OutIDs.Add(Pair.Key); }
    for (const auto& Pair : Respawns) { OutIDs.Add(Pair.Key); }
    for (const auto& Pair : DropRules) { OutIDs.Add(Pair.Key); }
    for (const auto& Pair : SpawnGroups) { OutIDs.Add(Pair.Key); }
    OutIDs.Sort();
}

void FAetherLootRewardRegistry::Reset()
{
    LootTables.Reset();
    Rewards.Reset();
    Respawns.Reset();
    DropRules.Reset();
    SpawnGroups.Reset();
}

bool FAetherLootRewardRegistry::Validate(TArray<FAetherLootRewardValidationIssue>& OutIssues,
    const FAetherContentRegistry* ContentRegistry,
    const FAetherAssetRegistry* AssetRegistry,
    const FAetherItemRegistry* ItemRegistry,
    const FAetherWorldActorRegistry* WorldActorRegistry) const
{
    OutIssues.Reset();
    auto ValidateItem = [&](const FString& OwnerID, const FString& ItemID)
    {
        if (ItemRegistry && !ItemRegistry->Contains(ItemID))
        {
            AddIssue(OutIssues, OwnerID, TEXT("MissingItemReference"), FString::Printf(TEXT("Missing item '%s'."), *ItemID));
        }
    };
    auto ValidateContent = [&](const FString& OwnerID, const FString& ID)
    {
        if (ContentRegistry && !ContentRegistry->Contains(ID))
        {
            AddIssue(OutIssues, OwnerID, TEXT("MissingContentReference"), FString::Printf(TEXT("Missing content '%s'."), *ID));
        }
    };

    for (const auto& Pair : LootTables)
    {
        const FAetherLootTableDefinition& Definition = Pair.Value;
        for (const FAetherDataLootEntry& Entry : Definition.Entries)
        {
            ValidateItem(Definition.DefinitionID, Entry.ItemID);
        }
    }

    for (const auto& Pair : Rewards)
    {
        const FAetherRewardDefinition& Definition = Pair.Value;
        for (const FAetherRewardItem& Item : Definition.Items)
        {
            ValidateItem(Definition.DefinitionID, Item.ItemID);
        }
        for (const FString& LootID : Definition.LootTableIDs)
        {
            if (!LootTables.Contains(NormalizeID(LootID)))
            {
                AddIssue(OutIssues, Definition.DefinitionID, TEXT("MissingLootTableReference"), FString::Printf(TEXT("Missing loot table '%s'."), *LootID));
            }
        }
    }

    for (const auto& Pair : Respawns)
    {
        const FAetherRespawnDefinition& Definition = Pair.Value;
        if (WorldActorRegistry)
        {
            if (!WorldActorRegistry->Contains(Definition.WorldActorID))
            {
                AddIssue(OutIssues, Definition.DefinitionID, TEXT("MissingWorldActorReference"), FString::Printf(TEXT("Missing world actor '%s'."), *Definition.WorldActorID));
            }
        }
    }

    for (const auto& Pair : DropRules)
    {
        const FAetherDropRuleDefinition& Definition = Pair.Value;
        if (WorldActorRegistry && !WorldActorRegistry->Contains(Definition.SourceWorldActorID))
        {
            AddIssue(OutIssues, Definition.DefinitionID, TEXT("MissingWorldActorReference"), FString::Printf(TEXT("Missing source world actor '%s'."), *Definition.SourceWorldActorID));
        }
        if (!LootTables.Contains(NormalizeID(Definition.LootTableID)))
        {
            AddIssue(OutIssues, Definition.DefinitionID, TEXT("MissingLootTableReference"), FString::Printf(TEXT("Missing loot table '%s'."), *Definition.LootTableID));
        }
    }

    for (const auto& Pair : SpawnGroups)
    {
        const FAetherSpawnGroupDefinition& Definition = Pair.Value;
        if (WorldActorRegistry && !WorldActorRegistry->Contains(Definition.WorldActorID))
        {
            AddIssue(OutIssues, Definition.DefinitionID, TEXT("MissingWorldActorReference"), FString::Printf(TEXT("Missing spawn world actor '%s'."), *Definition.WorldActorID));
        }
        if (!Respawns.Contains(NormalizeID(Definition.RespawnDefinitionID)))
        {
            AddIssue(OutIssues, Definition.DefinitionID, TEXT("MissingRespawnReference"), FString::Printf(TEXT("Missing respawn definition '%s'."), *Definition.RespawnDefinitionID));
        }
    }

    if (ContentRegistry)
    {
        for (const auto& Pair : LootTables)
        {
            ValidateContent(Pair.Key, Pair.Key);
        }
        for (const auto& Pair : Rewards)
        {
            ValidateContent(Pair.Key, Pair.Key);
        }
        for (const auto& Pair : Respawns)
        {
            ValidateContent(Pair.Key, Pair.Key);
        }
        for (const auto& Pair : DropRules)
        {
            ValidateContent(Pair.Key, Pair.Key);
        }
        for (const auto& Pair : SpawnGroups)
        {
            ValidateContent(Pair.Key, Pair.Key);
        }
    }

    return OutIssues.Num() == 0;
}
