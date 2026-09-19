#if WITH_DEV_AUTOMATION_TESTS

#include "Misc/AutomationTest.h"

#include "Accounts/AetherAccountSessionTypes.h"
#include "Characters/AetherCharacterService.h"

namespace
{
    FAetherAccountId MakeAccountId()
    {
        return FAetherAccountId::NewId();
    }
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FAetherCharacterCreationTest,
    "AgeOfAether.Character.Creation",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FAetherCharacterCreationTest::RunTest(const FString& Parameters)
{
    FAetherCharacterService Service;
    const FAetherAccountId AccountId = MakeAccountId();

    FAetherCharacterRecord Character;
    TestTrue(TEXT("Character creation succeeds"),
        Service.CreateCharacter(AccountId, TEXT("  AetherHero  "), EAetherCharacterClass::Warrior, Character));

    TestTrue(TEXT("Character ID is generated"), Character.CharacterId.IsValid());
    TestEqual(TEXT("Character name is normalized"), Character.Name, FString(TEXT("aetherhero")));
    TestEqual(TEXT("Character starts at level 1"), Character.Level, 1);
    TestEqual(TEXT("Character starts available"), Character.Status, EAetherCharacterStatus::Available);
    TestTrue(TEXT("Derived health is calculated"), Character.DerivedStats.MaxHealth > 100.0f);
    TestEqual(TEXT("Account has one character"), Service.NumCharactersForAccount(AccountId), 1);

    FAetherCharacterRecord Found;
    TestTrue(TEXT("Character can be found by normalized name"),
        Service.FindCharacterByName(TEXT(" AETHERHERO "), Found));
    TestEqual(TEXT("Found character ID matches"), Found.CharacterId.Value, Character.CharacterId.Value);
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FAetherCharacterOwnershipTest,
    "AgeOfAether.Character.Ownership",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FAetherCharacterOwnershipTest::RunTest(const FString& Parameters)
{
    FAetherCharacterService Service;
    const FAetherAccountId Owner = MakeAccountId();
    const FAetherAccountId Other = MakeAccountId();

    FAetherCharacterRecord Character;
    TestTrue(TEXT("Owner can create character"),
        Service.CreateCharacter(Owner, TEXT("OwnerChar"), EAetherCharacterClass::Mage, Character));

    TestTrue(TEXT("Owner relationship is valid"),
        Service.IsCharacterOwnedByAccount(Character.CharacterId, Owner));
    TestFalse(TEXT("Other account cannot claim ownership"),
        Service.IsCharacterOwnedByAccount(Character.CharacterId, Other));

    FAetherCharacterRecord Selected;
    TestFalse(TEXT("Other account cannot select character"),
        Service.SelectCharacter(Other, Character.CharacterId, Selected));
    TestTrue(TEXT("Owner can select character"),
        Service.SelectCharacter(Owner, Character.CharacterId, Selected));
    TestEqual(TEXT("Selected character becomes active"), Selected.Status, EAetherCharacterStatus::Active);
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FAetherCharacterLimitTest,
    "AgeOfAether.Character.AccountLimit",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FAetherCharacterLimitTest::RunTest(const FString& Parameters)
{
    FAetherCharacterService Service;
    const FAetherAccountId AccountId = MakeAccountId();

    for (int32 Index = 0; Index < FAetherCharacterService::MaxCharactersPerAccount; ++Index)
    {
        FAetherCharacterRecord Character;
        TestTrue(
            *FString::Printf(TEXT("Character %d creation succeeds"), Index + 1),
            Service.CreateCharacter(
                AccountId,
                FString::Printf(TEXT("Hero%d"), Index + 1),
                EAetherCharacterClass::Warrior,
                Character));
    }

    FAetherCharacterRecord Overflow;
    TestFalse(TEXT("Character above account limit is rejected"),
        Service.CreateCharacter(AccountId, TEXT("Overflow"), EAetherCharacterClass::Warrior, Overflow));
    TestEqual(TEXT("Account remains at configured character limit"),
        Service.NumCharactersForAccount(AccountId),
        FAetherCharacterService::MaxCharactersPerAccount);
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FAetherCharacterNameRulesTest,
    "AgeOfAether.Character.NameRules",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FAetherCharacterNameRulesTest::RunTest(const FString& Parameters)
{
    FAetherCharacterService Service;
    const FAetherAccountId AccountId = MakeAccountId();

    FAetherCharacterRecord Character;
    TestFalse(TEXT("Two-character name is rejected"),
        Service.CreateCharacter(AccountId, TEXT("ab"), EAetherCharacterClass::Archer, Character));
    TestFalse(TEXT("Seventeen-character name is rejected"),
        Service.CreateCharacter(AccountId, TEXT("abcdefghijklmnopq"), EAetherCharacterClass::Archer, Character));

    TestTrue(TEXT("Valid boundary name is accepted"),
        Service.CreateCharacter(AccountId, TEXT("abcdefghijklmnop"), EAetherCharacterClass::Archer, Character));

    FAetherCharacterRecord Duplicate;
    TestFalse(TEXT("Duplicate name is rejected"),
        Service.CreateCharacter(
            MakeAccountId(),
            TEXT(" ABCDEFGHIJKLMNOP "),
            EAetherCharacterClass::Cleric,
            Duplicate));
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FAetherCharacterSelectionLifecycleTest,
    "AgeOfAether.Character.SelectionLifecycle",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FAetherCharacterSelectionLifecycleTest::RunTest(const FString& Parameters)
{
    FAetherCharacterService Service;
    const FAetherAccountId AccountId = MakeAccountId();

    FAetherCharacterRecord First;
    FAetherCharacterRecord Second;

    TestTrue(TEXT("First character creation succeeds"),
        Service.CreateCharacter(AccountId, TEXT("FirstHero"), EAetherCharacterClass::Warrior, First));
    TestTrue(TEXT("Second character creation succeeds"),
        Service.CreateCharacter(AccountId, TEXT("SecondHero"), EAetherCharacterClass::Mage, Second));

    FAetherCharacterRecord Selected;
    TestTrue(TEXT("First character can be selected"),
        Service.SelectCharacter(AccountId, First.CharacterId, Selected));

    FAetherCharacterRecord SelectedLookup;
    TestTrue(TEXT("Selected character can be retrieved"),
        Service.GetSelectedCharacter(AccountId, SelectedLookup));
    TestEqual(TEXT("Selected identity is correct"),
        SelectedLookup.CharacterId.Value,
        First.CharacterId.Value);

    TestFalse(TEXT("Different character cannot be selected simultaneously"),
        Service.SelectCharacter(AccountId, Second.CharacterId, Selected));

    TestTrue(TEXT("Selected character can be deselected"),
        Service.DeselectCharacter(AccountId, First.CharacterId));

    TestTrue(TEXT("Deselected character becomes offline"),
        Service.FindCharacter(First.CharacterId, SelectedLookup));
    TestEqual(TEXT("Deselected state is offline"),
        SelectedLookup.Status,
        EAetherCharacterStatus::Offline);

    TestTrue(TEXT("Second character can then be selected"),
        Service.SelectCharacter(AccountId, Second.CharacterId, Selected));
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FAetherCharacterStatusTest,
    "AgeOfAether.Character.Status",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FAetherCharacterStatusTest::RunTest(const FString& Parameters)
{
    FAetherCharacterService Service;
    const FAetherAccountId AccountId = MakeAccountId();

    FAetherCharacterRecord Character;
    TestTrue(TEXT("Status test character creation succeeds"),
        Service.CreateCharacter(AccountId, TEXT("StatusHero"), EAetherCharacterClass::Warrior, Character));

    TestTrue(TEXT("Character can be disabled"),
        Service.UpdateCharacterStatus(AccountId, Character.CharacterId, EAetherCharacterStatus::Disabled));

    FAetherCharacterRecord Selected;
    TestFalse(TEXT("Disabled character cannot be selected"),
        Service.SelectCharacter(AccountId, Character.CharacterId, Selected));

    TestTrue(TEXT("Character can be re-enabled"),
        Service.UpdateCharacterStatus(AccountId, Character.CharacterId, EAetherCharacterStatus::Available));

    TestTrue(TEXT("Re-enabled character can be selected"),
        Service.SelectCharacter(AccountId, Character.CharacterId, Selected));

    TestTrue(TEXT("Active character can be deleted through lifecycle status"),
        Service.UpdateCharacterStatus(AccountId, Character.CharacterId, EAetherCharacterStatus::Deleted));

    TestFalse(TEXT("Deleted character cannot be selected"),
        Service.SelectCharacter(AccountId, Character.CharacterId, Selected));
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FAetherCharacterLocationTest,
    "AgeOfAether.Character.LocationAuthority",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FAetherCharacterLocationTest::RunTest(const FString& Parameters)
{
    FAetherCharacterService Service;
    const FAetherAccountId AccountId = MakeAccountId();

    FAetherCharacterRecord Character;
    TestTrue(TEXT("Location character creation succeeds"),
        Service.CreateCharacter(AccountId, TEXT("LocationHero"), EAetherCharacterClass::Warrior, Character));

    TestFalse(TEXT("Inactive character cannot update location"),
        Service.UpdateCharacterLocation(
            AccountId,
            Character.CharacterId,
            FVector(100.0f, 200.0f, 300.0f),
            FRotator::ZeroRotator));

    FAetherCharacterRecord Selected;
    TestTrue(TEXT("Character selection succeeds"),
        Service.SelectCharacter(AccountId, Character.CharacterId, Selected));

    const FVector Location(100.0f, 200.0f, 300.0f);
    const FRotator Rotation(0.0f, 90.0f, 0.0f);

    TestTrue(TEXT("Active character location update succeeds"),
        Service.UpdateCharacterLocation(AccountId, Character.CharacterId, Location, Rotation));

    FAetherCharacterRecord Updated;
    TestTrue(TEXT("Updated character can be read"),
        Service.FindCharacter(Character.CharacterId, Updated));
    TestEqual(TEXT("Server-owned location is preserved"), Updated.WorldLocation, Location);
    TestEqual(TEXT("Server-owned rotation is preserved"), Updated.WorldRotation, Rotation);
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FAetherCharacterListingTest,
    "AgeOfAether.Character.AccountListing",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FAetherCharacterListingTest::RunTest(const FString& Parameters)
{
    FAetherCharacterService Service;
    const FAetherAccountId AccountId = MakeAccountId();

    FAetherCharacterRecord First;
    FAetherCharacterRecord Second;

    Service.CreateCharacter(AccountId, TEXT("Zulu"), EAetherCharacterClass::Warrior, First);
    Service.CreateCharacter(AccountId, TEXT("Alpha"), EAetherCharacterClass::Mage, Second);

    TArray<FAetherCharacterRecord> Characters;
    Service.GetCharactersForAccount(AccountId, Characters);

    TestEqual(TEXT("Two characters are listed"), Characters.Num(), 2);
    TestEqual(TEXT("Listing is deterministic and sorted"), Characters[0].Name, FString(TEXT("alpha")));
    TestEqual(TEXT("Second sorted entry is zulu"), Characters[1].Name, FString(TEXT("zulu")));
    return true;
}

#endif
