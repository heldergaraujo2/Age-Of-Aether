#include "Items/AetherLootTypes.h"
#include "Items/AetherInventoryTypes.h"
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAetherLootValidationTest,"AgeOfAether.Items.LootValidation",EAutomationTestFlags_ApplicationContextMask|EAutomationTestFlags::ProductFilter)
bool FAetherLootValidationTest::RunTest(const FString&){ UAetherLootTable* T=NewObject<UAetherLootTable>(); T->LootTableID=TEXT("starter"); FAetherLootEntry E; E.ItemID=TEXT("item.test"); T->Entries.Add(E); FString Error; TestTrue(TEXT("Valid loot table"),T->Validate(Error)); E.DropChance=2.f; T->Entries[0]=E; TestFalse(TEXT("Invalid chance rejected"),T->Validate(Error)); return true; }
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAetherInventoryTypeTest,"AgeOfAether.Items.InventoryTypes",EAutomationTestFlags_ApplicationContextMask|EAutomationTestFlags::ProductFilter)
bool FAetherInventoryTypeTest::RunTest(const FString&){ FAetherInventoryItem I; TestFalse(TEXT("Empty item invalid"),I.IsValid()); I.Snapshot.InstanceID=TEXT("i"); I.Snapshot.DefinitionID=TEXT("d"); I.Snapshot.Quantity=1; TestTrue(TEXT("Item valid"),I.IsValid()); return true; }
