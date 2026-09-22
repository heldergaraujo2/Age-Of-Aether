#include "Misc/AutomationTest.h"
#include "Engine/GameInstance.h"
#include "Client/AetherPresentationSubsystem.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAetherPresentationAssetResolutionTest,"AgeOfAether.Presentation.AssetResolution",EAutomationTestFlags::EditorContext|EAutomationTestFlags::EngineFilter)
bool FAetherPresentationAssetResolutionTest::RunTest(const FString&)
{
    UAetherPresentationSubsystem* S=NewObject<UAetherPresentationSubsystem>(NewObject<UGameInstance>(GetTransientPackage()));
    S->RegisterAssetBinding(TEXT("Item.Sword"),TEXT("/Game/Aether/Items/Sword"),TEXT("/Game/Aether/Fallback"));
    FAetherAssetRuntimeBinding B;
    TestTrue(TEXT("resolve normalized asset"),S->ResolveAsset(TEXT(" item.sword "),B));
    TestTrue(TEXT("resolved binding"),B.IsUsable());
    TestEqual(TEXT("runtime path"),B.RuntimePath,FString(TEXT("/Game/Aether/Items/Sword")));
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAetherPresentationFallbackTest,"AgeOfAether.Presentation.Fallback",EAutomationTestFlags::EditorContext|EAutomationTestFlags::EngineFilter)
bool FAetherPresentationFallbackTest::RunTest(const FString&)
{
    UAetherPresentationSubsystem* S=NewObject<UAetherPresentationSubsystem>(NewObject<UGameInstance>(GetTransientPackage()));
    S->RegisterAssetBinding(TEXT("Missing"),TEXT(""),TEXT("/Game/Aether/Fallback"));
    FAetherAssetRuntimeBinding B;
    TestTrue(TEXT("binding exists"),S->ResolveAsset(TEXT("missing"),B));
    TestFalse(TEXT("binding unresolved"),B.bResolved);
    TestEqual(TEXT("fallback path"),B.RuntimePath,FString(TEXT("/Game/Aether/Fallback")));
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAetherPresentationActorTest,"AgeOfAether.Presentation.ActorSnapshot",EAutomationTestFlags::EditorContext|EAutomationTestFlags::EngineFilter)
bool FAetherPresentationActorTest::RunTest(const FString&)
{
    UAetherPresentationSubsystem* S=NewObject<UAetherPresentationSubsystem>(NewObject<UGameInstance>(GetTransientPackage()));
    FAetherPresentationActorSnapshot A; A.EntityID=TEXT("Character.1"); A.DefinitionID=TEXT("Player.Warrior"); A.MoveSpeed=300.0f; A.NormalizedHealth=0.75f;
    S->ApplyActorSnapshot(A);
    FAetherPresentationActorSnapshot Out;
    TestTrue(TEXT("actor snapshot"),S->GetActorSnapshot(TEXT("CHARACTER.1"),Out));
    TestEqual(TEXT("definition"),Out.DefinitionID,FString(TEXT("Player.Warrior")));
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAetherPresentationPerformanceTest,"AgeOfAether.Presentation.Performance",EAutomationTestFlags::EditorContext|EAutomationTestFlags::EngineFilter)
bool FAetherPresentationPerformanceTest::RunTest(const FString&)
{
    UAetherPresentationSubsystem* S=NewObject<UAetherPresentationSubsystem>(NewObject<UGameInstance>(GetTransientPackage()));
    S->RecordPerformanceSample(EAetherPerformanceMetric::Frame,12.0,16.6,1.0);
    FAetherPerformanceSample Sample;
    TestTrue(TEXT("sample available"),S->GetLatestPerformanceSample(EAetherPerformanceMetric::Frame,Sample));
    TestTrue(TEXT("within budget"),Sample.IsWithinBudget());
    S->RecordPerformanceSample(EAetherPerformanceMetric::GPU,20.0,16.6,2.0);
    TestFalse(TEXT("over budget"),S->GetLatestPerformanceSample(EAetherPerformanceMetric::GPU,Sample) && Sample.IsWithinBudget());
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAetherPresentationAcceptanceTest,"AgeOfAether.Presentation.Acceptance",EAutomationTestFlags::EditorContext|EAutomationTestFlags::EngineFilter)
bool FAetherPresentationAcceptanceTest::RunTest(const FString&)
{
    UAetherPresentationSubsystem* S=NewObject<UAetherPresentationSubsystem>(NewObject<UGameInstance>(GetTransientPackage()));
    for (uint8 I=0; I<30; ++I) S->SetAcceptanceResult(static_cast<EAetherClientAcceptanceStep>(I),true,TEXT("verified"));
    TestEqual(TEXT("all steps passed"),S->GetAcceptancePassedCount(),30);
    TestTrue(TEXT("acceptance complete"),S->IsAcceptanceComplete());
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAetherPresentationResetTest,"AgeOfAether.Presentation.Reset",EAutomationTestFlags::EditorContext|EAutomationTestFlags::EngineFilter)
bool FAetherPresentationResetTest::RunTest(const FString&)
{
    UAetherPresentationSubsystem* S=NewObject<UAetherPresentationSubsystem>(NewObject<UGameInstance>(GetTransientPackage()));
    S->RegisterAssetBinding(TEXT("A"),TEXT("/Game/A"),TEXT("/Game/F"));
    S->ResetPresentation();
    FAetherAssetRuntimeBinding B;
    TestFalse(TEXT("reset asset registry"),S->ResolveAsset(TEXT("A"),B));
    TestEqual(TEXT("reset acceptance"),S->GetAcceptancePassedCount(),0);
    return true;
}
