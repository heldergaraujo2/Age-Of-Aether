#pragma once
#include "CoreMinimal.h"
#include "Data/AetherQuestDialogueEventTypes.h"

class FAetherContentRegistry;
class FAetherAssetRegistry;
class FAetherItemRegistry;
class FAetherWorldActorRegistry;
class FAetherLootRewardRegistry;

class FAetherQuestDialogueEventRegistry
{
public:
    bool RegisterQuest(const FAetherDataQuestDefinition& Definition, FString& OutError);
    bool RegisterDialogue(const FAetherDialogueDefinition& Definition, FString& OutError);
    bool RegisterWorldEvent(const FAetherWorldEventDefinition& Definition, FString& OutError);
    bool RegisterWorldContent(const FAetherWorldContentDefinition& Definition, FString& OutError);

    bool ResolveQuest(const FString& ID, FAetherDataQuestDefinition& OutDefinition) const;
    bool ResolveDialogue(const FString& ID, FAetherDialogueDefinition& OutDefinition) const;
    bool ResolveWorldEvent(const FString& ID, FAetherWorldEventDefinition& OutDefinition) const;
    bool ResolveWorldContent(const FString& ID, FAetherWorldContentDefinition& OutDefinition) const;
    bool Contains(const FString& ID) const;

    bool Validate(TArray<FAetherQuestDialogueEventValidationIssue>& OutIssues,
        const FAetherContentRegistry* ContentRegistry = nullptr,
        const FAetherAssetRegistry* AssetRegistry = nullptr,
        const FAetherItemRegistry* ItemRegistry = nullptr,
        const FAetherWorldActorRegistry* WorldActorRegistry = nullptr,
        const FAetherLootRewardRegistry* LootRewardRegistry = nullptr) const;

    void GetDefinitionIDs(TArray<FString>& OutIDs) const;
    int32 NumQuests() const { return Quests.Num(); }
    int32 NumDialogues() const { return Dialogues.Num(); }
    int32 NumWorldEvents() const { return WorldEvents.Num(); }
    int32 NumWorldContents() const { return WorldContents.Num(); }
    void Reset();

private:
    TMap<FString, FAetherDataQuestDefinition> Quests;
    TMap<FString, FAetherDialogueDefinition> Dialogues;
    TMap<FString, FAetherWorldEventDefinition> WorldEvents;
    TMap<FString, FAetherWorldContentDefinition> WorldContents;
    bool IsIDAvailable(const FString& ID) const;
    void AddIssue(TArray<FAetherQuestDialogueEventValidationIssue>& OutIssues, const FString& ID, const FString& Code, const FString& Message) const;
};
