#include "Data/AetherQuestDialogueEventTypes.h"
#include "Math/UnrealMathUtility.h"

namespace
{
bool RequiredID(const FString& Value) { return !Value.TrimStartAndEnd().IsEmpty(); }
bool ValidLevelRange(int32 Min, int32 Max) { return Min >= 0 && Max >= 0 && (Max == 0 || Min <= Max); }
bool ValidFiniteNonNegative(double Value) { return FMath::IsFinite(Value) && Value >= 0.0; }
bool ValidateConditions(const TArray<FAetherQuestCondition>& Conditions, FString& Error)
{
    for (const auto& C : Conditions)
    {
        if (C.Type == EAetherQuestConditionType::LevelAtLeast || C.Type == EAetherQuestConditionType::LevelAtMost)
        {
            if (C.Value < 0) { Error = TEXT("Condition level value cannot be negative."); return false; }
        }
        if ((C.Type == EAetherQuestConditionType::QuestCompleted || C.Type == EAetherQuestConditionType::QuestActive ||
             C.Type == EAetherQuestConditionType::ItemOwned || C.Type == EAetherQuestConditionType::EventCompleted ||
             C.Type == EAetherQuestConditionType::ContentUnlocked) && !RequiredID(C.ReferenceID))
        {
            Error = TEXT("ReferenceID is required for this condition."); return false;
        }
        if (C.Type == EAetherQuestConditionType::WorldTag && !RequiredID(C.RequiredTag))
        {
            Error = TEXT("RequiredTag is required for a WorldTag condition."); return false;
        }
        if (C.Type == EAetherQuestConditionType::ReputationAtLeast && C.Value < 0)
        {
            Error = TEXT("Reputation requirement cannot be negative."); return false;
        }
    }
    return true;
}
}

bool FAetherQuestCondition::IsStructurallyValid(FString& OutError) const
{
    OutError.Reset();
    TArray<FAetherQuestCondition> Conditions;\n    Conditions.Add(*this);\n    return ValidateConditions(Conditions, OutError);
}

bool FAetherQuestObjectiveDefinition::IsStructurallyValid(FString& OutError) const
{
    OutError.Reset();
    if (!RequiredID(ObjectiveID)) { OutError = TEXT("ObjectiveID is required."); return false; }
    if (RequiredCount <= 0) { OutError = TEXT("RequiredCount must be positive."); return false; }
    if (!ValidLevelRange(MinimumLevel, MaximumLevel)) { OutError = TEXT("Invalid objective level range."); return false; }
    if (!ValidFiniteNonNegative(TimeLimitSeconds)) { OutError = TEXT("TimeLimitSeconds must be finite and non-negative."); return false; }
    if ((Type == EAetherQuestObjectiveType::Kill || Type == EAetherQuestObjectiveType::Collect ||
         Type == EAetherQuestObjectiveType::Interact || Type == EAetherQuestObjectiveType::Talk ||
         Type == EAetherQuestObjectiveType::Craft) && !RequiredID(TargetID))
    { OutError = TEXT("TargetID is required for this objective type."); return false; }
    if (Type == EAetherQuestObjectiveType::ReachArea && !RequiredID(AreaTag))
    { OutError = TEXT("AreaTag is required for ReachArea."); return false; }
    if (Type == EAetherQuestObjectiveType::WorldEvent && !RequiredID(EventID))
    { OutError = TEXT("EventID is required for WorldEvent."); return false; }
    if (!ValidateConditions(Conditions, OutError)) return false;
    return true;
}

bool FAetherQuestDefinition::IsStructurallyValid(FString& OutError) const
{
    OutError.Reset();
    if (!RequiredID(DefinitionID) || !RequiredID(DisplayName)) { OutError = TEXT("Quest ID and DisplayName are required."); return false; }
    if (!ValidLevelRange(MinimumLevel, MaximumLevel) || RecommendedLevel < 0) { OutError = TEXT("Invalid quest level range."); return false; }
    if (Objectives.Num() == 0) { OutError = TEXT("Quest requires at least one objective."); return false; }
    TSet<FString> IDs;
    for (const auto& O : Objectives)
    {
        if (!O.IsStructurallyValid(OutError) || IDs.Contains(O.ObjectiveID.TrimStartAndEnd()))
        { if (OutError.IsEmpty()) OutError = TEXT("Duplicate objective ID."); return false; }
        IDs.Add(O.ObjectiveID.TrimStartAndEnd());
    }
    if (!ValidateConditions(Prerequisites, OutError)) return false;
    return true;
}

