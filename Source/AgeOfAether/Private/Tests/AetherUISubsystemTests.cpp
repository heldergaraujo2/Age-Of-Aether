#include "Misc/AutomationTest.h"
#include "UI/AetherUISubsystem.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAetherUIValidationTest,"AgeOfAether.UI.Validation",EAutomationTestFlags::EditorContext|EAutomationTestFlags::EngineFilter)
bool FAetherUIValidationTest::RunTest(const FString&)
{
    FAetherUIItemEntry Item;
    Item.InstanceID=TEXT("I.1"); Item.DefinitionID=TEXT("Item.Sword"); Item.Quantity=1; Item.SlotIndex=0; Item.Durability=100.0f;
    TestTrue(TEXT("valid item"),Item.IsValid());
    Item.Quantity=0; TestFalse(TEXT("invalid quantity"),Item.IsValid());
    FAetherUISettings Settings; TestTrue(TEXT("default settings"),Settings.IsValid());
    Settings.UIScale=3.0f; TestFalse(TEXT("scale bound"),Settings.IsValid());
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAetherUINavigationTest,"AgeOfAether.UI.Navigation",EAutomationTestFlags::EditorContext|EAutomationTestFlags::EngineFilter)
bool FAetherUINavigationTest::RunTest(const FString&)
{
    UAetherUISubsystem* S=NewObject<UAetherUISubsystem>();
    TestTrue(TEXT("open inventory"),S->OpenScreen(EAetherUIScreen::Inventory));
    TestTrue(TEXT("open skills"),S->OpenScreen(EAetherUIScreen::Skills));
    TestEqual(TEXT("current skills"),S->GetCurrentScreen(),EAetherUIScreen::Skills);
    TestTrue(TEXT("back inventory"),S->NavigateBack());
    TestEqual(TEXT("back result"),S->GetCurrentScreen(),EAetherUIScreen::Inventory);
    TestFalse(TEXT("loading cannot open directly"),S->OpenScreen(EAetherUIScreen::Loading));
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAetherUIModalTest,"AgeOfAether.UI.Modal",EAutomationTestFlags::EditorContext|EAutomationTestFlags::EngineFilter)
bool FAetherUIModalTest::RunTest(const FString&)
{
    UAetherUISubsystem* S=NewObject<UAetherUISubsystem>();
    S->OpenScreen(EAetherUIScreen::HUD);
    TestTrue(TEXT("open modal"),S->OpenModal(TEXT("Confirm.Delete")));
    TestFalse(TEXT("blocked navigation"),S->OpenScreen(EAetherUIScreen::Settings));
    S->CloseModal();
    TestFalse(TEXT("modal closed"),S->IsModalOpen());
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAetherUIDataViewsTest,"AgeOfAether.UI.DataViews",EAutomationTestFlags::EditorContext|EAutomationTestFlags::EngineFilter)
bool FAetherUIDataViewsTest::RunTest(const FString&)
{
    UAetherUISubsystem* S=NewObject<UAetherUISubsystem>();
    TArray<FAetherUIItemEntry> Items; FAetherUIItemEntry I; I.InstanceID=TEXT("I"); I.DefinitionID=TEXT("Sword"); I.SlotIndex=0; Items.Add(I);
    S->SetInventory(Items,60); TestEqual(TEXT("inventory count"),S->GetSnapshot().Inventory.Num(),1); TestEqual(TEXT("capacity"),S->GetSnapshot().InventoryCapacity,60);
    TArray<FAetherUIQuestEntry> Quests; FAetherUIQuestEntry Q; Q.QuestID=TEXT("Quest.Intro"); Quests.Add(Q); S->SetQuests(Quests);
    TestEqual(TEXT("quest count"),S->GetSnapshot().Quests.Num(),1);
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAetherUINotificationTest,"AgeOfAether.UI.Notifications",EAutomationTestFlags::EditorContext|EAutomationTestFlags::EngineFilter)
bool FAetherUINotificationTest::RunTest(const FString&)
{
    UAetherUISubsystem* S=NewObject<UAetherUISubsystem>();
    const uint32 ID=S->PushNotification(EAetherUINotificationType::Success,TEXT("Loot"),TEXT("Sword obtained"),10.0,5.0);
    TestTrue(TEXT("notification id"),ID>0);
    TestEqual(TEXT("active notification"),S->GetSnapshot().Notifications.Num(),1);
    TestEqual(TEXT("expired notifications"),S->ExpireNotifications(15.0),1);
    TestEqual(TEXT("notification cleared"),S->GetSnapshot().Notifications.Num(),0);
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAetherUIResetTest,"AgeOfAether.UI.Reset",EAutomationTestFlags::EditorContext|EAutomationTestFlags::EngineFilter)
bool FAetherUIResetTest::RunTest(const FString&)
{
    UAetherUISubsystem* S=NewObject<UAetherUISubsystem>();
    S->OpenScreen(EAetherUIScreen::HUD); S->OpenModal(TEXT("Test")); S->CloseModal();
    S->PushNotification(EAetherUINotificationType::Info,TEXT("T"),TEXT("M"),0.0);
    S->ResetUI();
    TestEqual(TEXT("reset screen"),S->GetCurrentScreen(),EAetherUIScreen::None);
    TestFalse(TEXT("reset modal"),S->IsModalOpen());
    TestEqual(TEXT("reset notifications"),S->GetSnapshot().Notifications.Num(),0);
    return true;
}
