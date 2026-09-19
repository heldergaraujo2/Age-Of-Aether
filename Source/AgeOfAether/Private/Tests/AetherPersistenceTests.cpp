#include "Misc/AutomationTest.h"
#include "Persistence/AetherPersistenceService.h"

namespace
{
    FAetherCharacterPersistenceSnapshot MakeSnapshot(const FString& AccountValue, const FString& CharacterValue)
    {
        FAetherCharacterPersistenceSnapshot Snapshot;
        Snapshot.SchemaVersion = AETHER_PERSISTENCE_SCHEMA_VERSION;
        Snapshot.SavedAtUtcSeconds = 123.0;

        Snapshot.AccountId.Value = AccountValue;
        Snapshot.Character.AccountId = Snapshot.AccountId;
        Snapshot.Character.CharacterId.Value = CharacterValue;
        Snapshot.Character.Name = TEXT("PersistedHero");
        Snapshot.Character.Level = 10;
        Snapshot.Character.Experience = 250;
        Snapshot.Character.UnspentStatPoints = 15;
        Snapshot.Character.CurrentHealth = Snapshot.Character.DerivedStats.MaxHealth;
        Snapshot.Character.CurrentShield = 10.0f;

        Snapshot.Wallet.CharacterId = Snapshot.Character.CharacterId;
        FAetherCurrencyBalance Gold;
        Gold.Currency = EAetherCurrency::Gold;
        Gold.Amount = 5000;
        Snapshot.Wallet.Balances.Add(Gold);

        FAetherInventorySlot Slot;
        Slot.SlotIndex = 0;
        Slot.Item.InstanceId = FAetherItemInstanceId::NewId();
        Slot.Item.DefinitionId = FAetherItemDefinitionId::FromString(TEXT("item.sword"));
        Slot.Item.OwnerCharacterId = Snapshot.Character.CharacterId;
        Slot.Item.Quantity = 1;
        Slot.Item.Durability = 100;
        Snapshot.Inventory.Add(Slot);

        FAetherQuestState Quest;
        Quest.QuestId = FAetherQuestId::FromString(TEXT("quest.first"));
        Quest.Status = EAetherQuestStatus::Active;
        Snapshot.QuestStates.Add(Quest);

        return Snapshot;
    }
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FAetherPersistenceSaveLoadTest,
    "AgeOfAether.Persistence.SaveLoad",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter)

bool FAetherPersistenceSaveLoadTest::RunTest(const FString&)
{
    FAetherPersistenceService Service;
    FAetherCharacterPersistenceSnapshot Input = MakeSnapshot(TEXT("account-a"), TEXT("character-a"));
    FAetherPersistenceOperation Operation;

    TestTrue(TEXT("Initial save accepted"), Service.SaveSnapshot(Input, 0, Operation));
    TestEqual(TEXT("Initial result accepted"), Operation.Result, EAetherPersistenceResult::Accepted);
    TestEqual(TEXT("Initial revision"), Operation.Revision, static_cast<uint64>(1));

    FAetherCharacterPersistenceSnapshot Loaded;
    EAetherPersistenceResult Result = EAetherPersistenceResult::InvalidRequest;
    TestTrue(TEXT("Load succeeds"), Service.LoadSnapshot(Input.Character.CharacterId, Loaded, Result));
    TestEqual(TEXT("Load result accepted"), Result, EAetherPersistenceResult::Accepted);
    TestEqual(TEXT("Loaded level"), Loaded.Character.Level, 10);
    TestEqual(TEXT("Loaded gold"), Loaded.Wallet.Balances[0].Amount, static_cast<int64>(5000));
    TestEqual(TEXT("Loaded item count"), Loaded.Inventory.Num(), 1);
    TestEqual(TEXT("Loaded quest count"), Loaded.QuestStates.Num(), 1);
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FAetherPersistenceConflictTest,
    "AgeOfAether.Persistence.OptimisticConcurrency",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter)

bool FAetherPersistenceConflictTest::RunTest(const FString&)
{
    FAetherPersistenceService Service;
    FAetherCharacterPersistenceSnapshot Input = MakeSnapshot(TEXT("account-a"), TEXT("character-a"));
    FAetherPersistenceOperation Operation;

    TestTrue(TEXT("First save"), Service.SaveSnapshot(Input, 0, Operation));

    Input.Character.Level = 11;
    TestFalse(TEXT("Stale revision rejected"), Service.SaveSnapshot(Input, 0, Operation));
    TestEqual(TEXT("Conflict result"), Operation.Result, EAetherPersistenceResult::Conflict);
    TestEqual(TEXT("Current revision reported"), Operation.Revision, static_cast<uint64>(1));

    TestTrue(TEXT("Correct revision accepted"), Service.SaveSnapshot(Input, 1, Operation));
    TestEqual(TEXT("Second revision"), Operation.Revision, static_cast<uint64>(2));
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FAetherPersistenceIntegrityTest,
    "AgeOfAether.Persistence.Integrity",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter)

