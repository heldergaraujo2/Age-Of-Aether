#pragma once
#include "CoreMinimal.h"
#include "AetherQuestDialogueEventTypes.generated.h"

UENUM(BlueprintType)
enum class EAetherQuestObjectiveType : uint8
{
    Kill,
    Collect,
    Interact,
    Talk,
    ReachArea,
    Escort,
    Craft,
    Timed,
    WorldEvent
};

UENUM(BlueprintType)
enum class EAetherQuestConditionType : uint8
{
    QuestCompleted,
    QuestActive,
    LevelAtLeast,
    LevelAtMost,
    ItemOwned,
    ReputationAtLeast,
    WorldTag,
    EventCompleted,
    ContentUnlocked
};

UENUM(BlueprintType)
enum class EAetherDialogueNodeType : uint8
{
    Line,
    Choice,
    Condition,
    End
};

UENUM(BlueprintType)
enum class EAetherDialogueOutcomeType : uint8
{
    None,
    AcceptQuest,
    CompleteQuest,
    StartEvent,
    GrantReward,
    SetWorldTag,
    ClearWorldTag
};

UENUM(BlueprintType)
enum class EAetherWorldEventTriggerType : uint8
{
    Manual,
    Schedule,
    Condition,
    PreviousEvent
};

USTRUCT(BlueprintType)
struct FAetherQuestCondition
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadOnly) EAetherQuestConditionType Type = EAetherQuestConditionType::LevelAtLeast;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) FString ReferenceID;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) int32 Value = 0;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) FString RequiredTag;
    bool IsStructurallyValid(FString& OutError) const;
};

USTRUCT(BlueprintType)
struct FAetherQuestObjectiveDefinition
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadOnly) FString ObjectiveID;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) EAetherQuestObjectiveType Type = EAetherQuestObjectiveType::Kill;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) FString TargetID;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) int32 RequiredCount = 1;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) int32 MinimumLevel = 0;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) int32 MaximumLevel = 0;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) FString AreaTag;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) FString EventID;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) double TimeLimitSeconds = 0.0;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) bool bOptional = false;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) TArray<FAetherQuestCondition> Conditions;
    bool IsStructurallyValid(FString& OutError) const;
};

USTRUCT(BlueprintType)
struct FAetherQuestDefinition
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadOnly) FString DefinitionID;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) FString DisplayName;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) FString Description;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) int32 RecommendedLevel = 0;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) int32 MinimumLevel = 0;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) int32 MaximumLevel = 0;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) FString StartNPCID;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) FString TurnInNPCID;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) TArray<FString> PrerequisiteQuestIDs;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) TArray<FAetherQuestCondition> Prerequisites;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) TArray<FAetherQuestObjectiveDefinition> Objectives;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) TArray<FString> RewardIDs;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) TArray<FString> NextQuestIDs;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) TArray<FString> Tags;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) bool bRepeatable = false;
    bool IsStructurallyValid(FString& OutError) const;
};

USTRUCT(BlueprintType)
struct FAetherDialogueChoice
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadOnly) FString ChoiceID;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) FText Text;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) FString TargetNodeID;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) TArray<FAetherQuestCondition> Conditions;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) TArray<EAetherDialogueOutcomeType> Outcomes;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) TArray<FString> OutcomeIDs;
    bool IsStructurallyValid(FString& OutError) const;
};

USTRUCT(BlueprintType)
struct FAetherDialogueNodeDefinition
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadOnly) FString NodeID;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) EAetherDialogueNodeType Type = EAetherDialogueNodeType::Line;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) FString SpeakerNPCID;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) FText Text;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) FString NextNodeID;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) TArray<FAetherDialogueChoice> Choices;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) TArray<FAetherQuestCondition> Conditions;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) TArray<EAetherDialogueOutcomeType> Outcomes;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) TArray<FString> OutcomeIDs;
    bool IsStructurallyValid(FString& OutError) const;
};

USTRUCT(BlueprintType)
struct FAetherDialogueDefinition
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadOnly) FString DefinitionID;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) FString DisplayName;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) FString LocalizationNamespace;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) FString StartNodeID;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) TArray<FAetherDialogueNodeDefinition> Nodes;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) TArray<FString> Tags;
    bool IsStructurallyValid(FString& OutError) const;
};

USTRUCT(BlueprintType)
struct FAetherWorldEventDefinition
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadOnly) FString DefinitionID;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) FString DisplayName;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) FString Description;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) EAetherWorldEventTriggerType TriggerType = EAetherWorldEventTriggerType::Manual;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) double StartDelaySeconds = 0.0;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) double DurationSeconds = 0.0;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) double CooldownSeconds = 0.0;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) int32 MaxParticipants = 0;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) FString ZoneTag;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) FString PreviousEventID;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) TArray<FAetherQuestCondition> Conditions;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) TArray<FString> ParticipatingWorldActorIDs;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) TArray<FString> ObjectiveIDs;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) TArray<FString> RewardIDs;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) TArray<FString> AnnouncementIDs;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) TArray<FString> Tags;
    bool IsStructurallyValid(FString& OutError) const;
};

USTRUCT(BlueprintType)
struct FAetherWorldContentDefinition
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadOnly) FString DefinitionID;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) FString DisplayName;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) FString ZoneTag;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) FString InteractionTag;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) TArray<FString> WorldActorIDs;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) TArray<FString> QuestIDs;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) TArray<FString> DialogueIDs;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) TArray<FString> EventIDs;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) TArray<FString> SpawnGroupIDs;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) bool bEnabled = true;
    bool IsStructurallyValid(FString& OutError) const;
};

struct FAetherQuestDialogueEventValidationIssue
{
    FString DefinitionID;
    FString Code;
    FString Message;
};
