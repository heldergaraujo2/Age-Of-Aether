#if WITH_DEV_AUTOMATION_TESTS

#include "Misc/AutomationTest.h"
#include "World/AetherWorldService.h"

namespace
{
    FAetherWorldZoneId ZoneId(const TCHAR* Value)
    {
        FAetherWorldZoneId Id;
        Id.Value = Value;
        return Id;
    }

    FAetherWorldZone MakeZone(
        const TCHAR* Id,
        EAetherWorldZoneType Type,
        int32 MinimumLevel,
        bool bEnabled = true)
    {
        FAetherWorldZone Zone;
        Zone.ZoneId = ZoneId(Id);
        Zone.DisplayName = Id;
        Zone.Type = Type;
        Zone.MinimumLevel = MinimumLevel;
        Zone.bEnabled = bEnabled;
        Zone.bAllowCombat = Type == EAetherWorldZoneType::PvP;
        return Zone;
    }

    FAetherWorldSpawnPoint MakeSpawn(
        const TCHAR* SpawnId,
        const TCHAR* Zone,
        float X,
        int32 MinimumLevel = 1)
    {
        FAetherWorldSpawnPoint Spawn;
        Spawn.SpawnId = SpawnId;
        Spawn.ZoneId = ZoneId(Zone);
        Spawn.Location = FVector(X, 10.0f, 20.0f);
        Spawn.Rotation = FRotator(0.0f, X, 0.0f);
        Spawn.MinimumLevel = MinimumLevel;
        return Spawn;
    }

    FAetherWorldPortal MakePortal(
        const TCHAR* Id,
        const TCHAR* Source,
        const TCHAR* Target,
        int32 MinimumLevel = 1,
        bool bEnabled = true)
    {
        FAetherWorldPortal Portal;
        Portal.PortalId = Id;
        Portal.SourceZoneId = ZoneId(Source);
        Portal.TargetZoneId = ZoneId(Target);
        Portal.MinimumLevel = MinimumLevel;
        Portal.bEnabled = bEnabled;
        return Portal;
    }

