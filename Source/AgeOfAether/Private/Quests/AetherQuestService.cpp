#include "Quests/AetherQuestService.h"

FAetherQuestService::FAetherQuestService()
{
    SetConfig(Config);
}

const FAetherQuestConfig& FAetherQuestService::GetConfig() const
{
    return Config;
}

bool FAetherQuestService::SetConfig(const FAetherQuestConfig& NewConfig)
{
    if (!IsConfigValid(NewConfig))
    {
        return false;
    }
    Config = NewConfig;
    return true;
}

bool FAetherQuestService::IsConfigValid(const FAetherQuestConfig& NewConfig)
{
    return NewConfig.MaxDefinitions > 0 && NewConfig.MaxActiveQuestsPerCharacter > 0;
}

bool FAetherQuestService::RegisterDefinition(const FAetherQuestDefinition& Definition)
{
    if (!Definition.IsValid() || Definitions.Num() >= Config.MaxDefinitions)
    {
        return false;
    }

    if (Definitions.Contains(Definition.QuestId))
    {
        return false;
    }

    Definitions.Add(Definition.QuestId, Definition);
    return true;
}

bool FAetherQuestService::FindDefinition(const FAetherQuestId& QuestId, FAetherQuestDefinition& OutDefinition) const
{
    const FAetherQuestDefinition* Found = Definitions.Find(QuestId);
    if (!Found)
    {
        return false;
    }

    OutDefinition = *Found;
    return true;
}

void FAetherQuestService::ClearDefinitions()
{
    Definitions.Empty();
}

bool FAetherQuestService::IsCharacterUsable(const FAetherCharacterRecord& Character)
{
    return Character.CharacterId.IsValid()
        && Character.AccountId.IsValid()
        && Character.Status == EAetherCharacterStatus::Active
        && Character.Status != EAetherCharacterStatus::Disabled
        && Character.Status != EAetherCharacterStatus::Deleted;
}

bool FAetherQuestService::BuildAvailableState(const FAetherQuestDefinition& Definition, FAetherQuestState& OutState) const
{
    OutState = FAetherQuestState();
    OutState.QuestId = Definition.QuestId;
    OutState.Status = EAetherQuestStatus::Available;

    for (const FAetherQuestObjectiveDefinition& Objective : Definition.Objectives)
    {
        FAetherQuestObjectiveProgress Progress;
        Progress.ObjectiveId = Objective.ObjectiveId;
        Progress.RequiredCount = Objective.RequiredCount;
        Progress.bOptional = Objective.bOptional;
        OutState.Objectives.Add(Progress);
    }
    return true;
}

bool FAetherQuestService::GetQuestState(const FAetherCharacterId& CharacterId, const FAetherQuestId& QuestId, FAetherQuestState& OutState) const
{
    const TMap<FAetherQuestId, FAetherQuestState>* CharacterStates = StatesByCharacter.Find(CharacterId);
    if (!CharacterStates)
    {
        return false;
    }

    const FAetherQuestState* Found = CharacterStates->Find(QuestId);
    if (!Found)
    {
        return false;
    }

    OutState = *Found;
    return true;
}

bool FAetherQuestService::GetQuestStates(const FAetherCharacterId& CharacterId, TArray<FAetherQuestState>& OutStates) const
{
    OutStates.Empty();
    const TMap<FAetherQuestId, FAetherQuestState>* CharacterStates = StatesByCharacter.Find(CharacterId);
    if (!CharacterStates)
    {
        return true;
    }

    for (const TPair<FAetherQuestId, FAetherQuestState>& Pair : *CharacterStates)
    {
        OutStates.Add(Pair.Value);
    }

    OutStates.Sort([](const FAetherQuestState& A, const FAetherQuestState& B)
    {
        return A.QuestId.Value < B.QuestId.Value;
    });
    return true;
}

bool FAetherQuestService::ArePrerequisitesMet(const FAetherCharacterId& CharacterId, const FAetherQuestDefinition& Definition) const
{
    const TMap<FAetherQuestId, FAetherQuestState>* CharacterStates = StatesByCharacter.Find(CharacterId);

    for (const FAetherQuestId& PrerequisiteId : Definition.PrerequisiteQuestIds)
    {
        if (!CharacterStates)
        {
            return false;
        }

        const FAetherQuestState* State = CharacterStates->Find(PrerequisiteId);
        if (!State || State->Status != EAetherQuestStatus::Completed || !State->bRewardGranted)
        {
            return false;
        }
    }

    return true;
}

