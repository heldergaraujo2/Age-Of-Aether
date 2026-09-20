#pragma once

#include "CoreMinimal.h"
#include "Characters/AetherCharacterTypes.h"
#include "Skills/AetherSkillTypes.generated.h"

UENUM(BlueprintType)
enum class EAetherSkillTargetMode : uint8
{
    Self,
    SingleTarget
};

UENUM(BlueprintType)
enum class EAetherSkillEffectType : uint8
{
    Damage,
    Heal,
    Buff,
    Debuff
};

UENUM(BlueprintType)
enum class EAetherSkillResultCode : uint8
{
    Accepted,
    InvalidRequest,
    NotAuthenticated,
    SkillNotFound,
    NotOwned,
    InvalidTarget,
    AttackerDead,
    TargetDead,
    OutOfRange,
    Cooldown,
    InvalidDefinition,
    ResourceInsufficient
};

USTRUCT(BlueprintType)
struct FAetherSkillEffect
{
    GENERATED_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Skill")
    EAetherSkillEffectType Type = EAetherSkillEffectType::Damage;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Skill")
    float Magnitude = 0.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Skill")
    float DurationSeconds = 0.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Skill")
    float TickIntervalSeconds = 0.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Skill")
    FString EffectID;
};

USTRUCT(BlueprintType)
struct FAetherSkillDefinition
{
    GENERATED_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Skill")
    FString SkillID;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Skill")
    FString DisplayName;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Skill")
    FString ClassID;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Skill")
    FString EvolutionID;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Skill")
    EAetherSkillTargetMode TargetMode = EAetherSkillTargetMode::SingleTarget;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Skill")
    float CooldownSeconds = 1.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Skill")
    float Range = 300.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Skill")
    float ResourceCost = 0.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Skill")
    TArray<FAetherSkillEffect> Effects;

    bool IsValid() const;
};

USTRUCT(BlueprintType)
struct FAetherSkillRuntimeStatus
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    FString EffectID;

    UPROPERTY(BlueprintReadOnly)
    EAetherSkillEffectType Type = EAetherSkillEffectType::Buff;

    UPROPERTY(BlueprintReadOnly)
    float Magnitude = 0.0f;

    UPROPERTY(BlueprintReadOnly)
    float RemainingSeconds = 0.0f;
};

USTRUCT(BlueprintType)
struct FAetherSkillResult
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    uint32 RequestId = 0;

    UPROPERTY(BlueprintReadOnly)
    EAetherSkillResultCode Result = EAetherSkillResultCode::InvalidRequest;

    UPROPERTY(BlueprintReadOnly)
    FString SkillID;

    UPROPERTY(BlueprintReadOnly)
    FAetherCharacterId TargetCharacterId;

    UPROPERTY(BlueprintReadOnly)
    float PrimaryMagnitude = 0.0f;

    UPROPERTY(BlueprintReadOnly)
    TArray<FAetherSkillRuntimeStatus> AppliedStatuses;
};
