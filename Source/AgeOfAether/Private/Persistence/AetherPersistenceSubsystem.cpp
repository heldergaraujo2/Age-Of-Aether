#include "Persistence/AetherPersistenceSubsystem.h"

#include "Persistence/AetherPersistenceSaveGame.h"
#include "Characters/AetherCharacterSubsystem.h"
#include "Items/AetherItemSubsystem.h"
#include "Economy/AetherEconomySubsystem.h"
#include "Quests/AetherQuestSubsystem.h"
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

    const TArray<FAetherCharacterPersistenceSnapshot>& SelectedSnapshots =
        bAlternateValid && (!bPrimaryValid || AlternateRevision > PrimaryRevision)
            ? AlternateSnapshots
            : PrimarySnapshots;

    bUsingAlternateSlot = bAlternateValid && (!bPrimaryValid || AlternateRevision > PrimaryRevision);
    ActiveRevision = bUsingAlternateSlot ? AlternateRevision : PrimaryRevision;

    for (const FAetherCharacterPersistenceSnapshot& Snapshot : SelectedSnapshots)
    {
        PersistenceService.ImportSnapshot(Snapshot);
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

    FAetherCharacterPersistenceSnapshot PreviousSnapshot;
    EAetherPersistenceResult PreviousResult = EAetherPersistenceResult::NotFound;
    const bool bHadPrevious = PersistenceService.LoadSnapshot(
        Snapshot.Character.CharacterId,
        PreviousSnapshot,
        PreviousResult);

    if (!PersistenceService.SaveSnapshot(Snapshot, ExpectedRevision, OutOperation))
    {
        return false;
    }

    const uint64 NextStorageRevision = ActiveRevision + 1;
    TArray<FAetherCharacterPersistenceSnapshot> Snapshots;
    PersistenceService.GetSnapshots(Snapshots);

    if (!WriteSlot(!bUsingAlternateSlot, Snapshots, NextStorageRevision))
    {
        if (bHadPrevious)
        {
            PersistenceService.RestoreSnapshot(PreviousSnapshot);
        }
        else
        {
            EAetherPersistenceResult IgnoredResult;
            PersistenceService.DeleteSnapshot(Snapshot.Character.CharacterId, IgnoredResult);
        }

        OutOperation.Result = EAetherPersistenceResult::StorageFailure;
        return false;
    }

    bUsingAlternateSlot = !bUsingAlternateSlot;
    ActiveRevision = NextStorageRevision;
    return true;
}

bool UAetherPersistenceSubsystem::SaveCharacterState(
    const FAetherAccountId& AccountId,
    const FAetherCharacterId& CharacterId,
    uint64 ExpectedRevision,
    FAetherPersistenceOperation& OutOperation)
{
    if (!GetGameInstance())
    {
        OutOperation = FAetherPersistenceOperation{};
        OutOperation.Result = EAetherPersistenceResult::StorageFailure;
        return false;
    }

    UAetherCharacterSubsystem* Characters = GetGameInstance()->GetSubsystem<UAetherCharacterSubsystem>();
    UAetherItemSubsystem* Items = GetGameInstance()->GetSubsystem<UAetherItemSubsystem>();
    UAetherEconomySubsystem* Economy = GetGameInstance()->GetSubsystem<UAetherEconomySubsystem>();
    UAetherQuestSubsystem* Quests = GetGameInstance()->GetSubsystem<UAetherQuestSubsystem>();

    if (!Characters || !Items || !Economy || !Quests)
    {
        OutOperation = FAetherPersistenceOperation{};
        OutOperation.Result = EAetherPersistenceResult::StorageFailure;
        return false;
    }

    FAetherCharacterRecord Character;
    if (!Characters->FindCharacter(CharacterId, Character) || Character.AccountId != AccountId)
    {
        OutOperation = FAetherPersistenceOperation{};
        OutOperation.Result = EAetherPersistenceResult::NotFound;
        return false;
    }

    FAetherCharacterPersistenceSnapshot Snapshot;
    Snapshot.AccountId = AccountId;
    Snapshot.Character = Character;
    Snapshot.SavedAtUtcSeconds = FDateTime::UtcNow().ToUnixTimestamp();

    if (!Items->GetInventory(CharacterId, Snapshot.Inventory)
        || !Economy->GetEconomyService().GetWallet(CharacterId, Snapshot.Wallet)
        || !Quests->GetQuestStates(CharacterId, Snapshot.QuestStates))
    {
        OutOperation = FAetherPersistenceOperation{};
        OutOperation.Result = EAetherPersistenceResult::InvalidSnapshot;
        return false;
    }

    return SaveCharacterSnapshot(Snapshot, ExpectedRevision, OutOperation);
}

