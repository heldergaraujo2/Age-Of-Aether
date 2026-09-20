#include "Skills/AetherSkillRegistry.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAetherSkillDefinitionValidationTest,
    "AgeOfAether.Skills.DefinitionValidation",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter)

bool FAetherSkillDefinitionValidationTest::RunTest(const FString&)
{
    FAetherSkillDefinition Definition;
    Definition.SkillID = TEXT("Warrior.Basic");
    Definition.DisplayName = TEXT("Basic");
    Definition.Range = 300.0f;

    FAetherSkillEffect Effect;
    Effect.Type = EAetherSkillEffectType::Damage;
    Effect.Magnitude = 10.0f;
    Definition.Effects.Add(Effect);

    TestTrue(TEXT("Definition is valid"), Definition.IsValid());
    Definition.CooldownSeconds = -1.0f;
    TestFalse(TEXT("Negative cooldown is rejected"), Definition.IsValid());
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAetherSkillRegistryDuplicateAndNormalizeTest,
    "AgeOfAether.Skills.RegistryDuplicateAndNormalize",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter)

bool FAetherSkillRegistryDuplicateAndNormalizeTest::RunTest(const FString&)
{
    FAetherSkillRegistry Registry;

    FAetherSkillDefinition A;
    A.SkillID = TEXT("Mage.Fire");
    A.DisplayName = TEXT("Fire");

    FAetherSkillEffect Effect;
    Effect.Type = EAetherSkillEffectType::Damage;
    Effect.Magnitude = 25.0f;
    A.Effects.Add(Effect);

    TestTrue(TEXT("First registration succeeds"), Registry.Register(A));
    TestNotNull(TEXT("Normalized lookup succeeds"), Registry.Find(TEXT("MAGE.FIRE")));
    TestFalse(TEXT("Duplicate ID is rejected"), Registry.Register(A));
    TestEqual(TEXT("Registry contains one definition"), Registry.Num(), 1);

    TArray<FString> Errors;
    TestTrue(TEXT("Registry validates"), Registry.Validate(Errors));
    TestEqual(TEXT("No validation errors"), Errors.Num(), 0);
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAetherSkillStatusValidationTest,
    "AgeOfAether.Skills.BuffDebuffValidation",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter)

bool FAetherSkillStatusValidationTest::RunTest(const FString&)
{
    FAetherSkillDefinition Definition;
    Definition.SkillID = TEXT("tank.guard");
    Definition.DisplayName = TEXT("Guard");

    FAetherSkillEffect Effect;
    Effect.Type = EAetherSkillEffectType::Buff;
    Effect.EffectID = TEXT("guard");
    Effect.Magnitude = 0.20f;
    Effect.DurationSeconds = 5.0f;
    Definition.Effects.Add(Effect);

    TestTrue(TEXT("Buff definition is valid"), Definition.IsValid());

    Effect.EffectID.Reset();
    Definition.Effects[0] = Effect;
    TestFalse(TEXT("Buff without stable EffectID is rejected"), Definition.IsValid());
    return true;
}
