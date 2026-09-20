#if WITH_DEV_AUTOMATION_TESTS
#include "Misc/AutomationTest.h"
#include "Presentation/AetherClassPresentationRegistry.h"
#include "Presentation/AetherClassPresentationSubsystem.h"

namespace
{
FAetherClassPresentationDefinition Make(const TCHAR* C,const TCHAR* E)
{
 FAetherClassPresentationDefinition D;D.ClassID=C;D.EvolutionID=E;D.MeshAssetID=FString(C)+TEXT(".mesh");D.AnimationSetAssetID=FString(C)+TEXT(".anim");D.IconAssetID=FString(C)+TEXT(".icon");D.SelectionWidgetAssetID=TEXT("ui.class.selection");D.TransitionVFXAssetID=TEXT("vfx.class.transition");D.TransitionSFXAssetID=TEXT("sfx.class.transition");return D;
}
}
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAetherClassPresentationRegistryTest,"AgeOfAether.ClassPresentation.Registry",EAutomationTestFlags::EditorContext|EAutomationTestFlags::EngineFilter)
bool FAetherClassPresentationRegistryTest::RunTest(const FString&){FAetherClassPresentationRegistry R;FString E;auto D=Make(TEXT("archer"),TEXT("archer.01"));TestTrue(TEXT("register"),R.Register(D,E));FAetherClassPresentationDefinition O;TestTrue(TEXT("resolve"),R.Resolve(TEXT(" ARCHER "),TEXT("ARCHER.01"),O));TestEqual(TEXT("mesh"),O.MeshAssetID,FString(TEXT("archer.mesh")));return true;}
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAetherClassPresentationDuplicateTest,"AgeOfAether.ClassPresentation.Duplicate",EAutomationTestFlags::EditorContext|EAutomationTestFlags::EngineFilter)
bool FAetherClassPresentationDuplicateTest::RunTest(const FString&){FAetherClassPresentationRegistry R;FString E;auto D=Make(TEXT("mage"),TEXT("mage.01"));TestTrue(TEXT("first"),R.Register(D,E));TestFalse(TEXT("duplicate"),R.Register(D,E));return true;}
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAetherClassPresentationValidationTest,"AgeOfAether.ClassPresentation.Validation",EAutomationTestFlags::EditorContext|EAutomationTestFlags::EngineFilter)
bool FAetherClassPresentationValidationTest::RunTest(const FString&){FAetherClassPresentationRegistry R;FString E;auto D=Make(TEXT("tank"),TEXT("tank.01"));D.MeshAssetID.Reset();TestFalse(TEXT("invalid mesh"),R.Register(D,E));return true;}
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAetherClassPresentationFallbackTest,"AgeOfAether.ClassPresentation.Fallback",EAutomationTestFlags::EditorContext|EAutomationTestFlags::EngineFilter)
bool FAetherClassPresentationFallbackTest::RunTest(const FString&){FAetherClassPresentationRegistry R;FString E;auto F=Make(TEXT("warrior"),TEXT("warrior.01"));R.Register(F,E);auto D=Make(TEXT("warrior"),TEXT("warrior.02"));D.FallbackPresentationID=TEXT("warrior|warrior.01");R.Register(D,E);FAetherClassPresentationDefinition O;bool B=false;TestTrue(TEXT("fallback"),R.ResolveWithFallback(TEXT("warrior"),TEXT("warrior.02"),O,B));TestFalse(TEXT("fallback flag on exact"),B);TestEqual(TEXT("exact wins"),O.EvolutionID,FString(TEXT("warrior.02")));return true;}
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAetherClassPresentationSubsystemTest,"AgeOfAether.ClassPresentation.Subsystem",EAutomationTestFlags::EditorContext|EAutomationTestFlags::EngineFilter)
bool FAetherClassPresentationSubsystemTest::RunTest(const FString&){UAetherClassPresentationSubsystem* S=NewObject<UAetherClassPresentationSubsystem>();auto D=Make(TEXT("healer"),TEXT("healer.01"));S->SetPresentation(D);FAetherClassPresentationBinding B;TestTrue(TEXT("resolve"),S->ResolvePresentation(TEXT("HEALER"),TEXT("healer.01"),B));S->SetLoadState(TEXT("healer"),TEXT("healer.01"),EAetherClassPresentationLoadState::Loading);TestEqual(TEXT("loading"),S->GetLoadState(TEXT("healer"),TEXT("healer.01")),EAetherClassPresentationLoadState::Loading);return true;}
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAetherClassPresentationDeterminismTest,"AgeOfAether.ClassPresentation.Determinism",EAutomationTestFlags::EditorContext|EAutomationTestFlags::EngineFilter)
bool FAetherClassPresentationDeterminismTest::RunTest(const FString&){FAetherClassPresentationRegistry R;FString E;const TCHAR* C[]={TEXT("archer"),TEXT("warrior"),TEXT("mage"),TEXT("tank"),TEXT("healer")};for(auto X:C){auto D=Make(X,FString::Printf(TEXT("%s.01"),X));R.Register(D,E);}TArray<FString>A,B;R.GetIDs(A);R.GetIDs(B);TestEqual(TEXT("count"),A.Num(),5);for(int32 I=0;I<A.Num();++I)TestEqual(TEXT("stable order"),A[I],B[I]);return true;}
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAetherClassPresentationResetTest,"AgeOfAether.ClassPresentation.Reset",EAutomationTestFlags::EditorContext|EAutomationTestFlags::EngineFilter)
bool FAetherClassPresentationResetTest::RunTest(const FString&){UAetherClassPresentationSubsystem* S=NewObject<UAetherClassPresentationSubsystem>();S->SetPresentation(Make(TEXT("archer"),TEXT("archer.01")));S->ResetClassPresentation();FAetherClassPresentationBinding B;TestFalse(TEXT("reset"),S->ResolvePresentation(TEXT("archer"),TEXT("archer.01"),B));return true;}
#endif
