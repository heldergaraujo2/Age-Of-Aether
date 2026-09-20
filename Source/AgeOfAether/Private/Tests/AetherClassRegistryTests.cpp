#include "Misc/AutomationTest.h"
#include "Data/AetherClassRegistry.h"

namespace
{
FAetherClassDefinition BaseClass(const TCHAR* ID){FAetherClassDefinition D;D.ClassID=ID;D.DisplayName=ID;D.PathTitle=TEXT("Path");D.StartingLevel=1;return D;}
FAetherClassEvolutionDefinition Evo(const TCHAR* ID,const TCHAR* C,int32 Stage){FAetherClassEvolutionDefinition D;D.EvolutionID=ID;D.ClassID=C;D.DisplayName=ID;D.EvolutionTitle=TEXT("Title");D.Stage=Stage;D.RequiredLevel=Stage; if(Stage>1)D.PrerequisiteEvolutionIDs.Add(FString::Printf(TEXT("%s.%d"),C,Stage-1)); return D;}
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAetherClassBasicTest,"AgeOfAether.Class.Basic",EAutomationTestFlags::EditorContext|EAutomationTestFlags::EngineFilter)
bool FAetherClassBasicTest::RunTest(const FString&){FAetherClassRegistry R;FString E;TestTrue(TEXT("class registers"),R.RegisterClass(BaseClass(TEXT("Archer")),E));TestTrue(TEXT("evolution registers"),R.RegisterEvolution(Evo(TEXT("Archer.Scout"),TEXT("Archer"),1),E));TestTrue(TEXT("case-insensitive"),R.ContainsClass(TEXT(" archer ")));return true;}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAetherClassInvalidTest,"AgeOfAether.Class.Invalid",EAutomationTestFlags::EditorContext|EAutomationTestFlags::EngineFilter)
bool FAetherClassInvalidTest::RunTest(const FString&){FAetherClassRegistry R;FString E;auto D=BaseClass(TEXT(""));TestFalse(TEXT("empty class rejected"),R.RegisterClass(D,E));auto X=Evo(TEXT("A"),TEXT("Missing"),1);R.RegisterEvolution(X,E);TArray<FAetherClassValidationIssue> I;TestFalse(TEXT("missing class detected"),R.Validate(I));return true;}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAetherClassDuplicateTest,"AgeOfAether.Class.Duplicate",EAutomationTestFlags::EditorContext|EAutomationTestFlags::EngineFilter)
bool FAetherClassDuplicateTest::RunTest(const FString&){FAetherClassRegistry R;FString E;TestTrue(TEXT("first"),R.RegisterClass(BaseClass(TEXT("Archer")),E));TestFalse(TEXT("duplicate normalized"),R.RegisterClass(BaseClass(TEXT(" archer ")),E));auto X=Evo(TEXT("ARCHER"),TEXT("Archer"),1);TestFalse(TEXT("cross type duplicate"),R.RegisterEvolution(X,E));return true;}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAetherClassDeterminismTest,"AgeOfAether.Class.Determinism",EAutomationTestFlags::EditorContext|EAutomationTestFlags::EngineFilter)
bool FAetherClassDeterminismTest::RunTest(const FString&){FAetherClassRegistry R;FString E;R.RegisterClass(BaseClass(TEXT("Z")),E);R.RegisterClass(BaseClass(TEXT("A")),E);R.RegisterEvolution(Evo(TEXT("Z.2"),TEXT("Z"),2),E);R.RegisterEvolution(Evo(TEXT("Z.1"),TEXT("Z"),1),E);TArray<FString> I;R.GetClassIDs(I);TestEqual(TEXT("sorted classes"),I[0],TEXT("a"));R.GetEvolutionIDsForClass(TEXT("z"),I);TestEqual(TEXT("sorted evolutions"),I[0],TEXT("z.1"));return true;}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAetherClassGraphTest,"AgeOfAether.Class.Graph",EAutomationTestFlags::EditorContext|EAutomationTestFlags::EngineFilter)
bool FAetherClassGraphTest::RunTest(const FString&){FAetherClassRegistry R;FString E;R.RegisterClass(BaseClass(TEXT("A")),E);auto X=Evo(TEXT("A.2"),TEXT("A"),2);TestTrue(TEXT("stage 2 registers"),R.RegisterEvolution(X,E));TArray<FAetherClassValidationIssue>I;TestFalse(TEXT("missing prerequisite detected"),R.Validate(I));return true;}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAetherClassReferenceTest,"AgeOfAether.Class.References",EAutomationTestFlags::EditorContext|EAutomationTestFlags::EngineFilter)
bool FAetherClassReferenceTest::RunTest(const FString&){FAetherClassRegistry R;FString E;R.RegisterClass(BaseClass(TEXT("A")),E);auto X=Evo(TEXT("A.1"),TEXT("A"),1);X.VisualAssetIDs.Add(TEXT("asset.missing"));R.RegisterEvolution(X,E);TArray<FAetherClassValidationIssue>I;TestFalse(TEXT("missing asset detected"),R.Validate(I,nullptr,nullptr,nullptr,nullptr,nullptr));return true;}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAetherClassResetTest,"AgeOfAether.Class.Reset",EAutomationTestFlags::EditorContext|EAutomationTestFlags::EngineFilter)
bool FAetherClassResetTest::RunTest(const FString&){FAetherClassRegistry R;FString E;R.RegisterClass(BaseClass(TEXT("A")),E);R.Reset();TestEqual(TEXT("reset classes"),R.NumClasses(),0);TestEqual(TEXT("reset evolutions"),R.NumEvolutions(),0);return true;}
