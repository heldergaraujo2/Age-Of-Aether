#pragma once
#include "CoreMinimal.h"
#include "Data/AetherClassTypes.h"
class FAetherItemRegistry;
class FAetherSkillEffectRegistry;
class FAetherQuestDialogueEventRegistry;
class FAetherContentRegistry;
class FAetherAssetRegistry;

class FAetherClassRegistry
{
public:
    bool RegisterClass(const FAetherClassDefinition& Definition, FString& OutError);
    bool RegisterEvolution(const FAetherClassEvolutionDefinition& Definition, FString& OutError);
    bool ResolveClass(const FString& ClassID, FAetherClassDefinition& OutDefinition) const;
    bool ResolveEvolution(const FString& EvolutionID, FAetherClassEvolutionDefinition& OutDefinition) const;
    bool ContainsClass(const FString& ClassID) const;
    bool ContainsEvolution(const FString& EvolutionID) const;
    bool RemoveClass(const FString& ClassID);
    bool RemoveEvolution(const FString& EvolutionID);
    void Reset();
    bool Validate(TArray<FAetherClassValidationIssue>& OutIssues,
        const FAetherItemRegistry* ItemRegistry = nullptr,
        const FAetherSkillEffectRegistry* SkillRegistry = nullptr,
        const FAetherQuestDialogueEventRegistry* QuestRegistry = nullptr,
        const FAetherContentRegistry* ContentRegistry = nullptr,
        const FAetherAssetRegistry* AssetRegistry = nullptr) const;
    void GetClassIDs(TArray<FString>& OutIDs) const;
    void GetEvolutionIDs(TArray<FString>& OutIDs) const;
    void GetEvolutionIDsForClass(const FString& ClassID, TArray<FString>& OutIDs) const;
    int32 NumClasses() const { return Classes.Num(); }
    int32 NumEvolutions() const { return Evolutions.Num(); }
private:
    TMap<FString, FAetherClassDefinition> Classes;
    TMap<FString, FAetherClassEvolutionDefinition> Evolutions;
    static FString Normalize(const FString& ID);
    static void AddIssue(TArray<FAetherClassValidationIssue>& OutIssues,const FString& ID,const FString& Code,const FString& Message);
};
