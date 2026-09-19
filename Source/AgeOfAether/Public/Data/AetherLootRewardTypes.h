#pragma once
#include "CoreMinimal.h"
#include "AetherLootRewardTypes.generated.h"

UENUM(BlueprintType)
enum class EAetherLootSelectionMode : uint8
{
    WeightedSingle,
    GuaranteedAll,
    RandomCount,
    ChanceEach
};

UENUM(BlueprintType)
enum class EAetherRewardType : uint8
{
    Experience,
    Currency,
    Item
};

UENUM(BlueprintType)
enum class EAetherDropTrigger : uint8
{
    OnDeath,
    OnEvent,
    OnInteraction,
    OnContainerOpen
};

USTRUCT(BlueprintType)
struct FAetherLootEntry
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadOnly) FString ItemID;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) int32 MinQuantity = 1;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) int32 MaxQuantity = 1;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) double Weight = 1.0;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) double Chance = 1.0;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) TArray<FString> RequiredTags;

    bool IsStructurallyValid(FString& OutError) const;
};

USTRUCT(BlueprintType)
struct FAetherLootTableDefinition
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadOnly) FString DefinitionID;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) FString DisplayName;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) FString Description;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) EAetherLootSelectionMode SelectionMode = EAetherLootSelectionMode::WeightedSingle;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) int32 RollCount = 1;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) TArray<FAetherLootEntry> Entries;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) TArray<FString> Tags;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) int32 MinimumLevel = 0;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) int32 MaximumLevel = 0;

    bool IsStructurallyValid(FString& OutError) const;
};

USTRUCT(BlueprintType)
struct FAetherRewardItem
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadOnly) FString ItemID;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) int32 Quantity = 1;

    bool IsStructurallyValid(FString& OutError) const;
};

USTRUCT(BlueprintType)
struct FAetherDropRuleDefinition
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadOnly) FString DefinitionID;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) FString SourceWorldActorID;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) FString LootTableID;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) EAetherDropTrigger Trigger = EAetherDropTrigger::OnDeath;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) double Chance = 1.0;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) int32 MinimumLevel = 0;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) int32 MaximumLevel = 0;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) TArray<FString> RequiredTags;

    bool IsStructurallyValid(FString& OutError) const;
};

USTRUCT(BlueprintType)
struct FAetherSpawnGroupDefinition
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadOnly) FString DefinitionID;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) FString WorldActorID;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) FString RespawnDefinitionID;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) int32 InitialCount = 1;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) int32 MaximumCount = 1;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) TArray<FString> SpawnPointTags;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) TArray<FString> RequiredWorldTags;

    bool IsStructurallyValid(FString& OutError) const;
};

USTRUCT(BlueprintType)
struct FAetherRewardDefinition
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadOnly) FString DefinitionID;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) FString DisplayName;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) FString Description;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) double Experience = 0.0;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) TMap<FString, int64> Currency;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) TArray<FAetherRewardItem> Items;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) TArray<FString> LootTableIDs;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) TArray<FString> Tags;

    bool IsStructurallyValid(FString& OutError) const;
};

USTRUCT(BlueprintType)
struct FAetherRespawnDefinition
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadOnly) FString DefinitionID;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) FString WorldActorID;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) double RespawnSeconds = 0.0;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) double RespawnJitterSeconds = 0.0;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) int32 MaxAlive = 1;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) bool bRespawnEnabled = true;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) TArray<FString> SpawnPointTags;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) TArray<FString> RequiredWorldTags;

    bool IsStructurallyValid(FString& OutError) const;
};

struct FAetherLootRewardValidationIssue
{
    FString DefinitionID;
    FString Code;
    FString Message;
};
