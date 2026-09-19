#include "Persistence/AetherPersistenceService.h"

#include "Misc/Crc.h"

namespace
{
    FString BuildCanonicalPayload(const FAetherCharacterPersistenceSnapshot& Snapshot)
    {
        FString Payload;
        Payload.Reserve(4096);
        Payload += FString::Printf(TEXT("schema=%d;revision=%llu;time=%.6f;account=%s;character=%s;name=%s;level=%d;xp=%lld;points=%d;"),
            Snapshot.SchemaVersion,
            Snapshot.Revision,
            Snapshot.SavedAtUtcSeconds,
            *Snapshot.AccountId.Value,
            *Snapshot.Character.CharacterId.Value,
            *Snapshot.Character.Name,
            Snapshot.Character.Level,
            Snapshot.Character.Experience,
            Snapshot.Character.UnspentStatPoints);

        Payload += FString::Printf(TEXT("stats=%d,%d,%d,%d,%d;health=%.6f;shield=%.6f;zone=%s;"),
            Snapshot.Character.BaseStats.Strength,
            Snapshot.Character.BaseStats.Agility,
            Snapshot.Character.BaseStats.Vitality,
            Snapshot.Character.BaseStats.Energy,
            Snapshot.Character.BaseStats.Command,
            Snapshot.Character.CurrentHealth,
            Snapshot.Character.CurrentShield,
            *Snapshot.Character.CurrentZoneId.Value);

        for (const FAetherInventorySlot& Slot : Snapshot.Inventory)
        {
            Payload += FString::Printf(TEXT("slot=%d;item=%s;def=%s;owner=%s;qty=%d;level=%d;dur=%d;binding=%d;"),
                Slot.SlotIndex,
                *Slot.Item.InstanceId.Value,
                *Slot.Item.DefinitionId.Value,
                *Slot.Item.OwnerCharacterId.Value,
                Slot.Item.Quantity,
                Slot.Item.ItemLevel,
                Slot.Item.Durability,
                static_cast<int32>(Slot.Item.Binding));
        }

        for (const FAetherCurrencyBalance& Balance : Snapshot.Wallet.Balances)
        {
            Payload += FString::Printf(TEXT("currency=%d:%lld;"),
                static_cast<int32>(Balance.Currency),
                Balance.Amount);
        }

        for (const FAetherQuestState& Quest : Snapshot.QuestStates)
        {
            Payload += FString::Printf(TEXT("quest=%s:%d:%d;"),
                *Quest.QuestId.Value,
                static_cast<int32>(Quest.Status),
                Quest.bRewardGranted ? 1 : 0);

            for (const FAetherQuestObjectiveProgress& Objective : Quest.Objectives)
            {
                Payload += FString::Printf(TEXT("objective=%s:%d:%d:%d;"),
                    *Objective.ObjectiveId,
                    Objective.CurrentCount,
                    Objective.RequiredCount,
                    Objective.bCompleted ? 1 : 0);
            }
        }

        return Payload;
    }
}

bool FAetherPersistenceService::SaveSnapshot(
    const FAetherCharacterPersistenceSnapshot& InputSnapshot,
    uint64 ExpectedRevision,
    FAetherPersistenceOperation& OutOperation)
{
    OutOperation = FAetherPersistenceOperation{};

    FAetherCharacterPersistenceSnapshot Snapshot = InputSnapshot;
    if (!MigrateSnapshot(Snapshot) || !Snapshot.HasValidIdentity())
    {
        OutOperation.Result = EAetherPersistenceResult::InvalidSnapshot;
        return false;
    }

    const FAetherCharacterPersistenceSnapshot* Existing = Snapshots.Find(Snapshot.Character.CharacterId);
    const uint64 CurrentRevision = Existing ? Existing->Revision : 0;

    if (ExpectedRevision != CurrentRevision)
    {
        OutOperation.Result = EAetherPersistenceResult::Conflict;
        OutOperation.Revision = CurrentRevision;
        return false;
    }

    Snapshot.Revision = CurrentRevision + 1;
    Snapshot.Checksum = ComputeChecksum(Snapshot);

    if (!ValidateSnapshot(Snapshot))
    {
        OutOperation.Result = EAetherPersistenceResult::InvalidSnapshot;
        return false;
    }

    Snapshots.Add(Snapshot.Character.CharacterId, Snapshot);

    OutOperation.Result = EAetherPersistenceResult::Accepted;
    OutOperation.Revision = Snapshot.Revision;
    OutOperation.Snapshot = Snapshot;
    return true;
}

bool FAetherPersistenceService::ImportSnapshot(const FAetherCharacterPersistenceSnapshot& InputSnapshot)
{
    FAetherCharacterPersistenceSnapshot Snapshot = InputSnapshot;
    if (!MigrateSnapshot(Snapshot) || !ValidateSnapshot(Snapshot) || !VerifyChecksum(Snapshot))
    {
        return false;
    }

    const FAetherCharacterPersistenceSnapshot* Existing = Snapshots.Find(Snapshot.Character.CharacterId);
    if (Existing && Existing->Revision >= Snapshot.Revision)
    {
        return false;
    }

    Snapshots.Add(Snapshot.Character.CharacterId, Snapshot);
    return true;
}

