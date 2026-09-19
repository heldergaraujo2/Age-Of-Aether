#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Quests/AetherQuestService.h"
#include "AetherQuestSubsystem.generated.h"

UCLASS()
class AGEOFAETHER_API UAetherQuestSubsystem : public UGameInstanceSubsystem
{
    GENERATED_BODY()

public:
    bool RegisterDefinition(const FAetherQuestDefinition& Definition);
    bool FindDefinition(const FAetherQuestId& QuestId, FAetherQuestDefinition& OutDefinition) const;
    bool GetQuestState(const FAetherCharacterId& CharacterId, const FAetherQuestId& QuestId, FAetherQuestState& OutState) const;
    bool GetQuestStates(const FAetherCharacterId& CharacterId, TArray<FAetherQuestState>& OutStates) const;

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

    FAetherQuestService& GetQuestService() { return QuestService; }
    const FAetherQuestService& GetQuestService() const { return QuestService; }

private:
    FAetherQuestService QuestService;
};
