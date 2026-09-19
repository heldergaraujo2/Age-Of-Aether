#include "Persistence/AetherPersistenceSubsystem.h"

#include "Persistence/AetherPersistenceSaveGame.h"
#include "Kismet/GameplayStatics.h"

namespace
{
    const TCHAR* PrimarySlot = TEXT("AetherServerPersistence_A");
    const TCHAR* AlternateSlot = TEXT("AetherServerPersistence_B");
}

void UAetherPersistenceSubsystem::Deinitialize()
{
    if (bLoaded)
    {
        PersistToDisk();
    }

    bLoaded = false;
    Super::Deinitialize();
}

const TCHAR* UAetherPersistenceSubsystem::GetSlotName(bool bAlternate)
{
    return bAlternate ? AlternateSlot : PrimarySlot;
}

bool UAetherPersistenceSubsystem::LoadSlot(
    bool bAlternate,
    TArray<FAetherCharacterPersistenceSnapshot>& OutSnapshots,
    uint64& OutRevision) const
{
    OutSnapshots.Reset();
    OutRevision = 0;

    if (!GetGameInstance())
    {
        return false;
    }

    USaveGame* RawSave = UGameplayStatics::LoadGameFromSlot(GetSlotName(bAlternate), 0);
    UAetherPersistenceSaveGame* Save = Cast<UAetherPersistenceSaveGame>(RawSave);
    if (!Save || Save->SaveSchemaVersion != AETHER_PERSISTENCE_SCHEMA_VERSION)
    {
        return false;
    }

    OutSnapshots = Save->Snapshots;
    OutRevision = Save->SaveRevision;
    return true;
}

bool UAetherPersistenceSubsystem::WriteSlot(
    bool bAlternate,
    const TArray<FAetherCharacterPersistenceSnapshot>& Snapshots,
    uint64 Revision) const
{
    if (!GetGameInstance())
    {
        return false;
    }

    UAetherPersistenceSaveGame* Save = Cast<UAetherPersistenceSaveGame>(
        UGameplayStatics::CreateSaveGameObject(UAetherPersistenceSaveGame::StaticClass()));

    if (!Save)
    {
        return false;
    }

    Save->SaveSchemaVersion = AETHER_PERSISTENCE_SCHEMA_VERSION;
    Save->SaveRevision = Revision;
    Save->Snapshots = Snapshots;

    return UGameplayStatics::SaveGameToSlot(Save, GetSlotName(bAlternate), 0);
}

bool UAetherPersistenceSubsystem::RefreshFromDisk()
{
    TArray<FAetherCharacterPersistenceSnapshot> PrimarySnapshots;
    TArray<FAetherCharacterPersistenceSnapshot> AlternateSnapshots;
    uint64 PrimaryRevision = 0;
    uint64 AlternateRevision = 0;

    const bool bPrimaryValid = LoadSlot(false, PrimarySnapshots, PrimaryRevision);
    const bool bAlternateValid = LoadSlot(true, AlternateSnapshots, AlternateRevision);

    if (!bPrimaryValid && !bAlternateValid)
    {
        bLoaded = true;
        bUsingAlternateSlot = false;
        ActiveRevision = 0;
        return true;
    }

    if (bAlternateValid && (!bPrimaryValid || AlternateRevision > PrimaryRevision))
    {
        bUsingAlternateSlot = true;
        ActiveRevision = AlternateRevision;
        for (const FAetherCharacterPersistenceSnapshot& Snapshot : AlternateSnapshots)
        {
            PersistenceService.ImportSnapshot(Snapshot);
        }
    }
    else
    {
        bUsingAlternateSlot = false;
        ActiveRevision = PrimaryRevision;
        for (const FAetherCharacterPersistenceSnapshot& Snapshot : PrimarySnapshots)
        {
            PersistenceService.ImportSnapshot(Snapshot);
        }
    }

    bLoaded = true;
    return true;
}

bool UAetherPersistenceSubsystem::PersistToDisk()
{
    TArray<FAetherCharacterPersistenceSnapshot> Snapshots;
    PersistenceService.GetSnapshots(Snapshots);

    const uint64 NextStorageRevision = ActiveRevision + 1;
    if (!WriteSlot(!bUsingAlternateSlot, Snapshots, NextStorageRevision))
    {
        return false;
    }

    bUsingAlternateSlot = !bUsingAlternateSlot;
    ActiveRevision = NextStorageRevision;
    return true;
}

bool UAetherPersistenceSubsystem::SaveCharacterSnapshot(
    const FAetherCharacterPersistenceSnapshot& Snapshot,
    uint64 ExpectedRevision,
    FAetherPersistenceOperation& OutOperation)
{
    if (!bLoaded && !RefreshFromDisk())
    {
        OutOperation = FAetherPersistenceOperation{};
        OutOperation.Result = EAetherPersistenceResult::StorageFailure;
        return false;
    }

    if (!PersistenceService.SaveSnapshot(Snapshot, ExpectedRevision, OutOperation))
    {
        return false;
    }

    const uint64 NextStorageRevision = ActiveRevision + 1;
    TArray<FAetherCharacterPersistenceSnapshot> Snapshots;
    PersistenceService.GetSnapshots(Snapshots);
    if (!WriteSlot(!bUsingAlternateSlot, Snapshots, NextStorageRevision))
    {
        EAetherPersistenceResult IgnoredResult;
        PersistenceService.DeleteSnapshot(Snapshot.Character.CharacterId, IgnoredResult);
        OutOperation.Result = EAetherPersistenceResult::StorageFailure;
        return false;
    }

    bUsingAlternateSlot = !bUsingAlternateSlot;
    ActiveRevision = NextStorageRevision;
    return true;
}

bool UAetherPersistenceSubsystem::LoadCharacterSnapshot(
    const FAetherCharacterId& CharacterId,
    FAetherCharacterPersistenceSnapshot& OutSnapshot,
    EAetherPersistenceResult& OutResult)
{
    if (!bLoaded && !RefreshFromDisk())
    {
        OutResult = EAetherPersistenceResult::StorageFailure;
        return false;
    }

    return PersistenceService.LoadSnapshot(CharacterId, OutSnapshot, OutResult);
}

bool UAetherPersistenceSubsystem::DeleteCharacterSnapshot(
    const FAetherCharacterId& CharacterId,
    EAetherPersistenceResult& OutResult)
{
    if (!bLoaded && !RefreshFromDisk())
    {
        OutResult = EAetherPersistenceResult::StorageFailure;
        return false;
    }

    if (!PersistenceService.DeleteSnapshot(CharacterId, OutResult))
    {
        return false;
    }

    const uint64 NextStorageRevision = ActiveRevision + 1;
    TArray<FAetherCharacterPersistenceSnapshot> Snapshots;
    PersistenceService.GetSnapshots(Snapshots);
    if (!WriteSlot(!bUsingAlternateSlot, Snapshots, NextStorageRevision))
    {
        return false;
    }

    bUsingAlternateSlot = !bUsingAlternateSlot;
    ActiveRevision = NextStorageRevision;
    return true;
}

bool UAetherPersistenceSubsystem::HasCharacterSnapshot(const FAetherCharacterId& CharacterId) const
{
    return PersistenceService.HasSnapshot(CharacterId);
}

int32 UAetherPersistenceSubsystem::NumCharacterSnapshots() const
{
    return PersistenceService.NumSnapshots();
}
