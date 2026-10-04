#pragma once

#include "CoreMinimal.h"
#include "World/AetherWorldTypes.h"
#include "AetherVerticalSliceTypes.generated.h"

UENUM(BlueprintType)
enum class EAetherVerticalSliceStage : uint8
{
    Settlement,
    Quest,
    Exploration,
    Combat,
    Reward,
    Dungeon,
    Boss,
    Return
};

USTRUCT(BlueprintType)
struct FAetherVerticalSliceStageDefinition
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FString StageID;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    EAetherVerticalSliceStage Stage = EAetherVerticalSliceStage::Settlement;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FString DisplayName;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FAetherWorldZoneId ZoneID;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FString MapID;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FString EntryID;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FString CompletionID;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    bool bRequired = true;

    bool IsValid() const;
};

USTRUCT(BlueprintType)
struct FAetherVerticalSliceDefinition
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FString SliceID;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FString DisplayName;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    int32 MinimumLevel = 1;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FString SettlementZoneID;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FString OpeningQuestID;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FString EnemyCreatureID;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FString LootTableID;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FString DungeonID;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FString BossCreatureID;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FString CompletionQuestID;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TArray<FAetherVerticalSliceStageDefinition> Stages;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    bool bEnabled = true;

    bool IsValid(TArray<FString>* OutErrors = nullptr) const;
};