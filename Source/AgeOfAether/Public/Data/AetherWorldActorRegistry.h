#pragma once
#include "CoreMinimal.h"
#include "Data/AetherWorldActorTypes.h"

class FAetherContentRegistry;
class FAetherAssetRegistry;

class FAetherWorldActorRegistry
{
public:
    bool RegisterMonster(const FAetherMonsterDefinition& Definition, FString& OutError);
    bool RegisterNPC(const FAetherNPCDefinition& Definition, FString& OutError);
    bool RegisterBoss(const FAetherBossDefinition& Definition, FString& OutError);

    bool ResolveMonster(const FString& ID, FAetherMonsterDefinition& OutDefinition) const;
    bool ResolveNPC(const FString& ID, FAetherNPCDefinition& OutDefinition) const;
    bool ResolveBoss(const FString& ID, FAetherBossDefinition& OutDefinition) const;
    bool Contains(const FString& ID) const;

    bool Validate(TArray<FAetherWorldActorValidationIssue>& OutIssues,
        const FAetherContentRegistry* ContentRegistry = nullptr,
        const FAetherAssetRegistry* AssetRegistry = nullptr) const;

    void GetDefinitionIDs(TArray<FString>& OutIDs) const;
    int32 NumMonsters() const { return Monsters.Num(); }
    int32 NumNPCs() const { return NPCs.Num(); }
    int32 NumBosses() const { return Bosses.Num(); }
    void Reset();

private:
    TMap<FString, FAetherMonsterDefinition> Monsters;
    TMap<FString, FAetherNPCDefinition> NPCs;
    TMap<FString, FAetherBossDefinition> Bosses;

    bool IsIDAvailable(const FString& ID) const;
    void AddIssue(TArray<FAetherWorldActorValidationIssue>& OutIssues,
        const FString& ID, const FString& Code, const FString& Message) const;
};
