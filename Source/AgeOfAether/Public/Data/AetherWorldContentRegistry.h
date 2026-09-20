#pragma once
#include "CoreMinimal.h"
#include "Data/AetherWorldContentTypes.h"

class FAetherContentRegistry;
class FAetherAssetRegistry;
class FAetherItemRegistry;
class FAetherWorldActorRegistry;
class FAetherLootRewardRegistry;
class FAetherQuestDialogueEventRegistry;
class FAetherRecipeRegistry;
class FAetherSkillEffectRegistry;

class FAetherWorldContentRegistry
{
public:
    bool RegisterMap(const FAetherMapDefinition& Definition, FString& OutError);
    bool ResolveMap(const FString& ID, FAetherMapDefinition& OutDefinition) const;
    bool ContainsMap(const FString& ID) const;
    bool Validate(TArray<FAetherWorldContentValidationIssue>& OutIssues,
        const FAetherContentRegistry* ContentRegistry = nullptr,
        const FAetherAssetRegistry* AssetRegistry = nullptr,
        const FAetherItemRegistry* ItemRegistry = nullptr,
        const FAetherWorldActorRegistry* WorldActorRegistry = nullptr,
        const FAetherLootRewardRegistry* LootRewardRegistry = nullptr,
        const FAetherQuestDialogueEventRegistry* QuestRegistry = nullptr,
        const FAetherRecipeRegistry* RecipeRegistry = nullptr,
        const FAetherSkillEffectRegistry* SkillRegistry = nullptr) const;
    void GetMapIDs(TArray<FString>& OutIDs) const;
    int32 NumMaps() const { return Maps.Num(); }
    void Reset();
private:
    TMap<FString, FAetherMapDefinition> Maps;
    void AddIssue(TArray<FAetherWorldContentValidationIssue>& OutIssues,const FString& ID,const FString& Code,const FString& Message) const;
};
