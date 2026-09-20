#if WITH_DEV_AUTOMATION_TESTS
#include "Misc/AutomationTest.h"
#include "Combat/AetherClassCombatIntegration.h"
#include "Data/AetherClassCatalog.h"
#include "Data/AetherClassBalanceConfig.h"

namespace
{
FString SampleBalance()
{
    return TEXT("config|1|testing|testing\\n")
        TEXT("profile|testing|1\\n")
        TEXT("balance|testing|archer.base|archer||2|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|2|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1\\n")
        TEXT("balance|testing|archer.01|archer|archer.01|1.5|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1");
}

void MakeCharacter(FAetherCharacterRecord& C, const TCHAR* ClassID, const TCHAR* EvolutionID)
{
    C.CharacterId = FAetherCharacterId::NewId(); C.Status = EAetherCharacterStatus::Active;
    C.ClassID = ClassID; C.EvolutionID = EvolutionID; C.CurrentHealth = 100.0f;
}
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAetherClassCombatAuthoritativeResolutionTest,"AgeOfAether.ClassCombat.AuthoritativeResolution",EAutomationTestFlags::EditorContext|EAutomationTestFlags::EngineFilter)
bool FAetherClassCombatAuthoritativeResolutionTest::RunTest(const FString&)
{
    FAetherClassRegistry Classes; FString E; TestTrue(TEXT("catalog"),FAetherClassCatalog::BuildRegistry(Classes,E));
    FAetherBalanceConfig Config; TestTrue(TEXT("parse balance"),FAetherClassBalanceConfigLoader::Parse(SampleBalance(),Config,E));
    FAetherClassBalanceRegistry Balance; TestTrue(TEXT("build balance"),FAetherClassBalanceConfigLoader::BuildRegistry(Config,Balance,E));
    FAetherCharacterRecord A,T; MakeCharacter(A,TEXT("archer"),TEXT("archer.01")); MakeCharacter(T,TEXT("warrior"),TEXT("warrior.01"));
    FAetherClassCombatIntegration I; FAetherCombatBalanceContext C; TestTrue(TEXT("authoritative context"),I.ResolveAuthoritativeContext(A,T,EAetherCombatMode::PvE,Classes,Balance,C,E));
    TestTrue(TEXT("context authoritative"),C.bAuthoritative); TestEqual(TEXT("class+evolution multiplier"),C.AttackerModifiers.Damage,3.0);
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAetherClassCombatPvPSwitchTest,"AgeOfAether.ClassCombat.PvPSwitch",EAutomationTestFlags::EditorContext|EAutomationTestFlags::EngineFilter)
bool FAetherClassCombatPvPSwitchTest::RunTest(const FString&)
{
    FAetherClassRegistry Classes; FString E; FAetherClassCatalog::BuildRegistry(Classes,E);
    FAetherBalanceConfig Config; FAetherClassBalanceConfigLoader::Parse(SampleBalance(),Config,E); FAetherClassBalanceRegistry Balance; FAetherClassBalanceConfigLoader::BuildRegistry(Config,Balance,E);
    FAetherCharacterRecord A,T; MakeCharacter(A,TEXT("archer"),TEXT("archer.01")); MakeCharacter(T,TEXT("archer"),TEXT("archer.01"));
    FAetherClassCombatIntegration I; FAetherCombatBalanceContext PvE,PvP; TestTrue(TEXT("PvE"),I.ResolveAuthoritativeContext(A,T,EAetherCombatMode::PvE,Classes,Balance,PvE,E)); TestTrue(TEXT("PvP"),I.ResolveAuthoritativeContext(A,T,EAetherCombatMode::PvP,Classes,Balance,PvP,E)); TestEqual(TEXT("PvE and PvP remain separated"),PvE.AttackerModifiers.Damage,PvP.AttackerModifiers.Damage); return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAetherClassCombatClientClaimTest,"AgeOfAether.ClassCombat.ClientClaimRejected",EAutomationTestFlags::EditorContext|EAutomationTestFlags::EngineFilter)
bool FAetherClassCombatClientClaimTest::RunTest(const FString&)
{
    FAetherCharacterRecord C; MakeCharacter(C,TEXT("archer"),TEXT("archer.01")); FAetherClassCombatIntegration I; FString E;
    TestTrue(TEXT("matching claims accepted"),I.ValidateClientClassClaims(C,TEXT("archer"),TEXT("archer.01"),E));
    TestFalse(TEXT("class spoof rejected"),I.ValidateClientClassClaims(C,TEXT("mage"),TEXT("archer.01"),E));
    TestFalse(TEXT("evolution spoof rejected"),I.ValidateClientClassClaims(C,TEXT("archer"),TEXT("archer.05"),E)); return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAetherClassCombatInvalidAuthorityTest,"AgeOfAether.ClassCombat.InvalidAuthority",EAutomationTestFlags::EditorContext|EAutomationTestFlags::EngineFilter)
bool FAetherClassCombatInvalidAuthorityTest::RunTest(const FString&)
{
    FAetherClassRegistry Classes; FString E; FAetherClassCatalog::BuildRegistry(Classes,E); FAetherClassBalanceRegistry Balance; FAetherBalanceProfileDefinition P; P.ProfileID=TEXT("testing"); P.Version=1; Balance.RegisterProfile(P,E); Balance.SetFallbackProfile(TEXT("testing"),E); Balance.SetActiveProfile(TEXT("testing"),E);
    FAetherCharacterRecord A,T; MakeCharacter(A,TEXT("not-a-class"),TEXT("not-a-class.01")); MakeCharacter(T,TEXT("warrior"),TEXT("warrior.01")); FAetherClassCombatIntegration I; FAetherCombatBalanceContext C;
    TestFalse(TEXT("unknown authoritative class rejected"),I.ResolveAuthoritativeContext(A,T,EAetherCombatMode::PvE,Classes,Balance,C,E)); return true;
}
#endif
