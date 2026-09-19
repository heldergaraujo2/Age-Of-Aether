#pragma once
#include "CoreMinimal.h"
#include "AetherWorldActorTypes.generated.h"

UENUM(BlueprintType)
enum class EAetherAIBehavior : uint8
{
    Passive,
    Defensive,
    Aggressive,
    Guard,
    FleeAtLowHealth,
    Boss
};

UENUM(BlueprintType)
enum class EAetherNPCType : uint8
{
    Citizen,
    Vendor,
    Blacksmith,
    QuestGiver,
    Trainer,
    Guard,
    Banker,
    Teleporter,
    Service
};

UENUM(BlueprintType)
enum class EAetherBossPhaseTrigger : uint8
{
    HealthPercent,
    TimerSeconds,
    AddCount,
    Event
};

USTRUCT(BlueprintType)
struct FAetherCombatStats
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadOnly) int32 Level = 1;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) int64 MaxHealth = 1;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) int64 MaxMana = 0;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) double MinDamage = 0.0;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) double MaxDamage = 0.0;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) double Defense = 0.0;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) double MagicDefense = 0.0;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) double AttackSpeed = 1.0;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) double MoveSpeed = 1.0;
};

USTRUCT(BlueprintType)
struct FAetherAIProfile
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadOnly) EAetherAIBehavior Behavior = EAetherAIBehavior::Passive;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) double AggroRange = 0.0;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) double LeashRange = 0.0;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) double AttackRange = 1.0;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) double FleeHealthPercent = 0.0;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) FString BehaviorProfileID;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) TArray<FString> SkillIDs;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) TArray<FString> EffectIDs;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) TArray<FString> TargetPriorityTags;
};

USTRUCT(BlueprintType)
struct FAetherMonsterDefinition
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadOnly) FString DefinitionID;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) FString DisplayName;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) FString Description;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) FAetherCombatStats Combat;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) FAetherAIProfile AI;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) TArray<FString> Tags;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) TArray<FString> LootTableIDs;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) TArray<FString> QuestIDs;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) FString SpawnAssetID;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) FString CombatAssetID;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) FString DeathEffectAssetID;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) double ExperienceReward = 0.0;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) int64 CurrencyReward = 0;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) bool bCanRespawn = true;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) double RespawnSeconds = 0.0;
    bool IsStructurallyValid(FString& OutError) const;
};

USTRUCT(BlueprintType)
struct FAetherNPCDefinition
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadOnly) FString DefinitionID;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) FString DisplayName;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) FString Description;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) EAetherNPCType Type = EAetherNPCType::Citizen;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) int32 Level = 1;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) FString FactionID;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) FString SpawnAssetID;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) FString IconAssetID;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) TArray<FString> DialogueIDs;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) TArray<FString> QuestIDs;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) TArray<FString> ShopIDs;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) TArray<FString> ServiceIDs;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) TArray<FString> Tags;
    bool IsStructurallyValid(FString& OutError) const;
};

USTRUCT(BlueprintType)
struct FAetherBossPhase
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadOnly) int32 Phase = 1;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) EAetherBossPhaseTrigger Trigger = EAetherBossPhaseTrigger::HealthPercent;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) double TriggerValue = 0.0;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) TArray<FString> SkillIDs;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) TArray<FString> AddMonsterIDs;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) TArray<FString> EffectIDs;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) FString VisualAssetID;
};

USTRUCT(BlueprintType)
struct FAetherBossDefinition
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadOnly) FString DefinitionID;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) FString DisplayName;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) FString Description;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) FAetherCombatStats Combat;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) FAetherAIProfile AI;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) TArray<FAetherBossPhase> Phases;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) TArray<FString> LootTableIDs;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) TArray<FString> QuestIDs;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) FString SpawnAssetID;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) FString CombatAssetID;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) FString DeathEffectAssetID;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) double ExperienceReward = 0.0;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) int64 CurrencyReward = 0;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) double RespawnSeconds = 0.0;
    bool IsStructurallyValid(FString& OutError) const;
};

struct FAetherWorldActorValidationIssue
{
    FString DefinitionID;
    FString Code;
    FString Message;
};
