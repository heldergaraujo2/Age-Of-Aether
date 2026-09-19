#if WITH_DEV_AUTOMATION_TESTS

#include "Misc/AutomationTest.h"
#include "Items/AetherItemService.h"

namespace
{
    FAetherCharacterId MakeCharacterId()
    {
        return FAetherCharacterId::NewId();
    }

    FAetherItemDefinition MakeDefinition(const TCHAR* Id, int32 MaxStack)
    {
        FAetherItemDefinition Definition;
        Definition.DefinitionId = FAetherItemDefinitionId::FromString(Id);
        Definition.InternalName = Id;
        Definition.DisplayName = Id;
        Definition.MaxStack = MaxStack;
        Definition.BaseLevel = 1;
        Definition.BaseDurability = 100;
        return Definition;
    }
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FAetherItemDefinitionTest,
    "AgeOfAether.Item.Definition",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FAetherItemDefinitionTest::RunTest(const FString& Parameters)
{
    FAetherItemService Service;
    const FAetherItemDefinition Definition = MakeDefinition(TEXT("potion.health"), 20);

    TestTrue(TEXT("Definition registration succeeds"), Service.RegisterDefinition(Definition));
    TestFalse(TEXT("Duplicate definition is rejected"), Service.RegisterDefinition(Definition));
    TestEqual(TEXT("Definition count is one"), Service.NumDefinitions(), 1);

    FAetherItemDefinition Found;
    TestTrue(TEXT("Definition can be read"), Service.FindDefinition(Definition.DefinitionId, Found));
    TestEqual(TEXT("Definition identity is preserved"), Found.DefinitionId.Value, FString(TEXT("potion.health")));
    TestEqual(TEXT("Definition max stack is preserved"), Found.MaxStack, 20);
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FAetherItemAddStackTest,
    "AgeOfAether.Item.InventoryAddAndStack",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FAetherItemAddStackTest::RunTest(const FString& Parameters)
{
    FAetherItemService Service;
    Service.RegisterDefinition(MakeDefinition(TEXT("potion.health"), 20));

    const FAetherCharacterId CharacterId = MakeCharacterId();
    TArray<FAetherInventorySlot> Inventory;

    TestTrue(TEXT("First grant succeeds"), Service.AddItem(CharacterId, FAetherItemDefinitionId::FromString(TEXT("potion.health")), 15, Inventory));
    TestEqual(TEXT("One occupied slot after first grant"), Service.NumOccupiedSlots(CharacterId), 1);
    const FAetherItemInstanceId FirstId = Inventory[0].Item.InstanceId;

    TestTrue(TEXT("Second grant stacks"), Service.AddItem(CharacterId, FAetherItemDefinitionId::FromString(TEXT("potion.health")), 10, Inventory));
    TestEqual(TEXT("Two occupied slots after exceeding one stack"), Service.NumOccupiedSlots(CharacterId), 2);
    TestEqual(TEXT("First stack reaches max"), Inventory[0].Item.Quantity, 20);
    TestEqual(TEXT("Second stack receives remainder"), Inventory[1].Item.Quantity, 5);
    TestEqual(TEXT("Existing stack keeps instance identity"), Inventory[0].Item.InstanceId.Value, FirstId.Value);
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FAetherItemMutationTest,
    "AgeOfAether.Item.InventoryMutations",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FAetherItemMutationTest::RunTest(const FString& Parameters)
{
    FAetherItemService Service;
    Service.RegisterDefinition(MakeDefinition(TEXT("material.crystal"), 99));

    const FAetherCharacterId CharacterId = MakeCharacterId();
    TArray<FAetherInventorySlot> Inventory;

    TestTrue(TEXT("Initial grant succeeds"), Service.AddItem(CharacterId, FAetherItemDefinitionId::FromString(TEXT("material.crystal")), 10, Inventory));
    const FAetherItemInstanceId SourceId = Inventory[0].Item.InstanceId;

    TestTrue(TEXT("Stack split succeeds"), Service.SplitStack(CharacterId, SourceId, 4, 1, Inventory));
    TestEqual(TEXT("Source quantity after split"), Inventory[0].Item.Quantity, 6);
    TestEqual(TEXT("Split quantity"), Inventory[1].Item.Quantity, 4);
    TestFalse(TEXT("Split creates a new instance"), Inventory[0].Item.InstanceId == Inventory[1].Item.InstanceId);

    const FAetherItemInstanceId SplitId = Inventory[1].Item.InstanceId;
    TestTrue(TEXT("Stacks merge"), Service.MergeStacks(CharacterId, SplitId, SourceId, Inventory));
    TestEqual(TEXT("Merged source quantity"), Inventory[0].Item.Quantity, 10);
    TestFalse(TEXT("Merged source slot is empty"), Inventory[1].IsOccupied());

    TestTrue(TEXT("Move item succeeds"), Service.MoveItem(CharacterId, SourceId, 5, Inventory));
    TestFalse(TEXT("Original slot is empty after move"), Inventory[0].IsOccupied());
    TestEqual(TEXT("Moved item reaches target"), Inventory[5].Item.InstanceId.Value, SourceId.Value);

    TestTrue(TEXT("Partial removal succeeds"), Service.RemoveItem(CharacterId, SourceId, 3, Inventory));
    TestEqual(TEXT("Partial removal leaves quantity"), Inventory[5].Item.Quantity, 7);
    TestTrue(TEXT("Final removal succeeds"), Service.RemoveItem(CharacterId, SourceId, 7, Inventory));
    TestFalse(TEXT("Final removal empties slot"), Inventory[5].IsOccupied());
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FAetherItemOwnershipIsolationTest,
    "AgeOfAether.Item.CharacterIsolation",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FAetherItemOwnershipIsolationTest::RunTest(const FString& Parameters)
{
    FAetherItemService Service;
    Service.RegisterDefinition(MakeDefinition(TEXT("quest.token"), 10));

    const FAetherCharacterId Owner = MakeCharacterId();
    const FAetherCharacterId Other = MakeCharacterId();
    TArray<FAetherInventorySlot> Inventory;

    TestTrue(TEXT("Owner grant succeeds"), Service.AddItem(Owner, FAetherItemDefinitionId::FromString(TEXT("quest.token")), 2, Inventory));
    const FAetherItemInstanceId ItemId = Inventory[0].Item.InstanceId;

    TestFalse(TEXT("Other character cannot remove owner's item"), Service.RemoveItem(Other, ItemId, 1, Inventory));
    TestFalse(TEXT("Other character cannot move owner's item"), Service.MoveItem(Other, ItemId, 1, Inventory));
    TestFalse(TEXT("Other character cannot split owner's item"), Service.SplitStack(Other, ItemId, 1, 1, Inventory));
    TestEqual(TEXT("Owner still has one occupied slot"), Service.NumOccupiedSlots(Owner), 1);
    TestEqual(TEXT("Other character has no occupied slots"), Service.NumOccupiedSlots(Other), 0);
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FAetherItemTransactionalGrantTest,
    "AgeOfAether.Item.TransactionalGrant",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FAetherItemTransactionalGrantTest::RunTest(const FString& Parameters)
{
    FAetherItemService Service;
    Service.RegisterDefinition(MakeDefinition(TEXT("unique.fragment"), 1));

    const FAetherCharacterId CharacterId = MakeCharacterId();
    TArray<FAetherInventorySlot> Inventory;

    TestFalse(TEXT("Grant beyond inventory capacity is rejected atomically"),
        Service.AddItem(CharacterId, FAetherItemDefinitionId::FromString(TEXT("unique.fragment")), FAetherItemService::MaxInventorySlots + 1, Inventory));

    TestEqual(TEXT("Failed grant leaves no occupied slots"), Service.NumOccupiedSlots(CharacterId), 0);
    TestTrue(TEXT("Inventory still has fixed slot count"), Service.GetInventory(CharacterId, Inventory));
    TestEqual(TEXT("Inventory remains fixed size"), Inventory.Num(), FAetherItemService::MaxInventorySlots);
    return true;
}

#endif
