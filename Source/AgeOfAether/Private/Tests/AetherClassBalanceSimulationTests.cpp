#if WITH_DEV_AUTOMATION_TESTS
#include "Misc/AutomationTest.h"
#include "Data/AetherClassBalanceSimulation.h"
#include "Data/AetherClassCatalog.h"
#include "Data/AetherClassBalanceConfig.h"

namespace
{
FAetherClassBalanceRegistry BuildNeutralRegistry()
{
    FAetherClassBalanceRegistry R;
    FAetherBalanceProfileDefinition P;
    P.ProfileID = TEXT("testing"); P.Version = 1;
    const TCHAR* Classes[] = {TEXT("archer"),TEXT("warrior"),TEXT("mage"),TEXT("tank"),TEXT("healer")};
    for (const TCHAR* C : Classes)
    {
        FAetherClassBalanceDefinition D;
        D.DefinitionID = FString(C) + TEXT(".base"); D.ClassID = C;
        P.Definitions.Add(D);
    }
    FString E; R.RegisterProfile(P,E); R.SetActiveProfile(TEXT("testing"),E); R.SetFallbackProfile(TEXT("testing"),E);
    return R;
}
TArray<FAetherBalanceSimulationCase> Cases()
{
    TArray<FAetherBalanceSimulationCase> Out;
    const TCHAR* Classes[] = {TEXT("archer"),TEXT("warrior"),TEXT("mage"),TEXT("tank"),TEXT("healer")};
    for (const TCHAR* C : Classes)
    {
        FAetherBalanceSimulationCase X; X.ClassID=C; X.BaseDamage=100.0; X.TargetDefense=100.0; X.TargetResistancePercent=20.0; X.TargetClassID=TEXT("tank"); X.TargetEvolutionID=TEXT("tank.01"); Out.Add(X);
    }
    return Out;
}
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAetherBalanceSimulationNeutralTest,"AgeOfAether.BalanceSimulation.Neutral",EAutomationTestFlags::EditorContext|EAutomationTestFlags::EngineFilter)
bool FAetherBalanceSimulationNeutralTest::RunTest(const FString&)
{
    auto R=BuildNeutralRegistry(); TArray<FAetherBalanceSimulationResult> Results; FAetherBalanceSimulationReport Report;
    TestTrue(TEXT("neutral simulation passes"),FAetherClassBalanceSimulation::Simulate(R,Cases(),false,Results,Report));
    TestEqual(TEXT("five cases"),Report.CasesExecuted,5); TestEqual(TEXT("neutral damage"),Results[0].EffectiveDamage,26.6666666667,1e-6);
    TestTrue(TEXT("finite"),Report.bFinite); TestTrue(TEXT("safe"),Report.bSafetyBoundsPassed); return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAetherBalanceSimulationBothContextsTest,"AgeOfAether.BalanceSimulation.BothContexts",EAutomationTestFlags::EditorContext|EAutomationTestFlags::EngineFilter)
bool FAetherBalanceSimulationBothContextsTest::RunTest(const FString&)
{
    auto R=BuildNeutralRegistry(); TArray<FAetherBalanceSimulationResult> Results; FAetherBalanceSimulationReport A,B;
    TestTrue(TEXT("both contexts pass"),FAetherClassBalanceSimulation::SimulateBothContexts(R,Cases(),Results,A,B));
    TestEqual(TEXT("ten results"),Results.Num(),10); return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAetherBalanceSimulationDeterminismTest,"AgeOfAether.BalanceSimulation.Determinism",EAutomationTestFlags::EditorContext|EAutomationTestFlags::EngineFilter)
bool FAetherBalanceSimulationDeterminismTest::RunTest(const FString&)
{
    auto R=BuildNeutralRegistry(); FString E; TestTrue(TEXT("deterministic"),FAetherClassBalanceSimulation::ValidateSymmetryAndDeterminism(R,Cases(),E)); return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAetherBalanceSimulationInvalidInputTest,"AgeOfAether.BalanceSimulation.InvalidInput",EAutomationTestFlags::EditorContext|EAutomationTestFlags::EngineFilter)
bool FAetherBalanceSimulationInvalidInputTest::RunTest(const FString&)
{
    auto R=BuildNeutralRegistry(); auto C=Cases(); C[0].BaseDamage=-1.0; TArray<FAetherBalanceSimulationResult> Results; FAetherBalanceSimulationReport Report;
    TestFalse(TEXT("negative damage rejected"),FAetherClassBalanceSimulation::Simulate(R,C,false,Results,Report)); TestTrue(TEXT("failure recorded"),Report.Failures>0); return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAetherBalanceSimulationExtremeMultiplierTest,"AgeOfAether.BalanceSimulation.ExtremeMultiplier",EAutomationTestFlags::EditorContext|EAutomationTestFlags::EngineFilter)
bool FAetherBalanceSimulationExtremeMultiplierTest::RunTest(const FString&)
{
    auto R=BuildNeutralRegistry(); TArray<FAetherBalanceSimulationCase> C=Cases(); FString E;
    TestTrue(TEXT("extreme validation"),FAetherClassBalanceSimulation::ValidateExtremeMultipliers(R,C,false,E));
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAetherBalanceSimulationExtremeSafetyTest,"AgeOfAether.BalanceSimulation.ExtremeSafety",EAutomationTestFlags::EditorContext|EAutomationTestFlags::EngineFilter)
bool FAetherBalanceSimulationExtremeSafetyTest::RunTest(const FString&)
{
    auto R=BuildNeutralRegistry(); TArray<FAetherBalanceSimulationCase> C=Cases(); TArray<FAetherBalanceSimulationResult> Results; FAetherBalanceSimulationReport Report;
    TestTrue(TEXT("neutral extreme inputs remain safe"),FAetherClassBalanceSimulation::Simulate(R,C,false,Results,Report));
    for(const auto& X:Results){TestTrue(TEXT("finite result"),X.bFinite);TestTrue(TEXT("safe result"),X.bWithinSafetyBounds);}
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAetherBalanceSimulationMissingDefinitionTest,"AgeOfAether.BalanceSimulation.MissingDefinition",EAutomationTestFlags::EditorContext|EAutomationTestFlags::EngineFilter)
bool FAetherBalanceSimulationMissingDefinitionTest::RunTest(const FString&)
{
    auto R=BuildNeutralRegistry(); auto C=Cases(); C[0].ClassID=TEXT("missing"); TArray<FAetherBalanceSimulationResult> Results; FAetherBalanceSimulationReport Report;
    TestFalse(TEXT("missing class rejected"),FAetherClassBalanceSimulation::Simulate(R,C,false,Results,Report)); return true;
}
#endif
