#pragma once

#include "CoreMinimal.h"
#include "World/AetherWorldTypes.h"
#include "Creatures/AetherCreatureTypes.generated.h"

UENUM(BlueprintType)
enum class EAetherCreatureRole : uint8
{
    Monster,
    NPC,
    Boss
};

UENUM(BlueprintType)
enum class EAetherCreatureAIProfile : uint8
{
    Passive,
    Guard,
    Patrol,
    Aggressive,
    Boss
};

UENUM(BlueprintType)
enum class EAetherCreatureResultCode : uint8
{
    Accepted,
    InvalidRequest,
    DefinitionNotFound,
    InvalidDefinition,
    SpawnLimitReached,
    SpawnFailed
};

USTRUCT(BlueprintType)
struct FAetherCreatureDefinition
{
    GENERATED_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Creature")
    FString CreatureID;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Creature")
    FString DisplayName;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Creature")
    EAetherCreatureRole Role = EAetherCreatureRole::Monster;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Creature")
    EAetherCreatureAIProfile AIProfile = EAetherCreatureAIProfile::Passive;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Creature")
    float MaxHealth = 100.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Creature")
    float Defense = 0.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Creature")
    float AttackMin = 1.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Creature")
    float AttackMax = 2.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Creature")
    float MoveSpeed = 300.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Creature")
    int32 Level = 1;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Creature")
    FString LootTableID;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Creature")
    FString QuestActorID;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Creature")
    TSoftObjectPtr<USkeletalMesh> SkeletalMesh;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Creature")
    TSoftClassPtr<UAnimInstance> AnimationClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Creature")
    TSoftObjectPtr<UAnimMontage> AttackMontage;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Creature")
    TSoftObjectPtr<UObject> IdleVFX;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Creature")
    TSoftObjectPtr<UObject> SpawnVFX;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Creature")
    TSoftObjectPtr<USoundBase> SpawnSFX;

    bool IsValid() const;
};

USTRUCT(BlueprintType)
struct FAetherCreatureSpawnDefinition
{
    GENERATED_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Spawn")
    FString SpawnID;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Spawn")
    FString CreatureID;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Spawn")
    FAetherWorldZoneId ZoneID;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Spawn")
    FVector Location = FVector::ZeroVector;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Spawn")
    FRotator Rotation = FRotator::ZeroRotator;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Spawn")
    int32 MaxAlive = 1;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Spawn")
    bool bEnabled = true;

    bool IsValid() const;
};

USTRUCT(BlueprintType)
struct FAetherCreatureRuntimeState
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    FString CreatureID;

    UPROPERTY(BlueprintReadOnly)
    EAetherCreatureRole Role = EAetherCreatureRole::Monster;

    UPROPERTY(BlueprintReadOnly)
    float CurrentHealth = 0.0f;

    UPROPERTY(BlueprintReadOnly)
    float MaxHealth = 0.0f;

    UPROPERTY(BlueprintReadOnly)
    int32 Level = 1;

    UPROPERTY(BlueprintReadOnly)
    bool bAlive = true;
};
