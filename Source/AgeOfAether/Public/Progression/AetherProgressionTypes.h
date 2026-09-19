#pragma once
#include "CoreMinimal.h"
#include "Characters/AetherCharacterTypes.h"
#include "Progression/AetherProgressionTypes.generated.h"

UENUM(BlueprintType)
enum class EAetherProgressionResult : uint8
{
    Accepted,
    InvalidRequest,
    NotOwned,
    CharacterNotFound,
    CharacterDisabled,
    CharacterDeleted,
    MaxLevelReached,
    InsufficientStatPoints,
    InvalidStat,
    StatCapReached,
    InvalidExperience
};

UENUM(BlueprintType)
enum class EAetherCharacterStat : uint8
{
    Strength,
    Agility,
    Vitality,
    Energy,
    Command
};

USTRUCT(BlueprintType)
struct FAetherProgressionConfig
{
    GENERATED_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    int32 MaxLevel = 4000;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    int64 BaseExperienceToLevel = 100;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    float ExperienceGrowthPerLevel = 1.15f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    int32 StatPointsPerLevel = 5;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    int32 MaxStatValue = 30000;
};

USTRUCT(BlueprintType)
struct FAetherProgressionResult
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    EAetherProgressionResult Result = EAetherProgressionResult::InvalidRequest;

    UPROPERTY(BlueprintReadOnly)
    int32 PreviousLevel = 1;

    UPROPERTY(BlueprintReadOnly)
    int32 NewLevel = 1;

    UPROPERTY(BlueprintReadOnly)
    int64 PreviousExperience = 0;

    UPROPERTY(BlueprintReadOnly)
    int64 NewExperience = 0;

    UPROPERTY(BlueprintReadOnly)
    int32 LevelsGained = 0;

    UPROPERTY(BlueprintReadOnly)
    int32 StatPointsGranted = 0;

    UPROPERTY(BlueprintReadOnly)
    int32 UnspentStatPoints = 0;
};
