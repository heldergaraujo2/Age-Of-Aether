#pragma once
#include "CoreMinimal.h"
#include "Data/AetherRecipeTypes.h"

class FAetherContentRegistry;
class FAetherAssetRegistry;
class FAetherItemRegistry;
class FAetherSkillEffectRegistry;
class FAetherQuestDialogueEventRegistry;
class FAetherWorldActorRegistry;

class FAetherRecipeRegistry
{
public:
    bool RegisterRecipe(const FAetherRecipeDefinition& Definition, FString& OutError);
    bool Resolve(const FString& DefinitionID, FAetherRecipeDefinition& OutDefinition) const;
    bool Contains(const FString& DefinitionID) const;
    bool Validate(TArray<FAetherRecipeValidationIssue>& OutIssues,
        const FAetherContentRegistry* ContentRegistry = nullptr,
        const FAetherAssetRegistry* AssetRegistry = nullptr,
        const FAetherItemRegistry* ItemRegistry = nullptr,
        const FAetherSkillEffectRegistry* SkillRegistry = nullptr,
        const FAetherQuestDialogueEventRegistry* QuestRegistry = nullptr,
        const FAetherWorldActorRegistry* WorldActorRegistry = nullptr) const;
    void GetDefinitionIDs(TArray<FString>& OutIDs) const;
    int32 Num() const { return Definitions.Num(); }
    void Reset();
private:
    TMap<FString, FAetherRecipeDefinition> Definitions;
    void AddIssue(TArray<FAetherRecipeValidationIssue>& OutIssues,const FString& ID,const FString& Code,const FString& Message) const;
};
