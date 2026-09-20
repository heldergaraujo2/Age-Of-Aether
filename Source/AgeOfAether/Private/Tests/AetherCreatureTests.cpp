#include "Creatures/AetherCreatureRegistry.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAetherCreatureDefinitionValidationTest,
    "AgeOfAether.Creatures.DefinitionValidation",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter)

bool FAetherCreatureDefinitionValidationTest::RunTest(const FString&)
{
    FAetherCreatureDefinition Monster;
    Monster.CreatureID = TEXT("goblin_scout");
    Monster.DisplayName = TEXT("Goblin Scout");
    Monster.MaxHealth = 150.0f;
    Monster.AttackMin = 8.0f;
    Monster.AttackMax = 12.0f;

    TestTrue(TEXT("Valid monster"), Monster.IsValid());

    Monster.AttackMin = 20.0f;
    Monster.AttackMax = 10.0f;
    TestFalse(TEXT("Inverted attack range rejected"), Monster.IsValid());
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAetherCreatureRegistryTest,
    "AgeOfAether.Creatures.Registry",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter)

bool FAetherCreatureRegistryTest::RunTest(const FString&)
{
    FAetherCreatureRegistry Registry;

    FAetherCreatureDefinition Boss;
    Boss.CreatureID = TEXT("ancient_guardian");
    Boss.DisplayName = TEXT("Ancient Guardian");
    Boss.Role = EAetherCreatureRole::Boss;
    Boss.MaxHealth = 5000.0f;

    TestTrue(TEXT("Boss registered"), Registry.RegisterCreature(Boss));
    TestFalse(TEXT("Duplicate boss rejected"), Registry.RegisterCreature(Boss));

    FAetherCreatureSpawnDefinition Spawn;
    Spawn.SpawnID = TEXT("guardian_spawn");
    Spawn.CreatureID = TEXT("ANCIENT_GUARDIAN");
    Spawn.ZoneID.Value = TEXT("starter");
    Spawn.MaxAlive = 1;

    TestTrue(TEXT("Spawn registered"), Registry.RegisterSpawn(Spawn));
    TestNotNull(TEXT("Normalized creature lookup"), Registry.FindCreature(TEXT("Ancient_Guardian")));
    TestNotNull(TEXT("Spawn lookup"), Registry.FindSpawn(TEXT("GUARDIAN_SPAWN")));

    TArray<FString> Errors;
    TestTrue(TEXT("Registry valid"), Registry.Validate(Errors));
    TestEqual(TEXT("No registry errors"), Errors.Num(), 0);
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAetherCreatureSpawnValidationTest,
    "AgeOfAether.Creatures.SpawnValidation",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter)

bool FAetherCreatureSpawnValidationTest::RunTest(const FString&)
{
    FAetherCreatureSpawnDefinition Spawn;
    Spawn.SpawnID = TEXT("spawn");
    Spawn.CreatureID = TEXT("missing");
    Spawn.ZoneID.Value = TEXT("starter");
    Spawn.MaxAlive = 1;
    TestTrue(TEXT("Basic spawn definition validates structurally"), Spawn.IsValid());

    Spawn.MaxAlive = 0;
    TestFalse(TEXT("Zero max alive rejected"), Spawn.IsValid());
    return true;
}
