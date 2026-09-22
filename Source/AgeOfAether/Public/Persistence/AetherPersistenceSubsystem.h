#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Persistence/AetherPersistenceService.h"
#include "AetherPersistenceSubsystem.generated.h"
class UAetherPersistenceSaveGame;

UCLASS()
class AGEOFAETHER_API UAetherPersistenceSubsystem : public UGameInstanceSubsystem
{
    GENERATED_BODY()

public:
    virtual void Deinitialize() override;

    bool SaveCharacterSnapshot(
        const FAetherCharacterPersistenceSnapshot& Snapshot,
        uint64 ExpectedRevision,
        FAetherPersistenceOperation& OutOperation);

    bool SaveCharacterState(
        const FAetherAccountId& AccountId,
        const FAetherCharacterId& CharacterId,
        uint64 ExpectedRevision,
        FAetherPersistenceOperation& OutOperation);

    bool LoadCharacterState(
        const FAetherAccountId& AccountId,
        const FAetherCharacterId& CharacterId,
        FAetherPersistenceOperation& OutOperation);

    bool LoadCharacterSnapshot(
        const FAetherCharacterId& CharacterId,
        FAetherCharacterPersistenceSnapshot& OutSnapshot,
        EAetherPersistenceResult& OutResult);

    bool DeleteCharacterSnapshot(
        const FAetherCharacterId& CharacterId,
        EAetherPersistenceResult& OutResult);

    bool HasCharacterSnapshot(const FAetherCharacterId& CharacterId) const;

    int32 NumCharacterSnapshots() const;

private:
    static const TCHAR* GetSlotName(bool bAlternate);

    bool LoadSlot(bool bAlternate, TArray<FAetherCharacterPersistenceSnapshot>& OutSnapshots, uint64& OutRevision) const;
    bool WriteSlot(bool bAlternate, const TArray<FAetherCharacterPersistenceSnapshot>& Snapshots, uint64 Revision) const;
    bool RefreshFromDisk();
    bool PersistToDisk();

    FAetherPersistenceService PersistenceService;
    bool bLoaded = false;
    bool bUsingAlternateSlot = false;
    uint64 ActiveRevision = 0;
};
