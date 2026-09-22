#include <limits>
#include "Misc/AutomationTest.h"
#include "Characters/AetherCharacterAnimationProfile.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FAetherCharacterAnimationProfileValidationTest,
    "AgeOfAether.Animation.Profile.Validation",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FAetherCharacterAnimationProfileValidationTest::RunTest(const FString& Parameters)
{
    UAetherCharacterAnimationProfile* Profile = NewObject<UAetherCharacterAnimationProfile>();
    Profile->AnimationProfileID = TEXT("human_base");
    Profile->WalkSpeedThreshold = 5.0f;
    Profile->RunSpeedThreshold = 500.0f;

    FString Error;
    TestTrue(TEXT("valid thresholds are accepted"), Profile->ValidateProfile(Error));

    Profile->RunSpeedThreshold = 2.0f;
    TestFalse(TEXT("run threshold cannot be below walk threshold"), Profile->ValidateProfile(Error));

    Profile->RunSpeedThreshold = std::numeric_limits<float>::infinity();
    TestFalse(TEXT("infinite threshold is rejected"), Profile->ValidateProfile(Error));

    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FAetherCharacterAnimationProfileIdentityTest,
    "AgeOfAether.Animation.Profile.Identity",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FAetherCharacterAnimationProfileIdentityTest::RunTest(const FString& Parameters)
{
    UAetherCharacterAnimationProfile* Profile = NewObject<UAetherCharacterAnimationProfile>();
    FString Error;
    TestFalse(TEXT("missing profile ID is rejected"), Profile->ValidateProfile(Error));

    Profile->AnimationProfileID = TEXT("archer_base");
    TestTrue(TEXT("profile ID makes the definition configured"), Profile->IsConfigured());

    return true;
}