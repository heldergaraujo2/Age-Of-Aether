#pragma once

#include "CoreMinimal.h"
#include "Characters/AetherCharacterTypes.h"
#include "World/AetherWorldTypes.h"
#include "Items/AetherItemTypes.h"
#include "AetherQuestTypes.generated.h"

UENUM(BlueprintType)
enum class EAetherQuestStatus : uint8
{
    Locked,
    Available,
    Active,
    Completed,
    Failed,
    Abandoned
};

UENUM(BlueprintType)
enum class EAetherQuestObjectiveType : uint8
{
    Kill,
    Collect,
    ReachZone,
    Interact,
    Talk,
    ReachLevel,
    UseItem
};

UENUM(BlueprintType)
enum class EAetherQuestOperationResult : uint8
{
    Accepted,
    InvalidRequest,
    QuestNotFound,
    QuestDisabled,
    NotAvailable,
    AlreadyActive,
    AlreadyCompleted,
    NotActive,
    RequirementsNotMet,
    ObjectiveIncomplete,
    CannotAbandon,
    RewardAlreadyGranted,
    CharacterInvalid,
    CharacterNotOwned,
    InvalidDefinition,
    CapacityReached
};

USTRUCT(BlueprintType)
struct FAetherQuestId
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FString Value;

    bool IsValid() const { return !Value.IsEmpty(); }
    static FAetherQuestId FromString(const FString& InValue)
    {
        FAetherQuestId Id;
        Id.Value = InValue.TrimStartAndEnd();
        return Id;
    }
    bool operator==(const FAetherQuestId& Other) const { return Value == Other.Value; }
};
FORCEINLINE uint32 GetTypeHash(const FAetherQuestId& Id) { return GetTypeHash(Id.Value); }

USTRUCT(BlueprintType)
struct FAetherQuestObjectiveDefinition
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FString ObjectiveId;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    EAetherQuestObjectiveType Type = EAetherQuestObjectiveType::Kill;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FString TargetId;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    int32 RequiredCount = 1;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    bool bOptional = false;

    bool IsValid() const
    {
        return !ObjectiveId.IsEmpty() && !TargetId.IsEmpty() && RequiredCount > 0;
    }
};

USTRUCT(BlueprintType)
struct FAetherQuestObjectiveProgress
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    FString ObjectiveId;

    UPROPERTY(BlueprintReadOnly)
    int32 CurrentCount = 0;

    UPROPERTY(BlueprintReadOnly)
    int32 RequiredCount = 0;

    UPROPERTY(BlueprintReadOnly)
    bool bOptional = false;

    UPROPERTY(BlueprintReadOnly)
    bool bCompleted = false;
};

USTRUCT(BlueprintType)
struct FAetherQuestReward
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    int32 Experience = 0;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TArray<FAetherItemDefinitionId> ItemDefinitionIds;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TArray<int32> ItemQuantities;
};

USTRUCT(BlueprintType)
struct FAetherQuestDefinition
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FAetherQuestId QuestId;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FString DisplayName;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FString Description;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    bool bEnabled = true;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    int32 MinimumLevel = 1;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TArray<FAetherQuestId> PrerequisiteQuestIds;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TArray<FAetherQuestObjectiveDefinition> Objectives;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FAetherQuestReward Reward;

    bool IsValid() const
    {
        if (!QuestId.IsValid() || DisplayName.IsEmpty() || MinimumLevel < 1 || Objectives.Num() == 0)
        {
            return false;
        }

        TSet<FString> ObjectiveIds;
        for (const FAetherQuestObjectiveDefinition& Objective : Objectives)
        {
            if (!Objective.IsValid() || ObjectiveIds.Contains(Objective.ObjectiveId))
            {
                return false;
            }
            ObjectiveIds.Add(Objective.ObjectiveId);
        }

        if (Reward.ItemQuantities.Num() > 0 && Reward.ItemQuantities.Num() != Reward.ItemDefinitionIds.Num())
        {
            return false;
        }

        for (const int32 Quantity : Reward.ItemQuantities)
        {
            if (Quantity <= 0)
            {
                return false;
            }
        }

        return true;
    }
};

USTRUCT(BlueprintType)
struct FAetherQuestState
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    FAetherQuestId QuestId;

    UPROPERTY(BlueprintReadOnly)
    EAetherQuestStatus Status = EAetherQuestStatus::Locked;

    UPROPERTY(BlueprintReadOnly)
    TArray<FAetherQuestObjectiveProgress> Objectives;

    UPROPERTY(BlueprintReadOnly)
    bool bRewardGranted = false;
};

USTRUCT(BlueprintType)
struct FAetherQuestConfig
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    int32 MaxDefinitions = 4096;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    int32 MaxActiveQuestsPerCharacter = 32;
};

USTRUCT(BlueprintType)
struct FAetherQuestOperation
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    EAetherQuestOperationResult Result = EAetherQuestOperationResult::InvalidRequest;

    UPROPERTY(BlueprintReadOnly)
    FAetherQuestState State;

    UPROPERTY(BlueprintReadOnly)
    FAetherQuestReward Reward;

    bool IsAccepted() const { return Result == EAetherQuestOperationResult::Accepted; }
};
