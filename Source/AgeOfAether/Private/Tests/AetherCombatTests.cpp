#if WITH_DEV_AUTOMATION_TESTS

#include "Misc/AutomationTest.h"
#include "Combat/AetherCombatService.h"
#include "Combat/AetherClassCombatIntegration.h"

namespace
{
    FAetherCharacterRecord MakeCombatant(const TCHAR* Name, float LocationX)
    {
        FAetherCharacterRecord Character;
        Character.CharacterId = FAetherCharacterId::NewId();
        Character.AccountId = FAetherAccountId::NewId();
        Character.Name = Name;
        Character.Status = EAetherCharacterStatus::Active;
        Character.Level = 1;
        Character.BaseStats = FAetherBaseStats();
        Character.DerivedStats = FAetherDerivedStats();
        Character.DerivedStats.MaxHealth = 100.0f;
        Character.DerivedStats.AttackMin = 20.0f;
        Character.DerivedStats.AttackMax = 20.0f;
        Character.DerivedStats.Defense = 0.0f;
        Character.DerivedStats.Resistance = 0.0f;
        Character.CurrentHealth = 100.0f;
        Character.CurrentShield = 0.0f;
        Character.CombatState = EAetherCharacterCombatState::Alive;
        Character.WorldLocation = FVector(LocationX, 0.0f, 0.0f);
        return Character;
    }