bool FAetherPersistenceIntegrityTest::RunTest(const FString&)
{
    FAetherPersistenceService Service;
    FAetherCharacterPersistenceSnapshot Input = MakeSnapshot(TEXT("account-a"), TEXT("character-a"));
    FAetherPersistenceOperation Operation;

    TestTrue(TEXT("Save valid snapshot"), Service.SaveSnapshot(Input, 0, Operation));

    FAetherCharacterPersistenceSnapshot Tampered = Operation.Snapshot;
    Tampered.Character.Level = 999;
    TestFalse(TEXT("Tampered import rejected"), Service.ImportSnapshot(Tampered));

    FAetherCharacterPersistenceSnapshot Loaded;
    EAetherPersistenceResult Result = EAetherPersistenceResult::InvalidRequest;
    TestTrue(TEXT("Original snapshot remains loadable"), Service.LoadSnapshot(Input.Character.CharacterId, Loaded, Result));
    TestEqual(TEXT("Original level remains"), Loaded.Character.Level, 10);

    Tampered = Operation.Snapshot;
    Tampered.Inventory[0].Item.OwnerCharacterId = FAetherCharacterId::NewId();
    TestFalse(TEXT("Ownership tamper rejected"), FAetherPersistenceService::ValidateSnapshot(Tampered));

    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FAetherPersistenceValidationTest,
    "AgeOfAether.Persistence.Validation",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter)

bool FAetherPersistenceValidationTest::RunTest(const FString&)
{
    FAetherCharacterPersistenceSnapshot Snapshot = MakeSnapshot(TEXT("account-a"), TEXT("character-a"));

    Snapshot.Revision = 1;
    Snapshot.Checksum = FAetherPersistenceService::ComputeChecksum(Snapshot);
    TestTrue(TEXT("Baseline validates"), FAetherPersistenceService::ValidateSnapshot(Snapshot));
    TestTrue(TEXT("Baseline checksum verifies"), FAetherPersistenceService::VerifyChecksum(Snapshot));

    Snapshot.Inventory[0].SlotIndex = 64;
    TestFalse(TEXT("Out-of-range slot rejected"), FAetherPersistenceService::ValidateSnapshot(Snapshot));

    Snapshot = MakeSnapshot(TEXT("account-a"), TEXT("character-a"));
    Snapshot.Revision = 1;
    Snapshot.Wallet.Balances[0].Amount = -1;
    TestFalse(TEXT("Negative currency rejected"), FAetherPersistenceService::ValidateSnapshot(Snapshot));

    Snapshot = MakeSnapshot(TEXT("account-a"), TEXT("character-a"));
    Snapshot.Revision = 1;
    Snapshot.Character.AccountId.Value = TEXT("other-account");
    TestFalse(TEXT("Cross-account snapshot rejected"), FAetherPersistenceService::ValidateSnapshot(Snapshot));

    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FAetherPersistenceDeleteAndMigrationTest,
    "AgeOfAether.Persistence.DeleteAndMigration",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter)

bool FAetherPersistenceDeleteAndMigrationTest::RunTest(const FString&)
{
    FAetherPersistenceService Service;
    FAetherCharacterPersistenceSnapshot Snapshot = MakeSnapshot(TEXT("account-a"), TEXT("character-a"));
    Snapshot.SchemaVersion = 0;

    FAetherPersistenceOperation Operation;
    TestTrue(TEXT("Schema migration on save"), Service.SaveSnapshot(Snapshot, 0, Operation));
    TestEqual(TEXT("Migrated schema"), Operation.Snapshot.SchemaVersion, AETHER_PERSISTENCE_SCHEMA_VERSION);

    EAetherPersistenceResult Result = EAetherPersistenceResult::InvalidRequest;
    TestTrue(TEXT("Delete succeeds"), Service.DeleteSnapshot(Snapshot.Character.CharacterId, Result));
    TestEqual(TEXT("Delete result"), Result, EAetherPersistenceResult::Accepted);
    TestFalse(TEXT("Snapshot removed"), Service.HasSnapshot(Snapshot.Character.CharacterId));
    TestFalse(TEXT("Second delete rejected"), Service.DeleteSnapshot(Snapshot.Character.CharacterId, Result));
    TestEqual(TEXT("Second delete result"), Result, EAetherPersistenceResult::NotFound);
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FAetherPersistenceEnumerationTest,
    "AgeOfAether.Persistence.Enumeration",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter)

bool FAetherPersistenceEnumerationTest::RunTest(const FString&)
{
    FAetherPersistenceService Service;
    FAetherPersistenceOperation Operation;

    TestTrue(TEXT("Save character B"), Service.SaveSnapshot(MakeSnapshot(TEXT("account-b"), TEXT("character-b")), 0, Operation));
    TestTrue(TEXT("Save character A"), Service.SaveSnapshot(MakeSnapshot(TEXT("account-a"), TEXT("character-a")), 0, Operation));
    TestEqual(TEXT("Snapshot count"), Service.NumSnapshots(), 2);

    TArray<FAetherCharacterPersistenceSnapshot> Snapshots;
    Service.GetSnapshots(Snapshots);
    TestEqual(TEXT("Enumeration count"), Snapshots.Num(), 2);
    TestEqual(TEXT("Deterministic ordering"), Snapshots[0].Character.CharacterId.Value, FString(TEXT("character-a")));
    TestEqual(TEXT("Deterministic ordering second"), Snapshots[1].Character.CharacterId.Value, FString(TEXT("character-b")));
    return true;
}
