#include "Production/AetherReleaseGate.h"
#include "Misc/AutomationTest.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAetherReleaseManifestValidationTest,
    "AgeOfAether.Release.ManifestValidation",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::EngineFilter)

bool FAetherReleaseManifestValidationTest::RunTest(const FString&)
{
    FAetherReleaseManifest Manifest;
    Manifest.BuildId = TEXT("local-test");
    Manifest.ContentPackageID = TEXT("initial");
    Manifest.ContentVersion = TEXT("1.0.0");
    TestTrue(TEXT("Manifest valid"), Manifest.IsValid());

    Manifest.Version.Empty();
    TestFalse(TEXT("Version required"), Manifest.IsValid());
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAetherReleaseGateTest,
    "AgeOfAether.Release.Gate",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::EngineFilter)

bool FAetherReleaseGateTest::RunTest(const FString&)
{
    FAetherReleaseManifest Manifest;
    Manifest.BuildId = TEXT("local-test");
    Manifest.ContentPackageID = TEXT("initial");
    Manifest.ContentVersion = TEXT("1.0.0");

    UAetherReleaseGate* Gate = NewObject<UAetherReleaseGate>();
    Gate->Configure(Manifest);

    TestFalse(TEXT("Release blocked before checks"), Gate->IsFullyPassed());

    for (uint8 I = 0; I <= static_cast<uint8>(EAetherReleaseCheck::Audio); ++I)
        TestTrue(TEXT("Check accepted"), Gate->SetCheck(static_cast<EAetherReleaseCheck>(I), true, TEXT("pass")));

    TestTrue(TEXT("Release gate passed"), Gate->IsFullyPassed());
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAetherReleaseGateFailureTest,
    "AgeOfAether.Release.GateFailure",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::EngineFilter)

bool FAetherReleaseGateFailureTest::RunTest(const FString&)
{
    FAetherReleaseManifest Manifest;
    Manifest.BuildId = TEXT("test");
    Manifest.ContentPackageID = TEXT("initial");
    Manifest.ContentVersion = TEXT("1.0.0");

    UAetherReleaseGate* Gate = NewObject<UAetherReleaseGate>();
    Gate->Configure(Manifest);
    TestTrue(TEXT("Build check can fail explicitly"), Gate->SetCheck(EAetherReleaseCheck::Build, false, TEXT("compile failure")));
    TestFalse(TEXT("Failed check blocks release"), Gate->IsFullyPassed());
    return true;
}
