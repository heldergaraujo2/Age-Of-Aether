#pragma once

#include "CoreMinimal.h"
#include "Characters/AetherCharacterTypes.h"
#include "Combat/AetherCombatTypes.generated.h"

UENUM(BlueprintType)
enum class EAetherCombatResultCode : uint8
{
    Accepted,
    InvalidRequest,
    NotAuthenticated,
    AttackerNotFound,
    TargetNotFound,
    NotOwned,
    AttackerDead,
    TargetDead,
    OutOfRange,
    Cooldown,
    InvalidState,
    Missed,
    DamageRejected
};

UENUM(BlueprintType)
enum class EAetherCombatOutcome : uint8
{
    None,
    Hit,
    CriticalHit,
    Miss,
    Defeated
};

USTRUCT(BlueprintType)
struct FAetherCombatConfig
{
    GENERATED_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat")
    float MaxAttackRange = 250.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat")
    float AttackCooldownSeconds = 1.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat")
    float Accuracy = 1.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat")
    float CriticalChance = 0.10f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat")
    float CriticalMultiplier = 1.50f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat")
    float MinimumDamage = 1.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat")
    float DefenseMitigationScale = 100.0f;
};

USTRUCT(BlueprintType)
struct FAetherCombatResult
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    uint32 RequestId = 0;

    UPROPERTY(BlueprintReadOnly)
    EAetherCombatResultCode Result = EAetherCombatResultCode::InvalidRequest;

    UPROPERTY(BlueprintReadOnly)
    EAetherCombatOutcome Outcome = EAetherCombatOutcome::None;

    UPROPERTY(BlueprintReadOnly)
    bool bCritical = false;

    UPROPERTY(BlueprintReadOnly)
    float Distance = 0.0f;

    UPROPERTY(BlueprintReadOnly)
    float RawDamage = 0.0f;

    UPROPERTY(BlueprintReadOnly)
    float DamageApplied = 0.0f;

    UPROPERTY(BlueprintReadOnly)
    float TargetHealthBefore = 0.0f;

    UPROPERTY(BlueprintReadOnly)
    float TargetHealthAfter = 0.0f;

    UPROPERTY(BlueprintReadOnly)
    float TargetMaxHealth = 0.0f;

    UPROPERTY(BlueprintReadOnly)
    float CooldownRemaining = 0.0f;
};
