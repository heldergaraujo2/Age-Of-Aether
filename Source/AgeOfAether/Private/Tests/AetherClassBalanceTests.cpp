#include "Misc/AutomationTest.h"
#include <limits>
#include "Data/AetherClassBalanceConfig.h"
#include "Data/AetherClassBalanceRegistry.h"
#include "Data/AetherClassCatalog.h"

namespace
{
FString Sample()
{
    return TEXT("config|1|testing|production\\n"
                "profile|testing|7\\n"
                "balance|testing|archer.base|archer||2|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|3|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1\\n"
                "balance|testing|archer.01|archer|archer.01|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1");
}}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAetherClassBalanceParseTest,"AgeOfAether.ClassBalance.Parse",EAutomationTestFlags::EditorContext|EAutomationTestFlags::EngineFilter)
bool FAetherClassBalanceParseTest::RunTest(const FString&)
{
 FAetherBalanceConfig C;FString E;TestTrue(TEXT("parse"),FAetherClassBalanceConfigLoader::Parse(Sample(),C,E));TestEqual(TEXT("version"),C.ConfigVersion,1);TestEqual(TEXT("profiles"),C.Profiles.Num(),1);TestEqual(TEXT("active"),C.ActiveProfileID,TEXT("testing"));TestEqual(TEXT("profile version"),C.Profiles[0].Version,7);return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAetherClassBalanceResolutionTest,"AgeOfAether.ClassBalance.Resolution",EAutomationTestFlags::EditorContext|EAutomationTestFlags::EngineFilter)
bool FAetherClassBalanceResolutionTest::RunTest(const FString&)
{
 FAetherBalanceConfig C;FString E;TestTrue(TEXT("parse"),FAetherClassBalanceConfigLoader::Parse(Sample(),C,E));FAetherClassBalanceRegistry R;TestTrue(TEXT("registry"),FAetherClassBalanceConfigLoader::BuildRegistry(C,R,E));FAetherClassBalanceModifiers M;TestTrue(TEXT("resolve class PvE"),R.Resolve(TEXT("TESTING"),TEXT("ARCHER"),TEXT(""),false,M,E));TestEqual(TEXT("PvE damage"),M.Damage,2.0);TestTrue(TEXT("resolve class PvP"),R.Resolve(TEXT("testing"),TEXT("archer"),TEXT(""),true,M,E));TestEqual(TEXT("PvP damage"),M.Damage,3.0);return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAetherClassBalanceEvolutionTest,"AgeOfAether.ClassBalance.Evolution",EAutomationTestFlags::EditorContext|EAutomationTestFlags::EngineFilter)
bool FAetherClassBalanceEvolutionTest::RunTest(const FString&)
{
 FAetherBalanceConfig C;FString E;TestTrue(TEXT("parse"),FAetherClassBalanceConfigLoader::Parse(Sample(),C,E));FAetherClassBalanceRegistry R;TestTrue(TEXT("registry"),FAetherClassBalanceConfigLoader::BuildRegistry(C,R,E));FAetherClassBalanceModifiers M;TestTrue(TEXT("resolve evolution"),R.Resolve(TEXT("testing"),TEXT("archer"),TEXT("archer.01"),false,M,E));TestEqual(TEXT("class and evolution multipliers compose"),M.Damage,2.0);return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAetherClassBalanceInvalidTest,"AgeOfAether.ClassBalance.Invalid",EAutomationTestFlags::EditorContext|EAutomationTestFlags::EngineFilter)
bool FAetherClassBalanceInvalidTest::RunTest(const FString&)
{
 const FString Bad=TEXT("config|1|testing|production\nprofile|testing|1\nbalance|testing|archer.base|archer||-1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1");
 FAetherBalanceConfig C;FString E;TestTrue(TEXT("parser accepts syntax"),FAetherClassBalanceConfigLoader::Parse(Bad,C,E));FAetherClassBalanceRegistry R;TestFalse(TEXT("registry rejects negative multiplier"),FAetherClassBalanceConfigLoader::BuildRegistry(C,R,E));return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAetherClassBalanceSafetyTest,"AgeOfAether.ClassBalance.Safety",EAutomationTestFlags::EditorContext|EAutomationTestFlags::EngineFilter)
bool FAetherClassBalanceSafetyTest::RunTest(const FString&)
{
 FAetherClassBalanceModifiers M;FString E;M.Damage=11.0;TestFalse(TEXT("upper bound rejected"),M.IsWithinSafetyBounds(E));M.Damage=0.0;TestTrue(TEXT("zero is structurally safe"),M.IsWithinSafetyBounds(E));M.Damage=std::numeric_limits<double>::quiet_NaN();TestFalse(TEXT("non-finite rejected"),M.IsWithinSafetyBounds(E));return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAetherClassBalanceProfileTest,"AgeOfAether.ClassBalance.ProfileSafety",EAutomationTestFlags::EditorContext|EAutomationTestFlags::EngineFilter)
bool FAetherClassBalanceProfileTest::RunTest(const FString&)
{
 FAetherClassBalanceRegistry R;FString E;FAetherBalanceProfileDefinition A;A.ProfileID=TEXT("testing");A.Version=1;TestTrue(TEXT("register"),R.RegisterProfile(A,E));TestFalse(TEXT("duplicate rejected"),R.RegisterProfile(A,E));TestFalse(TEXT("unknown active rejected"),R.SetActiveProfile(TEXT("production"),E));TestTrue(TEXT("fallback existing profile"),R.SetFallbackProfile(TEXT("testing"),E));return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAetherClassBalanceSerializationTest,"AgeOfAether.ClassBalance.Serialization",EAutomationTestFlags::EditorContext|EAutomationTestFlags::EngineFilter)