bool FAetherDialogueChoice::IsStructurallyValid(FString& OutError) const
{
    OutError.Reset();
    if (!RequiredID(ChoiceID) || !RequiredID(TargetNodeID)) { OutError = TEXT("ChoiceID and TargetNodeID are required."); return false; }
    if (!ValidateConditions(Conditions, OutError)) return false;
    if (Outcomes.Num() != OutcomeIDs.Num()) { OutError = TEXT("Outcomes and OutcomeIDs must have equal lengths."); return false; }
    return true;
}

bool FAetherDialogueNodeDefinition::IsStructurallyValid(FString& OutError) const
{
    OutError.Reset();
    if (!RequiredID(NodeID)) { OutError = TEXT("NodeID is required."); return false; }
    if (!ValidateConditions(Conditions, OutError)) return false;
    if (Outcomes.Num() != OutcomeIDs.Num()) { OutError = TEXT("Outcomes and OutcomeIDs must have equal lengths."); return false; }
    if (Type == EAetherDialogueNodeType::Line && Text.IsEmpty()) { OutError = TEXT("Line node requires text."); return false; }
    if (Type == EAetherDialogueNodeType::Choice && Choices.Num() == 0) { OutError = TEXT("Choice node requires choices."); return false; }
    if (Type == EAetherDialogueNodeType::Line && NextNodeID.IsEmpty()) { OutError = TEXT("Line node requires NextNodeID."); return false; }
    for (const auto& Choice : Choices) if (!Choice.IsStructurallyValid(OutError)) return false;
    return true;
}

bool FAetherDialogueDefinition::IsStructurallyValid(FString& OutError) const
{
    OutError.Reset();
    if (!RequiredID(DefinitionID) || !RequiredID(DisplayName) || !RequiredID(StartNodeID)) { OutError = TEXT("Dialogue ID, DisplayName and StartNodeID are required."); return false; }
    if (Nodes.Num() == 0) { OutError = TEXT("Dialogue requires at least one node."); return false; }
    TSet<FString> IDs;
    for (const auto& Node : Nodes)
    {
        if (!Node.IsStructurallyValid(OutError) || IDs.Contains(Node.NodeID.TrimStartAndEnd()))
        { if (OutError.IsEmpty()) OutError = TEXT("Duplicate dialogue node ID."); return false; }
        IDs.Add(Node.NodeID.TrimStartAndEnd());
    }
    if (!IDs.Contains(StartNodeID.TrimStartAndEnd())) { OutError = TEXT("StartNodeID does not exist."); return false; }
    for (const auto& Node : Nodes)
    {
        if (!Node.NextNodeID.IsEmpty() && !IDs.Contains(Node.NextNodeID.TrimStartAndEnd()))
        { OutError = TEXT("Dialogue node references a missing NextNodeID."); return false; }
        for (const auto& Choice : Node.Choices)
            if (!IDs.Contains(Choice.TargetNodeID.TrimStartAndEnd()))
            { OutError = TEXT("Dialogue choice references a missing target node."); return false; }
    }
    return true;
}

bool FAetherWorldEventDefinition::IsStructurallyValid(FString& OutError) const
{
    OutError.Reset();
    if (!RequiredID(DefinitionID) || !RequiredID(DisplayName)) { OutError = TEXT("Event ID and DisplayName are required."); return false; }
    if (!ValidFiniteNonNegative(StartDelaySeconds) || !ValidFiniteNonNegative(DurationSeconds) || !ValidFiniteNonNegative(CooldownSeconds))
    { OutError = TEXT("Event timing values must be finite and non-negative."); return false; }
    if (DurationSeconds <= 0.0) { OutError = TEXT("Event DurationSeconds must be positive."); return false; }
    if (MaxParticipants < 0) { OutError = TEXT("MaxParticipants cannot be negative."); return false; }
    if (TriggerType == EAetherWorldEventTriggerType::PreviousEvent && !RequiredID(PreviousEventID))
    { OutError = TEXT("PreviousEventID is required for PreviousEvent trigger."); return false; }
    if (!ValidateConditions(Conditions, OutError)) return false;
    return true;
}

bool FAetherWorldContentDefinition::IsStructurallyValid(FString& OutError) const
{
    OutError.Reset();
    if (!RequiredID(DefinitionID) || !RequiredID(DisplayName)) { OutError = TEXT("World content ID and DisplayName are required."); return false; }
    if (ZoneTag.IsEmpty()) { OutError = TEXT("ZoneTag is required."); return false; }
    return true;
}