bool FAetherQuestService::CanRestoreQuestStates(const FAetherCharacterId& CharacterId, const TArray<FAetherQuestState>& States) const
{
    if (!CharacterId.IsValid() || States.Num() > Config.MaxDefinitions)
    {
        return false;
    }

    TSet<FAetherQuestId> QuestIds;
    for (const FAetherQuestState& State : States)
    {
        const FAetherQuestDefinition* Definition = Definitions.Find(State.QuestId);
        if (!Definition || !State.QuestId.IsValid() || QuestIds.Contains(State.QuestId))
        {
            return false;
        }

        TSet<FString> ObjectiveIds;
        for (const FAetherQuestObjectiveProgress& Objective : State.Objectives)
        {
            if (Objective.ObjectiveId.IsEmpty() || Objective.CurrentCount < 0 || Objective.RequiredCount < 0 || ObjectiveIds.Contains(Objective.ObjectiveId))
            {
                return false;
            }
            ObjectiveIds.Add(Objective.ObjectiveId);
        }
        QuestIds.Add(State.QuestId);
    }
    return true;
}

bool FAetherQuestService::RestoreQuestStates(const FAetherCharacterId& CharacterId, const TArray<FAetherQuestState>& States)
{
    if (!CharacterId.IsValid() || States.Num() > Config.MaxDefinitions)
    {
        return false;
    }

    TMap<FAetherQuestId, FAetherQuestState> Restored;
    for (const FAetherQuestState& State : States)
    {
        const FAetherQuestDefinition* Definition = Definitions.Find(State.QuestId);
        if (!Definition || State.QuestId.IsValid() == false || Restored.Contains(State.QuestId))
        {
            return false;
        }

        TSet<FString> ObjectiveIds;
        for (const FAetherQuestObjectiveProgress& Objective : State.Objectives)
        {
            if (Objective.ObjectiveId.IsEmpty() || Objective.CurrentCount < 0 || Objective.RequiredCount < 0 || ObjectiveIds.Contains(Objective.ObjectiveId))
            {
                return false;
            }
            ObjectiveIds.Add(Objective.ObjectiveId);
        }

        Restored.Add(State.QuestId, State);
    }

    StatesByCharacter.Add(CharacterId, MoveTemp(Restored));
    return true;
}

bool FAetherQuestService::AcceptQuest(const FAetherCharacterRecord& Character, const FAetherQuestId& QuestId, FAetherQuestOperation& OutOperation)
{
    OutOperation = FAetherQuestOperation();

    if (!IsCharacterUsable(Character))
    {
        OutOperation.Result = EAetherQuestOperationResult::CharacterInvalid;
        return false;
    }

    const FAetherQuestDefinition* Definition = Definitions.Find(QuestId);
    if (!Definition)
    {
        OutOperation.Result = EAetherQuestOperationResult::QuestNotFound;
        return false;
    }

    if (!Definition->bEnabled)
    {
        OutOperation.Result = EAetherQuestOperationResult::QuestDisabled;
        return false;
    }

    if (Character.Level < Definition->MinimumLevel || !ArePrerequisitesMet(Character.CharacterId, *Definition))
    {
        OutOperation.Result = EAetherQuestOperationResult::RequirementsNotMet;
        return false;
    }

    TMap<FAetherQuestId, FAetherQuestState>& CharacterStates = StatesByCharacter.FindOrAdd(Character.CharacterId);
    if (FAetherQuestState* Existing = CharacterStates.Find(QuestId))
    {
        if (Existing->Status == EAetherQuestStatus::Active)
        {
            OutOperation.Result = EAetherQuestOperationResult::AlreadyActive;
            OutOperation.State = *Existing;
            return false;
        }
        if (Existing->Status == EAetherQuestStatus::Completed)
        {
            OutOperation.Result = EAetherQuestOperationResult::AlreadyCompleted;
            OutOperation.State = *Existing;
            return false;
        }
    }

    int32 ActiveCount = 0;
    for (const TPair<FAetherQuestId, FAetherQuestState>& Pair : CharacterStates)
    {
        if (Pair.Value.Status == EAetherQuestStatus::Active)
        {
            ++ActiveCount;
        }
    }
    if (ActiveCount >= Config.MaxActiveQuestsPerCharacter)
    {
        OutOperation.Result = EAetherQuestOperationResult::CapacityReached;
        return false;
    }

    FAetherQuestState NewState;
    BuildAvailableState(*Definition, NewState);
    NewState.Status = EAetherQuestStatus::Active;
    CharacterStates.Add(QuestId, NewState);

    OutOperation.Result = EAetherQuestOperationResult::Accepted;
    OutOperation.State = NewState;
    return true;
}