bool FAetherClassBalanceSerializationTest::RunTest(const FString&)
{
 FAetherBalanceConfig A;FString E;TestTrue(TEXT("parse"),FAetherClassBalanceConfigLoader::Parse(Sample(),A,E));const FString Text=FAetherClassBalanceConfigLoader::Serialize(A);FAetherBalanceConfig B;TestTrue(TEXT("round trip parse"),FAetherClassBalanceConfigLoader::Parse(Text,B,E));TestEqual(TEXT("version stable"),A.ConfigVersion,B.ConfigVersion);TestEqual(TEXT("profile count stable"),A.Profiles.Num(),B.Profiles.Num());TestEqual(TEXT("definition count stable"),A.Profiles[0].Definitions.Num(),B.Profiles[0].Definitions.Num());return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAetherClassBalanceConfigValidationTest,"AgeOfAether.ClassBalance.ConfigValidation",EAutomationTestFlags::EditorContext|EAutomationTestFlags::EngineFilter)
bool FAetherClassBalanceConfigValidationTest::RunTest(const FString&)
{
 FAetherBalanceConfig C;FString E;TestTrue(TEXT("parse"),FAetherClassBalanceConfigLoader::Parse(Sample(),C,E));FAetherClassBalanceRegistry R;TestTrue(TEXT("build"),FAetherClassBalanceConfigLoader::BuildRegistry(C,R,E));TArray<FAetherBalanceValidationIssue>I;TestTrue(TEXT("zero validation issues"),R.Validate(I));TestEqual(TEXT("no issues"),I.Num(),0);return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAetherClassBalanceCrossRegistryTest,"AgeOfAether.ClassBalance.CrossRegistry",EAutomationTestFlags::EditorContext|EAutomationTestFlags::EngineFilter)
bool FAetherClassBalanceCrossRegistryTest::RunTest(const FString&)
{
 FAetherBalanceConfig C;FString E;TestTrue(TEXT("parse"),FAetherClassBalanceConfigLoader::Parse(Sample(),C,E));FAetherClassBalanceRegistry R;TestTrue(TEXT("build"),FAetherClassBalanceConfigLoader::BuildRegistry(C,R,E));FAetherClassRegistry Classes;TestTrue(TEXT("class catalog"),FAetherClassCatalog::BuildRegistry(Classes,E));TArray<FAetherBalanceValidationIssue>I;TestTrue(TEXT("cross registry validation"),R.Validate(I,&Classes));TestEqual(TEXT("no cross registry issues"),I.Num(),0);return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAetherClassBalanceFallbackTest,"AgeOfAether.ClassBalance.Fallback",EAutomationTestFlags::EditorContext|EAutomationTestFlags::EngineFilter)
bool FAetherClassBalanceFallbackTest::RunTest(const FString&)
{
 FAetherBalanceConfig C;FString E;TestTrue(TEXT("parse"),FAetherClassBalanceConfigLoader::Parse(Sample(),C,E));FAetherClassBalanceRegistry R;TestTrue(TEXT("build"),FAetherClassBalanceConfigLoader::BuildRegistry(C,R,E));TestTrue(TEXT("active resolution"),R.SetActiveProfile(TEXT("testing"),E));FAetherClassBalanceModifiers M;TestTrue(TEXT("active resolves"),R.ResolveActive(TEXT("archer"),TEXT("archer.01"),false,M,E));TestEqual(TEXT("active value"),M.Damage,2.0);return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAetherClassBalanceSchemaTest,"AgeOfAether.ClassBalance.Schema",EAutomationTestFlags::EditorContext|EAutomationTestFlags::EngineFilter)
bool FAetherClassBalanceSchemaTest::RunTest(const FString&)
{
 const FString Bad=TEXT("config|1|testing|production\nprofile|testing|1\nbalance|testing|archer.base|archer||1");
 FAetherBalanceConfig C;FString E;TestFalse(TEXT("short balance row rejected"),FAetherClassBalanceConfigLoader::Parse(Bad,C,E));return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAetherClassBalanceMismatchTest,"AgeOfAether.ClassBalance.ReferenceMismatch",EAutomationTestFlags::EditorContext|EAutomationTestFlags::EngineFilter)
bool FAetherClassBalanceMismatchTest::RunTest(const FString&)
{
 FAetherBalanceConfig C;FString E;TestTrue(TEXT("parse"),FAetherClassBalanceConfigLoader::Parse(Sample(),C,E));C.Profiles[0].Definitions[1].ClassID=TEXT("warrior");FAetherClassBalanceRegistry R;TestTrue(TEXT("build registry"),FAetherClassBalanceConfigLoader::BuildRegistry(C,R,E));FAetherClassRegistry Classes;TestTrue(TEXT("class catalog"),FAetherClassCatalog::BuildRegistry(Classes,E));TArray<FAetherBalanceValidationIssue>I;TestFalse(TEXT("class/evolution mismatch rejected"),R.Validate(I,&Classes));return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAetherClassBalanceFileLoadTest,"AgeOfAether.ClassBalance.FileLoad",EAutomationTestFlags::EditorContext|EAutomationTestFlags::EngineFilter)
bool FAetherClassBalanceFileLoadTest::RunTest(const FString&)
{
 FAetherBalanceConfig C;FString E;TestFalse(TEXT("missing external file is rejected safely"),FAetherClassBalanceConfigLoader::LoadFile(TEXT("/__age_of_aether_balance_file_that_does_not_exist__"),C,E));TestTrue(TEXT("diagnostic is provided"),!E.IsEmpty());return true;
}