    FAetherCharacterRecord MakeCharacter(int32 Level)
    {
        FAetherCharacterRecord Character;
        Character.CharacterId = FAetherCharacterId::NewId();
        Character.AccountId = FAetherAccountId::NewId();
        Character.Name = TEXT("WorldTester");
        Character.Level = Level;
        Character.Status = EAetherCharacterStatus::Active;
        return Character;
    }
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FAetherWorldConfigValidationTest,
    "AgeOfAether.World.ConfigValidation",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FAetherWorldConfigValidationTest::RunTest(const FString& Parameters)
{
    FAetherWorldService Service;
    FAetherWorldConfig Config;

    Config.MaxWorldZones = 0;
    TestFalse(TEXT("Zero world-zone limit is rejected"), Service.SetConfig(Config));

    Config.MaxWorldZones = 4;
    Config.MaxSpawnsPerZone = 2;
    Config.MaxPortals = 4;
    TestTrue(TEXT("Valid world config is accepted"), Service.SetConfig(Config));
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FAetherWorldDefinitionTest,
    "AgeOfAether.World.Definitions",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FAetherWorldDefinitionTest::RunTest(const FString& Parameters)
{
    FAetherWorldService Service;
    TestTrue(TEXT("Zone A registers"), Service.RegisterZone(MakeZone(TEXT("A"), EAetherWorldZoneType::Safe, 1)));
    TestTrue(TEXT("Zone B registers"), Service.RegisterZone(MakeZone(TEXT("B"), EAetherWorldZoneType::PvP, 10)));
    TestTrue(TEXT("Spawn registers"), Service.RegisterSpawnPoint(MakeSpawn(TEXT("B-Spawn"), TEXT("B"), 100.0f)));
    TestTrue(TEXT("Portal registers"), Service.RegisterPortal(MakePortal(TEXT("A-to-B"), TEXT("A"), TEXT("B"), 10)));

    FAetherWorldZone Zone;
    TestTrue(TEXT("Zone can be found"), Service.FindZone(ZoneId(TEXT("B")), Zone));
    TestEqual(TEXT("Zone minimum level is preserved"), Zone.MinimumLevel, 10);

    FAetherWorldPortal Duplicate = MakePortal(TEXT("A-to-B"), TEXT("A"), TEXT("B"));
    TestFalse(TEXT("Duplicate portal is rejected"), Service.RegisterPortal(Duplicate));
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FAetherWorldSpawnSelectionTest,
    "AgeOfAether.World.SpawnSelection",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FAetherWorldSpawnSelectionTest::RunTest(const FString& Parameters)
{
    FAetherWorldService Service;
    TestTrue(TEXT("Zone registers"), Service.RegisterZone(MakeZone(TEXT("A"), EAetherWorldZoneType::Safe, 1)));

    FAetherWorldSpawnPoint Z = MakeSpawn(TEXT("Z"), TEXT("A"), 300.0f);
    FAetherWorldSpawnPoint A = MakeSpawn(TEXT("A"), TEXT("A"), 100.0f);
    TestTrue(TEXT("Z spawn registers"), Service.RegisterSpawnPoint(Z));
    TestTrue(TEXT("A spawn registers"), Service.RegisterSpawnPoint(A));

    FAetherWorldSpawnPoint Selected;
    TestTrue(TEXT("Spawn resolves"), Service.FindSpawnPoint(ZoneId(TEXT("A")), 1, Selected));
    TestEqual(TEXT("Selection is deterministic by spawn id"), Selected.SpawnId, FString(TEXT("A")));
    TestEqual(TEXT("Selected location is preserved"), Selected.Location, FVector(100.0f, 10.0f, 20.0f));
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FAetherWorldTransitionRulesTest,
    "AgeOfAether.World.TransitionRules",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FAetherWorldTransitionRulesTest::RunTest(const FString& Parameters)
{
    FAetherWorldService Service;
    TestTrue(TEXT("Safe zone registers"), Service.RegisterZone(MakeZone(TEXT("Safe"), EAetherWorldZoneType::Safe, 1)));
    TestTrue(TEXT("PvP zone registers"), Service.RegisterZone(MakeZone(TEXT("PvP"), EAetherWorldZoneType::PvP, 10)));
    TestTrue(TEXT("PvP spawn registers"), Service.RegisterSpawnPoint(MakeSpawn(TEXT("PvP-Spawn"), TEXT("PvP"), 500.0f)));
    TestTrue(TEXT("Portal registers"), Service.RegisterPortal(MakePortal(TEXT("Gate"), TEXT("Safe"), TEXT("PvP"), 10)));

    FAetherWorldTransitionResult Result;
    TestFalse(TEXT("Low level is rejected"), Service.ResolveTransition(ZoneId(TEXT("Safe")), ZoneId(TEXT("PvP")), 9, Result));
    TestEqual(TEXT("Level requirement is reported"), Result.Result, EAetherWorldTransitionResult::LevelRequirement);

    TestFalse(TEXT("Missing portal is rejected"), Service.ResolveTransition(ZoneId(TEXT("PvP")), ZoneId(TEXT("Safe")), 10, Result));
    TestEqual(TEXT("Missing portal is reported"), Result.Result, EAetherWorldTransitionResult::PortalNotFound);

    TestTrue(TEXT("Valid transition resolves"), Service.ResolveTransition(ZoneId(TEXT("Safe")), ZoneId(TEXT("PvP")), 10, Result));
    TestEqual(TEXT("Accepted transition is reported"), Result.Result, EAetherWorldTransitionResult::Accepted);
    TestEqual(TEXT("Target location is spawn location"), Result.TargetLocation, FVector(500.0f, 10.0f, 20.0f));
    TestEqual(TEXT("Portal id is preserved"), Result.PortalId, FString(TEXT("Gate")));
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FAetherWorldDisabledRulesTest,
    "AgeOfAether.World.DisabledRules",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FAetherWorldDisabledRulesTest::RunTest(const FString& Parameters)
{
    FAetherWorldService Service;
    TestTrue(TEXT("Source zone registers"), Service.RegisterZone(MakeZone(TEXT("A"), EAetherWorldZoneType::Safe, 1)));
    TestTrue(TEXT("Disabled target registers"), Service.RegisterZone(MakeZone(TEXT("B"), EAetherWorldZoneType::Event, 1, false)));
    TestTrue(TEXT("Spawn registers"), Service.RegisterSpawnPoint(MakeSpawn(TEXT("B-Spawn"), TEXT("B"), 200.0f)));
    TestTrue(TEXT("Portal registers"), Service.RegisterPortal(MakePortal(TEXT("A-to-B"), TEXT("A"), TEXT("B"))));

    FAetherWorldTransitionResult Result;
    TestFalse(TEXT("Disabled zone is rejected"), Service.ResolveTransition(ZoneId(TEXT("A")), ZoneId(TEXT("B")), 1, Result));
    TestEqual(TEXT("Disabled zone is reported"), Result.Result, EAetherWorldTransitionResult::ZoneDisabled);
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FAetherWorldCharacterSpawnAndTransitionTest,
    "AgeOfAether.World.CharacterSpawnAndTransition",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FAetherWorldCharacterSpawnAndTransitionTest::RunTest(const FString& Parameters)
{
    FAetherWorldService Service;
    TestTrue(TEXT("Zone A registers"), Service.RegisterZone(MakeZone(TEXT("A"), EAetherWorldZoneType::Safe, 1)));
    TestTrue(TEXT("Zone B registers"), Service.RegisterZone(MakeZone(TEXT("B"), EAetherWorldZoneType::Event, 1)));
    TestTrue(TEXT("A spawn registers"), Service.RegisterSpawnPoint(MakeSpawn(TEXT("A-Spawn"), TEXT("A"), 10.0f)));
    TestTrue(TEXT("B spawn registers"), Service.RegisterSpawnPoint(MakeSpawn(TEXT("B-Spawn"), TEXT("B"), 20.0f)));
    TestTrue(TEXT("Portal registers"), Service.RegisterPortal(MakePortal(TEXT("A-to-B"), TEXT("A"), TEXT("B"))));

    FAetherCharacterRecord Character = MakeCharacter(1);
    const FAetherAccountId AccountId = Character.AccountId;
    FAetherWorldTransitionResult Result;

    TestTrue(TEXT("Initial spawn succeeds"), Service.InitializeCharacterSpawn(AccountId, Character, Result));
    TestEqual(TEXT("Initial zone is A"), Character.CurrentZoneId, ZoneId(TEXT("A")));
    TestEqual(TEXT("Initial location is A spawn"), Character.WorldLocation, FVector(10.0f, 10.0f, 20.0f));
    TestEqual(TEXT("Initial zone type is safe"), Character.CurrentZoneType, EAetherWorldZoneType::Safe);

    TestTrue(TEXT("Portal transition succeeds"), Service.TransitionCharacter(AccountId, Character, ZoneId(TEXT("B")), Result));
    TestEqual(TEXT("Target zone is B"), Character.CurrentZoneId, ZoneId(TEXT("B")));
    TestEqual(TEXT("Target location is B spawn"), Character.WorldLocation, FVector(20.0f, 10.0f, 20.0f));
    TestEqual(TEXT("Target zone type is event"), Character.CurrentZoneType, EAetherWorldZoneType::Event);
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FAetherWorldInvalidCharacterTest,
    "AgeOfAether.World.InvalidCharacter",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FAetherWorldInvalidCharacterTest::RunTest(const FString& Parameters)
{
    FAetherWorldService Service;
    TestTrue(TEXT("Zone registers"), Service.RegisterZone(MakeZone(TEXT("A"), EAetherWorldZoneType::Safe, 1)));
    TestTrue(TEXT("Spawn registers"), Service.RegisterSpawnPoint(MakeSpawn(TEXT("A-Spawn"), TEXT("A"), 10.0f)));

    FAetherCharacterRecord Character = MakeCharacter(1);
    Character.Status = EAetherCharacterStatus::Offline;

    FAetherWorldTransitionResult Result;
    TestFalse(TEXT("Offline character cannot initialize spawn"), Service.InitializeCharacterSpawn(Character.AccountId, Character, Result));
    TestEqual(TEXT("Invalid request is reported"), Result.Result, EAetherWorldTransitionResult::InvalidRequest);
    return true;
}

#endif
