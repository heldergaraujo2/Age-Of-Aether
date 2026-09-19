#pragma once
#include "CoreMinimal.h"
#include "Data/AetherLootRewardTypes.h"

class FAetherContentRegistry;
class FAetherAssetRegistry;
class FAetherItemRegistry;
class FAetherWorldActorRegistry;

class FAetherLootRewardRegistry
{
public:
    bool RegisterLootTable(const FAetherLootTableDefinition& Definition, FString& OutError);
    bool RegisterReward(const FAetherRewardDefinition& Definition, FString& OutError);
    bool RegisterRespawn(const FAetherRespawnDefinition& Definition, FString& OutError);
    bool RegisterDropRule(const FAetherDropRuleDefinition& Definition, FString& OutError);
    bool RegisterSpawnGroup(const FAetherSpawnGroupDefinition& Definition, FString& OutError);

    bool ResolveLootTable(const FString& ID, FAetherLootTableDefinition& OutDefinition) const;
    bool ResolveReward(const FString& ID, FAetherRewardDefinition& OutDefinition) const;
    bool ResolveRespawn(const FString& ID, FAetherRespawnDefinition& OutDefinition) const;
    bool ResolveDropRule(const FString& ID, FAetherDropRuleDefinition& OutDefinition) const;
    bool ResolveSpawnGroup(const FString& ID, FAetherSpawnGroupDefinition& OutDefinition) const;
    bool Contains(const FString& ID) const;

    bool Validate(TArray<FAetherLootRewardValidationIssue>& OutIssues,
        const FAetherContentRegistry* ContentRegistry = nullptr,
        const FAetherAssetRegistry* AssetRegistry = nullptr,
        const FAetherItemRegistry* ItemRegistry = nullptr,
        const FAetherWorldActorRegistry* WorldActorRegistry = nullptr) const;

    void GetDefinitionIDs(TArray<FString>& OutIDs) const;
    int32 NumLootTables() const { return LootTables.Num(); }
    int32 NumRewards() const { return Rewards.Num(); }
    int32 NumRespawns() const { return Respawns.Num(); }
    int32 NumDropRules() const { return DropRules.Num(); }
    int32 NumSpawnGroups() const { return SpawnGroups.Num(); }
    void Reset();

private:
    TMap<FString, FAetherLootTableDefinition> LootTables;
    TMap<FString, FAetherRewardDefinition> Rewards;
    TMap<FString, FAetherRespawnDefinition> Respawns;
    TMap<FString, FAetherDropRuleDefinition> DropRules;
    TMap<FString, FAetherSpawnGroupDefinition> SpawnGroups;

    bool IsIDAvailable(const FString& ID) const;
    void AddIssue(TArray<FAetherLootRewardValidationIssue>& OutIssues,
        const FString& ID, const FString& Code, const FString& Message) const;
};