void FAetherPersistenceService::GetSnapshots(TArray<FAetherCharacterPersistenceSnapshot>& OutSnapshots) const
{
    OutSnapshots.Reset();
    OutSnapshots.Reserve(Snapshots.Num());

    for (const TPair<FAetherCharacterId, FAetherCharacterPersistenceSnapshot>& Pair : Snapshots)
    {
        OutSnapshots.Add(Pair.Value);
    }

    OutSnapshots.Sort([](const FAetherCharacterPersistenceSnapshot& A, const FAetherCharacterPersistenceSnapshot& B)
    {
        return A.Character.CharacterId.Value < B.Character.CharacterId.Value;
    });
}

bool FAetherPersistenceService::LoadSnapshot(
    const FAetherCharacterId& CharacterId,
    FAetherCharacterPersistenceSnapshot& OutSnapshot,
    EAetherPersistenceResult& OutResult) const
{
    OutSnapshot = FAetherCharacterPersistenceSnapshot{};
    OutResult = EAetherPersistenceResult::NotFound;

    const FAetherCharacterPersistenceSnapshot* Stored = Snapshots.Find(CharacterId);
    if (!Stored)
    {
        return false;
    }

    OutSnapshot = *Stored;
    if (!VerifyChecksum(OutSnapshot))
    {
        OutResult = EAetherPersistenceResult::ChecksumMismatch;
        return false;
    }

    OutResult = EAetherPersistenceResult::Accepted;
    return true;
}

bool FAetherPersistenceService::DeleteSnapshot(
    const FAetherCharacterId& CharacterId,
    EAetherPersistenceResult& OutResult)
{
    OutResult = EAetherPersistenceResult::NotFound;
    if (Snapshots.Remove(CharacterId) == 0)
    {
        return false;
    }

    OutResult = EAetherPersistenceResult::Accepted;
    return true;
}

bool FAetherPersistenceService::HasSnapshot(const FAetherCharacterId& CharacterId) const
{
    return Snapshots.Contains(CharacterId);
}

int32 FAetherPersistenceService::NumSnapshots() const
{
    return Snapshots.Num();
}

bool FAetherPersistenceService::ValidateSnapshot(const FAetherCharacterPersistenceSnapshot& Snapshot)
{
    if (!Snapshot.HasValidIdentity()
        || Snapshot.SchemaVersion != AETHER_PERSISTENCE_SCHEMA_VERSION
        || Snapshot.Revision == 0
        || Snapshot.Character.Level < 1
        || Snapshot.Character.Experience < 0
        || Snapshot.Character.UnspentStatPoints < 0
        || Snapshot.Character.CurrentHealth < 0.0f
        || Snapshot.Character.CurrentShield < 0.0f
        || Snapshot.Inventory.Num() > AETHER_DEFAULT_INVENTORY_SLOTS)
    {
        return false;
    }

    TSet<int32> OccupiedSlots;
    TSet<FAetherItemInstanceId> ItemIds;

    for (const FAetherInventorySlot& Slot : Snapshot.Inventory)
    {
        if (Slot.SlotIndex < 0 || Slot.SlotIndex >= AETHER_DEFAULT_INVENTORY_SLOTS || OccupiedSlots.Contains(Slot.SlotIndex))
        {
            return false;
        }

        if (!Slot.IsOccupied()
            || !Slot.Item.DefinitionId.IsValid()
            || Slot.Item.OwnerCharacterId != Snapshot.Character.CharacterId
            || Slot.Item.Quantity <= 0
            || ItemIds.Contains(Slot.Item.InstanceId))
        {
            return false;
        }

        OccupiedSlots.Add(Slot.SlotIndex);
        ItemIds.Add(Slot.Item.InstanceId);
    }

    for (const FAetherCurrencyBalance& Balance : Snapshot.Wallet.Balances)
    {
        if (Balance.Amount < 0)
        {
            return false;
        }
    }

    TSet<FAetherQuestId> QuestIds;
    for (const FAetherQuestState& Quest : Snapshot.QuestStates)
    {
        if (!Quest.QuestId.IsValid() || QuestIds.Contains(Quest.QuestId))
        {
            return false;
        }
        QuestIds.Add(Quest.QuestId);
    }

    return true;
}

FString FAetherPersistenceService::ComputeChecksum(const FAetherCharacterPersistenceSnapshot& Snapshot)
{
    const uint32 Hash = FCrc::StrCrc32(*BuildCanonicalPayload(Snapshot));
    return FString::Printf(TEXT("%08X"), Hash);
}

bool FAetherPersistenceService::VerifyChecksum(const FAetherCharacterPersistenceSnapshot& Snapshot)
{
    return !Snapshot.Checksum.IsEmpty() && Snapshot.Checksum == ComputeChecksum(Snapshot);
}

bool FAetherPersistenceService::MigrateSnapshot(FAetherCharacterPersistenceSnapshot& Snapshot)
{
    if (Snapshot.SchemaVersion == 0)
    {
        Snapshot.SchemaVersion = AETHER_PERSISTENCE_SCHEMA_VERSION;
    }

    return Snapshot.SchemaVersion == AETHER_PERSISTENCE_SCHEMA_VERSION;
}
