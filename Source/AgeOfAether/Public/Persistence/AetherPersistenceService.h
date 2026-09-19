#pragma once

#include "CoreMinimal.h"
#include "Persistence/AetherPersistenceTypes.h"

class FAetherPersistenceService
{
public:
    bool SaveSnapshot(
        const FAetherCharacterPersistenceSnapshot& Snapshot,
        uint64 ExpectedRevision,
        FAetherPersistenceOperation& OutOperation);

    bool ImportSnapshot(
        const FAetherCharacterPersistenceSnapshot& Snapshot);

    bool RestoreSnapshot(
        const FAetherCharacterPersistenceSnapshot& Snapshot);

    bool LoadSnapshot(
        const FAetherCharacterId& CharacterId,
        FAetherCharacterPersistenceSnapshot& OutSnapshot,
        EAetherPersistenceResult& OutResult) const;

    bool DeleteSnapshot(
        const FAetherCharacterId& CharacterId,
        EAetherPersistenceResult& OutResult);

    void GetSnapshots(TArray<FAetherCharacterPersistenceSnapshot>& OutSnapshots) const;
    bool HasSnapshot(const FAetherCharacterId& CharacterId) const;
    int32 NumSnapshots() const;

    static bool ValidateSnapshot(const FAetherCharacterPersistenceSnapshot& Snapshot);
    static FString ComputeChecksum(const FAetherCharacterPersistenceSnapshot& Snapshot);
    static bool VerifyChecksum(const FAetherCharacterPersistenceSnapshot& Snapshot);
    static bool MigrateSnapshot(FAetherCharacterPersistenceSnapshot& Snapshot);

private:
    TMap<FAetherCharacterId, FAetherCharacterPersistenceSnapshot> Snapshots;
};
