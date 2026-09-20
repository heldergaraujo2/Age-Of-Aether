#include "Misc/AutomationTest.h"

#include "Characters/AetherCharacter.h"
#include "Characters/AetherPlayableCharacterVisualComponent.h"
#include "Characters/AetherPlayableCharacterVisualProfile.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FAetherPlayableCharacterVisualProfileValidationTest,
    "AgeOfAether.Phase39.PlayableCharacter.VisualProfileValidation",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FAetherPlayableCharacterVisualProfileValidationTest::RunTest(const FString& Parameters)
{
    UAetherPlayableCharacterVisualProfile* Profile = NewObject<UAetherPlayableCharacterVisualProfile>();
    Profile->VisualProfileID = TEXT("character.base.default");

    FString Error;
    TestFalse(TEXT("A profile without a mesh must fail validation"), Profile->ValidateProfile(Error));
    TestFalse(TEXT("Validation should provide a useful error"), Error.IsEmpty());
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FAetherPlayableCharacterVisualComponentIntegrationTest,
    "AgeOfAether.Phase39.PlayableCharacter.VisualComponentIntegration",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FAetherPlayableCharacterVisualComponentIntegrationTest::RunTest(const FString& Parameters)
{
    AAetherCharacter* Character = NewObject<AAetherCharacter>();
    TestNotNull(TEXT("Playable character can be constructed"), Character);
    TestNotNull(TEXT("Playable character owns a dedicated visual component"), Character->GetVisualComponent());
    TestTrue(TEXT("Visual component is an Aether playable-character visual component"),
        Character->GetVisualComponent()->IsA<UAetherPlayableCharacterVisualComponent>());
    return true;
}