bool FAetherQuestService::AbandonQuest(const FAetherCharacterRecord& Character, const FAetherQuestId& QuestId, FAetherQuestOperation& OutOperation)
{
    OutOperation = FAetherQuestOperation();

    if (!IsCharacterUsable(Character))
    {
        OutOperation.Result = EAetherQuestOperationResult::CharacterInvalid;
        return false;
    }

    TMap<FAetherQuestId, FAetherQuestState>* CharacterStates = StatesByCharacter.Find(Character.CharacterId);
    FAetherQuestState* State = CharacterStates ? CharacterStates->Find(QuestId) : nullptr;
    if (!State || State->Status != EAetherQuestStatus::Active)
    {
        OutOperation.Result = EAetherQuestOperationResult::NotActive;
        return false;
    }

    State->Status = EAetherQuestStatus::Abandoned;
    OutOperation.Result = EAetherQuestOperationResult::Accepted;
    OutOperation.State = *State;
    return true;
}

bool FAetherQuestService::AreRequiredObjectivesComplete(const FAetherQuestState& State) const
{
    for (const FAetherQuestObjectiveProgress& Objective : State.Objectives)
    {
        if (Objective.bOptional)
        {
            continue;
        }
        if (!Objective.bCompleted)
        {
            return false;
        }
    }
    return true;
}

bool FAetherQuestService::CompleteQuest(const FAetherCharacterRecord& Character, const FAetherQuestId& QuestId, FAetherQuestOperation& OutOperation)
{
    OutOperation = FAetherQuestOperation();

    if (!IsCharacterUsable(Character))
    {
        OutOperation.Result = EAetherQuestOperationResult::CharacterInvalid;
        return false;
    }

    TMap<FAetherQuestId, FAetherQuestState>* CharacterStates = StatesByCharacter.Find(Character.CharacterId);
    FAetherQuestState* State = CharacterStates ? CharacterStates->Find(QuestId) : nullptr;
    const FAetherQuestDefinition* Definition = Definitions.Find(QuestId);

    if (!State || !Definition)
    {
        OutOperation.Result = EAetherQuestOperationResult::QuestNotFound;
        return false;
    }

    if (State->Status == EAetherQuestStatus::Completed)
    {
        OutOperation.Result = EAetherQuestOperationResult::AlreadyCompleted;
        OutOperation.State = *State;
        return false;
    }

    if (State->Status != EAetherQuestStatus::Active)
    {
        OutOperation.Result = EAetherQuestOperationResult::NotActive;
        OutOperation.State = *State;
        return false;
    }

    if (!AreRequiredObjectivesComplete(*State))
    {
        OutOperation.Result = EAetherQuestOperationResult::ObjectiveIncomplete;
        OutOperation.State = *State;
        return false;
    }

    State->Status = EAetherQuestStatus::Completed;
    if (!State->bRewardGranted)
    {
        State->bRewardGranted = true;
        OutOperation.Reward = Definition->Reward;
    }

    OutOperation.Result = EAetherQuestOperationResult::Accepted;
    OutOperation.State = *State;
    return true;
}

bool FAetherQuestService::IsObjectiveTypeTargetMatch(const FAetherQuestObjectiveDefinition& Definition, EAetherQuestObjectiveType ObjectiveType, const FString& TargetId) const
{
    return Definition.Type == ObjectiveType && Definition.TargetId == TargetId;
}

