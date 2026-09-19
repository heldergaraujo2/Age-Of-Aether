#if WITH_DEV_AUTOMATION_TESTS

#include "Misc/AutomationTest.h"
#include "Progression/AetherProgressionService.h"

namespace
{
    FAetherCharacterRecord MakeCharacter()
    {
        FAetherCharacterRecord Character;
        Character.CharacterId = FAetherCharacterId::NewId();
        Character.AccountId = FAetherAccountId::NewId();
        Character.Name = TEXT("ProgressionTest");
        Character.Status = EAetherCharacterStatus::Active;
        Character.Level = 1;
        Character.Experience = 0;
        Character.UnspentStatPoints = 0;
        Character.BaseStats = FAetherBaseStats();
        return Character;
    }
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FAetherProgressionCurveTest,
    "AgeOfAether.Progression.ExperienceCurve",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FAetherProgressionCurveTest::RunTest(const FString& Parameters)
{
    FAetherProgressionService Service;

    TestEqual(TEXT("Level 1 requires 100 XP"), Service.ExperienceRequiredForNextLevel(1), 100LL);
    TestEqual(TEXT("Level 2 requires 125 XP"), Service.ExperienceRequiredForNextLevel(2), 125LL);
    TestEqual(TEXT("Level 4000 has no next-level requirement"), Service.ExperienceRequiredForNextLevel(4000), 0LL);
    TestEqual(TEXT("Invalid level has no requirement"), Service.ExperienceRequiredForNextLevel(0), 0LL);
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FAetherProgressionLevelUpTest,
    "AgeOfAether.Progression.ExperienceAndLevel",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FAetherProgressionLevelUpTest::RunTest(const FString& Parameters)
{
    FAetherProgressionService Service;
    FAetherCharacterRecord Character = MakeCharacter();
    FAetherProgressionResult Result;

    TestTrue(TEXT("Experience grant succeeds"), Service.GrantExperience(Character, 100, Result));
    TestEqual(TEXT("Character reaches level 2"), Character.Level, 2);
    TestEqual(TEXT("Rollover experience is zero"), Character.Experience, 0LL);
    TestEqual(TEXT("One level gained"), Result.LevelsGained, 1);
    TestEqual(TEXT("Five stat points granted"), Result.StatPointsGranted, 5);
    TestEqual(TEXT("Five stat points remain unspent"), Character.UnspentStatPoints, 5);
    TestTrue(TEXT("Derived health increases with level"), Character.DerivedStats.MaxHealth > 100.0f);
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FAetherProgressionMultiLevelTest,
    "AgeOfAether.Progression.MultiLevel",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FAetherProgressionMultiLevelTest::RunTest(const FString& Parameters)
{
    FAetherProgressionService Service;
    FAetherCharacterRecord Character = MakeCharacter();
    FAetherProgressionResult Result;

    const int64 ToLevel2 = Service.ExperienceRequiredForNextLevel(1);
    const int64 ToLevel3 = Service.ExperienceRequiredForNextLevel(2);

    TestTrue(TEXT("Multi-level grant succeeds"), Service.GrantExperience(Character, ToLevel2 + ToLevel3 + 10, Result));
    TestEqual(TEXT("Character reaches level 3"), Character.Level, 3);
    TestEqual(TEXT("Rollover XP is preserved"), Character.Experience, 10LL);
    TestEqual(TEXT("Two levels gained"), Result.LevelsGained, 2);
    TestEqual(TEXT("Ten stat points granted"), Character.UnspentStatPoints, 10);
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FAetherProgressionStatAllocationTest,
    "AgeOfAether.Progression.StatAllocation",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FAetherProgressionStatAllocationTest::RunTest(const FString& Parameters)
{
    FAetherProgressionService Service;
    FAetherCharacterRecord Character = MakeCharacter();
    Character.UnspentStatPoints = 10;
    FAetherProgressionResult Result;

    const float InitialAttack = Character.DerivedStats.AttackMin;
    TestTrue(TEXT("Strength allocation succeeds"), Service.AllocateStatPoints(Character, EAetherCharacterStat::Strength, 7, Result));
    TestEqual(TEXT("Strength receives points"), Character.BaseStats.Strength, 17);
    TestEqual(TEXT("Remaining points are correct"), Character.UnspentStatPoints, 3);
    TestTrue(TEXT("Derived attack changes"), Character.DerivedStats.AttackMin > InitialAttack);

    TestFalse(TEXT("Overspending is rejected"), Service.AllocateStatPoints(Character, EAetherCharacterStat::Agility, 4, Result));
    TestEqual(TEXT("Overspend does not mutate points"), Character.UnspentStatPoints, 3);
    TestEqual(TEXT("Overspend does not mutate agility"), Character.BaseStats.Agility, 10);
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FAetherProgressionStatCapTest,
    "AgeOfAether.Progression.StatCap",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FAetherProgressionStatCapTest::RunTest(const FString& Parameters)
{
    FAetherProgressionService Service;
    FAetherCharacterRecord Character = MakeCharacter();
    Character.UnspentStatPoints = 100;
    Character.BaseStats.Strength = Service.GetConfig().MaxStatValue;
    FAetherProgressionResult Result;

    TestFalse(TEXT("Allocation at stat cap is rejected"), Service.AllocateStatPoints(Character, EAetherCharacterStat::Strength, 1, Result));
    TestEqual(TEXT("Stat remains capped"), Character.BaseStats.Strength, Service.GetConfig().MaxStatValue);
    TestEqual(TEXT("Points remain unspent"), Character.UnspentStatPoints, 100);
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FAetherProgressionInvalidInputTest,
    "AgeOfAether.Progression.InvalidInput",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FAetherProgressionInvalidInputTest::RunTest(const FString& Parameters)
{
    FAetherProgressionService Service;
    FAetherCharacterRecord Character = MakeCharacter();
    FAetherProgressionResult Result;

    TestFalse(TEXT("Zero XP is rejected"), Service.GrantExperience(Character, 0, Result));
    TestFalse(TEXT("Negative XP is rejected"), Service.GrantExperience(Character, -1, Result));
    TestFalse(TEXT("Zero stat allocation is rejected"), Service.AllocateStatPoints(Character, EAetherCharacterStat::Strength, 0, Result));
    TestFalse(TEXT("Negative stat allocation is rejected"), Service.AllocateStatPoints(Character, EAetherCharacterStat::Strength, -1, Result));
    TestEqual(TEXT("Character remains level 1"), Character.Level, 1);
    TestEqual(TEXT("Character remains at zero XP"), Character.Experience, 0LL);
    TestEqual(TEXT("No stat points were created"), Character.UnspentStatPoints, 0);
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FAetherProgressionLifecycleTest,
    "AgeOfAether.Progression.CharacterLifecycle",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FAetherProgressionLifecycleTest::RunTest(const FString& Parameters)
{
    FAetherProgressionService Service;
    FAetherProgressionResult Result;

    FAetherCharacterRecord Disabled = MakeCharacter();
    Disabled.Status = EAetherCharacterStatus::Disabled;
    TestFalse(TEXT("Disabled character cannot gain XP"), Service.GrantExperience(Disabled, 100, Result));
    TestEqual(TEXT("Disabled character level is unchanged"), Disabled.Level, 1);

    FAetherCharacterRecord Deleted = MakeCharacter();
    Deleted.Status = EAetherCharacterStatus::Deleted;
    TestFalse(TEXT("Deleted character cannot allocate stats"), Service.AllocateStatPoints(Deleted, EAetherCharacterStat::Strength, 1, Result));
    return true;
}

#endif