    FAetherCombatConfig MakeDeterministicConfig()
    {
        FAetherCombatConfig Config;
        Config.MaxAttackRange = 250.0f;
        Config.AttackCooldownSeconds = 1.0f;
        Config.Accuracy = 1.0f;
        Config.CriticalChance = 0.0f;
        Config.CriticalMultiplier = 2.0f;
        Config.MinimumDamage = 1.0f;
        Config.DefenseMitigationScale = 100.0f;
        return Config;
    }
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FAetherCombatBasicHitTest,
    "AgeOfAether.Combat.BasicHit",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FAetherCombatBasicHitTest::RunTest(const FString& Parameters)
{
    FAetherCombatService Service;
    TestTrue(TEXT("Deterministic config accepted"), Service.SetConfig(MakeDeterministicConfig()));

    FAetherCharacterRecord Attacker = MakeCombatant(TEXT("Attacker"), 0.0f);
    FAetherCharacterRecord Target = MakeCombatant(TEXT("Target"), 100.0f);
    FAetherCombatResult Result;

    TestTrue(TEXT("Basic attack resolves"), Service.ResolveBasicAttack(Attacker, Target, 1, 0.0, Result));
    TestEqual(TEXT("Result is accepted"), Result.Result, EAetherCombatResultCode::Accepted);
    TestEqual(TEXT("Outcome is hit"), Result.Outcome, EAetherCombatOutcome::Hit);
    TestEqual(TEXT("Raw damage is attacker attack"), Result.RawDamage, 20.0f);
    TestEqual(TEXT("Applied damage is 20"), Result.DamageApplied, 20.0f);
    TestEqual(TEXT("Target health becomes 80"), Target.CurrentHealth, 80.0f);
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FAetherCombatDefenseResistanceTest,
    "AgeOfAether.Combat.DefenseResistance",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FAetherCombatDefenseResistanceTest::RunTest(const FString& Parameters)
{
    FAetherCombatService Service;
    FAetherCombatConfig Config = MakeDeterministicConfig();
    TestTrue(TEXT("Config accepted"), Service.SetConfig(Config));

    FAetherCharacterRecord Attacker = MakeCombatant(TEXT("Attacker"), 0.0f);
    FAetherCharacterRecord Target = MakeCombatant(TEXT("Target"), 100.0f);
    Target.DerivedStats.Defense = 100.0f;
    Target.DerivedStats.Resistance = 50.0f;

    FAetherCombatResult Result;
    TestTrue(TEXT("Attack resolves"), Service.ResolveBasicAttack(Attacker, Target, 1, 0.0, Result));
    TestEqual(TEXT("Defense and resistance reduce damage to 5"), Result.DamageApplied, 5.0f);
    TestEqual(TEXT("Health becomes 95"), Target.CurrentHealth, 95.0f);
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FAetherCombatShieldTest,
    "AgeOfAether.Combat.ShieldAbsorption",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FAetherCombatShieldTest::RunTest(const FString& Parameters)
{
    FAetherCombatService Service;
    TestTrue(TEXT("Config accepted"), Service.SetConfig(MakeDeterministicConfig()));

    FAetherCharacterRecord Attacker = MakeCombatant(TEXT("Attacker"), 0.0f);
    FAetherCharacterRecord Target = MakeCombatant(TEXT("Target"), 100.0f);
    Target.CurrentShield = 12.0f;

    FAetherCombatResult Result;
    TestTrue(TEXT("Attack resolves"), Service.ResolveBasicAttack(Attacker, Target, 1, 0.0, Result));
    TestEqual(TEXT("Shield absorbs first 12 damage"), Result.ShieldDamageApplied, 12.0f);
    TestEqual(TEXT("Remaining 8 damage reaches health"), Target.CurrentHealth, 92.0f);
    TestEqual(TEXT("Shield is depleted"), Target.CurrentShield, 0.0f);
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FAetherCombatCriticalTest,
    "AgeOfAether.Combat.Critical",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FAetherCombatCriticalTest::RunTest(const FString& Parameters)
{
    FAetherCombatService Service;
    FAetherCombatConfig Config = MakeDeterministicConfig();
    Config.CriticalChance = 1.0f;
    Config.CriticalMultiplier = 2.0f;
    TestTrue(TEXT("Critical config accepted"), Service.SetConfig(Config));

    FAetherCharacterRecord Attacker = MakeCombatant(TEXT("Attacker"), 0.0f);
    FAetherCharacterRecord Target = MakeCombatant(TEXT("Target"), 100.0f);
    FAetherCombatResult Result;

    TestTrue(TEXT("Critical attack resolves"), Service.ResolveBasicAttack(Attacker, Target, 1, 0.0, Result));
    TestTrue(TEXT("Critical flag is set"), Result.bCritical);
    TestEqual(TEXT("Critical outcome is reported"), Result.Outcome, EAetherCombatOutcome::CriticalHit);
    TestEqual(TEXT("Critical doubles damage"), Result.DamageApplied, 40.0f);
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FAetherCombatAccuracyMissTest,
    "AgeOfAether.Combat.AccuracyMiss",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FAetherCombatAccuracyMissTest::RunTest(const FString& Parameters)
{
    FAetherCombatService Service;
    FAetherCombatConfig Config = MakeDeterministicConfig();
    Config.Accuracy = 0.0f;
    TestTrue(TEXT("Miss config accepted"), Service.SetConfig(Config));

    FAetherCharacterRecord Attacker = MakeCombatant(TEXT("Attacker"), 0.0f);
    FAetherCharacterRecord Target = MakeCombatant(TEXT("Target"), 100.0f);
    FAetherCombatResult Result;

    TestTrue(TEXT("Miss resolves as a processed attack"), Service.ResolveBasicAttack(Attacker, Target, 1, 0.0, Result));
    TestEqual(TEXT("Miss result is reported"), Result.Result, EAetherCombatResultCode::Missed);
    TestEqual(TEXT("Miss outcome is reported"), Result.Outcome, EAetherCombatOutcome::Miss);
    TestEqual(TEXT("Miss deals no damage"), Target.CurrentHealth, 100.0f);
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FAetherCombatCooldownTest,
    "AgeOfAether.Combat.Cooldown",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FAetherCombatCooldownTest::RunTest(const FString& Parameters)
{
    FAetherCombatService Service;
    TestTrue(TEXT("Config accepted"), Service.SetConfig(MakeDeterministicConfig()));

    FAetherCharacterRecord Attacker = MakeCombatant(TEXT("Attacker"), 0.0f);
    FAetherCharacterRecord Target = MakeCombatant(TEXT("Target"), 100.0f);
    FAetherCombatResult Result;

    TestTrue(TEXT("First attack succeeds"), Service.ResolveBasicAttack(Attacker, Target, 1, 0.0, Result));
    TestFalse(TEXT("Immediate second attack is blocked"), Service.ResolveBasicAttack(Attacker, Target, 2, 0.5, Result));
    TestEqual(TEXT("Cooldown result is reported"), Result.Result, EAetherCombatResultCode::Cooldown);
    TestTrue(TEXT("Cooldown remaining is positive"), Result.CooldownRemaining > 0.0f);
    TestTrue(TEXT("Attack after cooldown succeeds"), Service.ResolveBasicAttack(Attacker, Target, 3, 1.0, Result));
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FAetherCombatRangeAndTargetValidationTest,
    "AgeOfAether.Combat.RangeAndTargetValidation",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FAetherCombatRangeAndTargetValidationTest::RunTest(const FString& Parameters)
{
    FAetherCombatService Service;
    TestTrue(TEXT("Config accepted"), Service.SetConfig(MakeDeterministicConfig()));

    FAetherCharacterRecord Attacker = MakeCombatant(TEXT("Attacker"), 0.0f);
    FAetherCharacterRecord Target = MakeCombatant(TEXT("Target"), 1000.0f);
    FAetherCombatResult Result;

    TestFalse(TEXT("Out of range is rejected"), Service.ResolveBasicAttack(Attacker, Target, 1, 0.0, Result));
    TestEqual(TEXT("Out of range result is reported"), Result.Result, EAetherCombatResultCode::OutOfRange);

    Target.WorldLocation = Attacker.WorldLocation;
    TestFalse(TEXT("Self target is rejected"), Service.ResolveBasicAttack(Attacker, Attacker, 2, 0.0, Result));
    TestEqual(TEXT("Invalid self-target result is reported"), Result.Result, EAetherCombatResultCode::InvalidRequest);

    Target.WorldLocation = FVector(100.0f, 0.0f, 0.0f);
    Target.CombatState = EAetherCharacterCombatState::Dead;
    Target.CurrentHealth = 0.0f;
    TestFalse(TEXT("Dead target is rejected"), Service.ResolveBasicAttack(Attacker, Target, 3, 0.0, Result));
    TestEqual(TEXT("Dead target result is reported"), Result.Result, EAetherCombatResultCode::TargetDead);
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FAetherCombatDeathTest,
    "AgeOfAether.Combat.Death",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FAetherCombatDeathTest::RunTest(const FString& Parameters)
{
    FAetherCombatService Service;
    FAetherCombatConfig Config = MakeDeterministicConfig();
    Config.MinimumDamage = 100.0f;
    TestTrue(TEXT("Death config accepted"), Service.SetConfig(Config));

    FAetherCharacterRecord Attacker = MakeCombatant(TEXT("Attacker"), 0.0f);
    FAetherCharacterRecord Target = MakeCombatant(TEXT("Target"), 100.0f);
    Target.CurrentHealth = 50.0f;
    FAetherCombatResult Result;

    TestTrue(TEXT("Lethal attack resolves"), Service.ResolveBasicAttack(Attacker, Target, 1, 0.0, Result));
    TestEqual(TEXT("Target reaches zero health"), Target.CurrentHealth, 0.0f);
    TestEqual(TEXT("Target becomes dead"), Target.CombatState, EAetherCharacterCombatState::Dead);
    TestEqual(TEXT("Defeated outcome is reported"), Result.Outcome, EAetherCombatOutcome::Defeated);
    TestFalse(TEXT("Dead target cannot be attacked again"), Service.ResolveBasicAttack(Attacker, Target, 2, 1.0, Result));
    TestEqual(TEXT("Dead target rejection is reported"), Result.Result, EAetherCombatResultCode::TargetDead);
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FAetherCombatConfigValidationTest,
    "AgeOfAether.Combat.ConfigValidation",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FAetherCombatConfigValidationTest::RunTest(const FString& Parameters)
{
    FAetherCombatService Service;
    FAetherCombatConfig Config = MakeDeterministicConfig();

    Config.MaxAttackRange = 0.0f;
    TestFalse(TEXT("Zero range is rejected"), Service.SetConfig(Config));

    Config = MakeDeterministicConfig();
    Config.Accuracy = 1.5f;
    TestFalse(TEXT("Accuracy above one is rejected"), Service.SetConfig(Config));

    Config = MakeDeterministicConfig();
    Config.CriticalMultiplier = 0.5f;
    TestFalse(TEXT("Critical multiplier below one is rejected"), Service.SetConfig(Config));
    return true;
}

#endif

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FAetherCombatAuthoritativeClassBalanceTest,
    "AgeOfAether.Combat.AuthoritativeClassBalance",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FAetherCombatAuthoritativeClassBalanceTest::RunTest(const FString& Parameters)
{
    FAetherCombatService Service;
    TestTrue(TEXT("Config accepted"), Service.SetConfig(MakeDeterministicConfig()));
    FAetherCharacterRecord Attacker = MakeCombatant(TEXT("Attacker"), 0.0f);
    FAetherCharacterRecord Target = MakeCombatant(TEXT("Target"), 100.0f);
    Attacker.ClassID = TEXT("archer"); Attacker.EvolutionID = TEXT("archer.01");
    Target.ClassID = TEXT("warrior"); Target.EvolutionID = TEXT("warrior.01");
    FAetherCombatBalanceContext Context = FAetherCombatBalanceContext::Neutral(EAetherCombatMode::PvE);
    Context.AttackerModifiers.Damage = 3.0;
    Context.TargetModifiers.IncomingDamage = 1.0;
    FAetherCombatResult Result;
    TestTrue(TEXT("Authoritative class balance attack resolves"), Service.ResolveBasicAttackAuthoritative(Attacker, Target, 1, 0.0, Context, Result));
    TestEqual(TEXT("Class/evolution damage multiplier is applied"), Result.DamageApplied, 60.0f);
    TestTrue(TEXT("Result is finite"), FMath::IsFinite(Result.DamageApplied));
    return true;
}
