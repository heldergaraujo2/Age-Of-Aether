#include "Misc/AutomationTest.h"
#include "Characters/AetherMovementCameraProfile.h"
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAetherMovementCameraProfileValidationTest,"AgeOfAether.Phase40.MovementCamera.ProfileValidation",EAutomationTestFlags::EditorContext|EAutomationTestFlags::EngineFilter)
bool FAetherMovementCameraProfileValidationTest::RunTest(const FString&)
{
 UAetherMovementCameraProfile* P=NewObject<UAetherMovementCameraProfile>(); FString Error;
 TestTrue(TEXT("Default profile is valid"),P->Validate(Error)); P->SprintSpeed=100.0f;
 TestFalse(TEXT("Sprint below walk is rejected"),P->Validate(Error)); TestFalse(TEXT("Invalid profile returns an error"),Error.IsEmpty()); return true;
}