bool UAetherPersistenceSubsystem::LoadCharacterState(
    const FAetherAccountId& AccountId,
    const FAetherCharacterId& CharacterId,
    FAetherPersistenceOperation& OutOperation)
{
    OutOperation = FAetherPersistenceOperation{};

    if (!GetGameInstance())
    {
        OutOperation.Result = EAetherPersistenceResult::StorageFailure;
        return false;
    }

    if (!bLoaded && !RefreshFromDisk())
    {
        OutOperation.Result = EAetherPersistenceResult::StorageFailure;
        return false;
    }

    EAetherPersistenceResult LoadResult = EAetherPersistenceResult::InvalidRequest;
    FAetherCharacterPersistenceSnapshot Snapshot;
    if (!PersistenceService.LoadSnapshot(CharacterId, Snapshot, LoadResult))
    {
        OutOperation.Result = LoadResult;
        return false;
    }

    if (Snapshot.AccountId != AccountId)
    {
        OutOperation.Result = EAetherPersistenceResult::InvalidSnapshot;
        return false;
    }

    UAetherCharacterSubsystem* Characters = GetGameInstance()->GetSubsystem<UAetherCharacterSubsystem>();
    UAetherItemSubsystem* Items = GetGameInstance()->GetSubsystem<UAetherItemSubsystem>();
    UAetherEconomySubsystem* Economy = GetGameInstance()->GetSubsystem<UAetherEconomySubsystem>();
    UAetherQuestSubsystem* Quests = GetGameInstance()->GetSubsystem<UAetherQuestSubsystem>();

    if (!Characters || !Items || !Economy || !Quests)
    {
        OutOperation.Result = EAetherPersistenceResult::StorageFailure;
        return false;
    }

    if (!Characters->RestoreCharacter(Snapshot.Character)
        || !Items->GetItemService().RestoreInventory(CharacterId, Snapshot.Inventory)
        || !Economy->GetEconomyService().RestoreWallet(Snapshot.Wallet)
        || !Quests->GetQuestService().RestoreQuestStates(CharacterId, Snapshot.QuestStates))
    {
        OutOperation.Result = EAetherPersistenceResult::InvalidSnapshot;
        return false;
    }

    OutOperation.Result = EAetherPersistenceResult::Accepted;
    OutOperation.Revision = Snapshot.Revision;
    OutOperation.Snapshot = Snapshot;
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

    FAetherCharacterPersistenceSnapshot PreviousSnapshot;
    EAetherPersistenceResult PreviousResult = EAetherPersistenceResult::NotFound;
    const bool bHadPrevious = PersistenceService.LoadSnapshot(CharacterId, PreviousSnapshot, PreviousResult);

    if (!PersistenceService.DeleteSnapshot(CharacterId, OutResult))
    {
        return false;
    }

    const uint64 NextStorageRevision = ActiveRevision + 1;
    TArray<FAetherCharacterPersistenceSnapshot> Snapshots;
    PersistenceService.GetSnapshots(Snapshots);

    if (!WriteSlot(!bUsingAlternateSlot, Snapshots, NextStorageRevision))
    {
        if (bHadPrevious)
        {
            PersistenceService.RestoreSnapshot(PreviousSnapshot);
        }

        OutResult = EAetherPersistenceResult::StorageFailure;
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