bool FAetherQuestService::AdvanceObjective(
    const FAetherCharacterRecord& Character,
    const FAetherQuestId& QuestId,
    const FString& ObjectiveId,
    int32 Amount,
    FAetherQuestOperation& OutOperation)
{
    OutOperation = FAetherQuestOperation();

    if (!IsCharacterUsable(Character) || Amount <= 0)
    {
        OutOperation.Result = Amount <= 0 ? EAetherQuestOperationResult::InvalidRequest : EAetherQuestOperationResult::CharacterInvalid;
        return false;
    }

    TMap<FAetherQuestId, FAetherQuestState>* CharacterStates = StatesByCharacter.Find(Character.CharacterId);
    FAetherQuestState* State = CharacterStates ? CharacterStates->Find(QuestId) : nullptr;
    const FAetherQuestDefinition* Definition = Definitions.Find(QuestId);

    if (!State || !Definition)
    {
        OutOperation.Result = EAetherQuestOperationResult::QuestNotFound;
        return false;
    }

    if (State->Status != EAetherQuestStatus::Active)
    {
        OutOperation.Result = EAetherQuestOperationResult::NotActive;
        return false;
    }

    const FAetherQuestObjectiveDefinition* ObjectiveDefinition = Definition->Objectives.FindByPredicate(
        [&ObjectiveId](const FAetherQuestObjectiveDefinition& Objective)
        {
            return Objective.ObjectiveId == ObjectiveId;
        });

    FAetherQuestObjectiveProgress* Progress = State->Objectives.FindByPredicate(
        [&ObjectiveId](FAetherQuestObjectiveProgress& Objective)
        {
            return Objective.ObjectiveId == ObjectiveId;
        });

    if (!ObjectiveDefinition || !Progress)
    {
        OutOperation.Result = EAetherQuestOperationResult::InvalidRequest;
        return false;
    }

    if (Progress->bCompleted)
    {
        OutOperation.Result = EAetherQuestOperationResult::Accepted;
        OutOperation.State = *State;
        return true;
    }

    Progress->CurrentCount = FMath::Min(Progress->RequiredCount, Progress->CurrentCount + Amount);
    Progress->bCompleted = Progress->CurrentCount >= Progress->RequiredCount;

    OutOperation.Result = EAetherQuestOperationResult::Accepted;
    OutOperation.State = *State;
    return true;
}

bool FAetherQuestService::AdvanceObjectiveByTarget(
    const FAetherCharacterRecord& Character,
    EAetherQuestObjectiveType ObjectiveType,
    const FString& TargetId,
    int32 Amount,
    FAetherQuestOperation& OutOperation)
{
    OutOperation = FAetherQuestOperation();
    if (!IsCharacterUsable(Character) || Amount <= 0 || TargetId.IsEmpty())
    {
        OutOperation.Result = EAetherQuestOperationResult::InvalidRequest;
        return false;
    }

    TArray<FAetherQuestState> ActiveStates;
    const TMap<FAetherQuestId, FAetherQuestState>* CharacterStates = StatesByCharacter.Find(Character.CharacterId);
    if (!CharacterStates)
    {
        OutOperation.Result = EAetherQuestOperationResult::NotActive;
        return false;
    }

    bool bAdvanced = false;
    for (const TPair<FAetherQuestId, FAetherQuestState>& Pair : *CharacterStates)
    {
        if (Pair.Value.Status != EAetherQuestStatus::Active)
        {
            continue;
        }

        const FAetherQuestDefinition* Definition = Definitions.Find(Pair.Key);
        if (!Definition)
        {
            continue;
        }

        for (const FAetherQuestObjectiveDefinition& Objective : Definition->Objectives)
        {
            if (IsObjectiveTypeTargetMatch(Objective, ObjectiveType, TargetId))
            {
                FAetherQuestOperation LocalOperation;
                if (AdvanceObjective(Character, Pair.Key, Objective.ObjectiveId, Amount, LocalOperation))
                {
                    OutOperation = LocalOperation;
                    bAdvanced = true;
                }
            }
        }
    }

    return bAdvanced;
}

void FAetherQuestService::ClearCharacterState(const FAetherCharacterId& CharacterId)
{
    StatesByCharacter.Remove(CharacterId);
}

void FAetherQuestService::ClearAllState()
{
    StatesByCharacter.Empty();
}
