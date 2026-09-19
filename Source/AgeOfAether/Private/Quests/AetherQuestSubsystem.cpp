#include "Quests/AetherQuestSubsystem.h"

bool UAetherQuestSubsystem::RegisterDefinition(const FAetherQuestDefinition& Definition)
{
    return QuestService.RegisterDefinition(Definition);
}

bool UAetherQuestSubsystem::FindDefinition(const FAetherQuestId& QuestId, FAetherQuestDefinition& OutDefinition) const
{
    return QuestService.FindDefinition(QuestId, OutDefinition);
}

bool UAetherQuestSubsystem::GetQuestState(const FAetherCharacterId& CharacterId, const FAetherQuestId& QuestId, FAetherQuestState& OutState) const
{
    return QuestService.GetQuestState(CharacterId, QuestId, OutState);
}

bool UAetherQuestSubsystem::GetQuestStates(const FAetherCharacterId& CharacterId, TArray<FAetherQuestState>& OutStates) const
{
    return QuestService.GetQuestStates(CharacterId, OutStates);
}

bool UAetherQuestSubsystem::AcceptQuest(const FAetherCharacterRecord& Character, const FAetherQuestId& QuestId, FAetherQuestOperation& OutOperation)
{
    return QuestService.AcceptQuest(Character, QuestId, OutOperation);
}

bool UAetherQuestSubsystem::AbandonQuest(const FAetherCharacterRecord& Character, const FAetherQuestId& QuestId, FAetherQuestOperation& OutOperation)
{
    return QuestService.AbandonQuest(Character, QuestId, OutOperation);
}

bool UAetherQuestSubsystem::CompleteQuest(const FAetherCharacterRecord& Character, const FAetherQuestId& QuestId, FAetherQuestOperation& OutOperation)
{
    return QuestService.CompleteQuest(Character, QuestId, OutOperation);
}

bool UAetherQuestSubsystem::AdvanceObjective(
    const FAetherCharacterRecord& Character,
    const FAetherQuestId& QuestId,
    const FString& ObjectiveId,
    int32 Amount,
    FAetherQuestOperation& OutOperation)
{
    return QuestService.AdvanceObjective(Character, QuestId, ObjectiveId, Amount, OutOperation);
}

bool UAetherQuestSubsystem::AdvanceObjectiveByTarget(
    const FAetherCharacterRecord& Character,
    EAetherQuestObjectiveType ObjectiveType,
    const FString& TargetId,
    int32 Amount,
    FAetherQuestOperation& OutOperation)
{
    return QuestService.AdvanceObjectiveByTarget(Character, ObjectiveType, TargetId, Amount, OutOperation);
}

void UAetherQuestSubsystem::ClearCharacterState(const FAetherCharacterId& CharacterId)
{
    QuestService.ClearCharacterState(CharacterId);
}
