#include "Misc/AutomationTest.h"
#include "Characters/AetherEquipmentVisualProfile.h"
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAetherEquipmentVisualProfileValidationTest,"AgeOfAether.EquipmentVisual.ProfileValidation",EAutomationTestFlags::EditorContext|EAutomationTestFlags::EngineFilter)
bool FAetherEquipmentVisualProfileValidationTest::RunTest(const FString& Parameters)
{
    UAetherEquipmentVisualProfile* Profile=NewObject<UAetherEquipmentVisualProfile>(); FString Error;
    TestFalse(TEXT("empty profile is rejected"),Profile->ValidateProfile(Error));
    Profile->VisualProfileID=TEXT("sword_visual"); Profile->ItemDefinitionID=TEXT("item.sword"); Profile->EquipmentSlot=EAetherDataEquipmentSlot::MainHand; Profile->VisualType=EAetherEquipmentVisualType::StaticMesh;
    TestFalse(TEXT("static visual requires mesh"),Profile->ValidateProfile(Error));
    Profile->VisualType=EAetherEquipmentVisualType::SkeletalMesh; TestFalse(TEXT("skeletal visual requires mesh"),Profile->ValidateProfile(Error)); return true;
}
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAetherEquipmentVisualProfileSlotTest,"AgeOfAether.EquipmentVisual.ProfileSlots",EAutomationTestFlags::EditorContext|EAutomationTestFlags::EngineFilter)
bool FAetherEquipmentVisualProfileSlotTest::RunTest(const FString& Parameters)
{
    UAetherEquipmentVisualProfile* Profile=NewObject<UAetherEquipmentVisualProfile>();
    Profile->VisualProfileID=TEXT("helm_visual"); Profile->ItemDefinitionID=TEXT("item.helm"); Profile->EquipmentSlot=EAetherDataEquipmentSlot::Head; Profile->VisualType=EAetherEquipmentVisualType::SkeletalMesh;
    TestEqual(TEXT("slot is preserved"),Profile->EquipmentSlot,EAetherDataEquipmentSlot::Head); TestEqual(TEXT("item identity is preserved"),Profile->ItemDefinitionID,FName(TEXT("item.helm"))); return true;
}