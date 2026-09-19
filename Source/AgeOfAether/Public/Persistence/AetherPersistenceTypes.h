#pragma once

#include "CoreMinimal.h"
#include "Characters/AetherCharacterTypes.h"
#include "Items/AetherItemTypes.h"
#include "Economy/AetherEconomyTypes.h"
#include "Quests/AetherQuestTypes.h"
#include "AetherPersistenceTypes.generated.h"

static constexpr int32 AETHER_PERSISTENCE_SCHEMA_VERSION = 1;

UENUM(BlueprintType)
enum class EAetherPersistenceResult : uint8
{
    Accepted,
    InvalidRequest,
    NotFound,
    InvalidSnapshot,
    SchemaMismatch,
    ChecksumMismatch,
    StorageFailure,
    Conflict,
    AlreadyExists
};

USTRUCT(BlueprintType)
struct FAetherCharacterPersistenceSnapshot
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    int32 SchemaVersion = AETHER_PERSISTENCE_SCHEMA_VERSION;

    UPROPERTY(BlueprintReadOnly)
    uint64 Revision = 0;

    UPROPERTY(BlueprintReadOnly)
    double SavedAtUtcSeconds = 0.0;

    UPROPERTY(BlueprintReadOnly)
    FAetherAccountId AccountId;

    UPROPERTY(BlueprintReadOnly)
    FAetherCharacterRecord Character;

    UPROPERTY(BlueprintReadOnly)
    TArray<FAetherInventorySlot> Inventory;

    UPROPERTY(BlueprintReadOnly)
    FAetherWallet Wallet;

    UPROPERTY(BlueprintReadOnly)
    TArray<FAetherQuestState> QuestStates;

    UPROPERTY(BlueprintReadOnly)
    FString Checksum;

    bool HasValidIdentity() const
    {
        return SchemaVersion > 0
            && AccountId.IsValid()
            && Character.CharacterId.IsValid()
            && Character.AccountId == AccountId
            && Wallet.CharacterId == Character.CharacterId;
    }
};

USTRUCT(BlueprintType)
struct FAetherPersistenceOperation
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    EAetherPersistenceResult Result = EAetherPersistenceResult::InvalidRequest;

    UPROPERTY(BlueprintReadOnly)
    uint64 Revision = 0;

    UPROPERTY(BlueprintReadOnly)
    FAetherCharacterPersistenceSnapshot Snapshot;

    bool IsAccepted() const { return Result == EAetherPersistenceResult::Accepted; }
};
