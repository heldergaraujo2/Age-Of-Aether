#pragma once

#include "CoreMinimal.h"
#include "Quests/AetherQuestTypes.h"

class FAetherQuestService
{
public:
    FAetherQuestService();

    const FAetherQuestConfig& GetConfig() const;
    bool SetConfig(const FAetherQuestConfig& NewConfig);

    bool RegisterDefinition(const FAetherQuestDefinition& Definition);
    bool FindDefinition(const FAetherQuestId& QuestId, FAetherQuestDefinition& OutDefinition) const;
    void ClearDefinitions();

    bool GetQuestState(const FAetherCharacterId& CharacterId, const FAetherQuestId& QuestId, FAetherQuestState& OutState) const;
    bool GetQuestStates(const FAetherCharacterId& CharacterId, TArray<FAetherQuestState>& OutStates) const;
    bool CanRestoreQuestStates(const FAetherCharacterId& CharacterId, const TArray<FAetherQuestState>& States) const;
    bool RestoreQuestStates(const FAetherCharacterId& CharacterId, const TArray<FAetherQuestState>& States);

    bool AcceptQuest(const FAetherCharacterRecord& Character, const FAetherQuestId& QuestId, FAetherQuestOperation& OutOperation);
    bool AbandonQuest(const FAetherCharacterRecord& Character, const FAetherQuestId& QuestId, FAetherQuestOperation& OutOperation);
    bool CompleteQuest(const FAetherCharacterRecord& Character, const FAetherQuestId& QuestId, FAetherQuestOperation& OutOperation);

    bool AdvanceObjective(
        const FAetherCharacterRecord& Character,
        const FAetherQuestId& QuestId,
        const FString& ObjectiveId,
        int32 Amount,
        FAetherQuestOperation& OutOperation);

    bool AdvanceObjectiveByTarget(
        const FAetherCharacterRecord& Character,
        EAetherQuestObjectiveType ObjectiveType,
        const FString& TargetId,
        int32 Amount,
        FAetherQuestOperation& OutOperation);

    void ClearCharacterState(const FAetherCharacterId& CharacterId);
    void ClearAllState();

private:
    static bool IsConfigValid(const FAetherQuestConfig& Config);
    static bool IsCharacterUsable(const FAetherCharacterRecord& Character);
    bool ArePrerequisitesMet(const FAetherCharacterId& CharacterId, const FAetherQuestDefinition& Definition) const;
    bool BuildAvailableState(const FAetherQuestDefinition& Definition, FAetherQuestState& OutState) const;
    bool AreRequiredObjectivesComplete(const FAetherQuestState& State) const;
    bool IsObjectiveTypeTargetMatch(const FAetherQuestObjectiveDefinition& Definition, EAetherQuestObjectiveType Type, const FString& TargetId) const;

    FAetherQuestConfig Config;
    TMap<FAetherQuestId, FAetherQuestDefinition> Definitions;
    TMap<FAetherCharacterId, TMap<FAetherQuestId, FAetherQuestState>> StatesByCharacter;
};
