#include "Economy/AetherEconomyService.h"
#include "Items/AetherItemService.h"
#include "Misc/AutomationTest.h"

namespace
{
    FAetherCharacterId TestCharacter(const TCHAR* Name)
    {
        FAetherCharacterId Id;
        Id.Value = Name;
        return Id;
    }

    FAetherItemDefinition TestItem(const TCHAR* Id, int32 MaxStack)
    {
        FAetherItemDefinition Definition;
        Definition.DefinitionId = FAetherItemDefinitionId::FromString(Id);
        Definition.InternalName = Id;
        Definition.DisplayName = Id;
        Definition.MaxStack = MaxStack;
        return Definition;
    }
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAetherEconomyRegistrationTest, "AgeOfAether.Economy.Registration",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::EngineFilter)

bool FAetherEconomyRegistrationTest::RunTest(const FString&)
{
    FAetherEconomyService Economy;
    FAetherShopDefinition Shop;
    Shop.ShopId = TEXT("General");
    FAetherShopEntry Entry;
    Entry.ItemDefinitionId = FAetherItemDefinitionId::FromString(TEXT("Potion"));
    Entry.BuyPrice = 10;
    Entry.SellPrice = 5;
    Shop.Entries.Add(Entry);

    FAetherCraftRecipe Recipe;
    Recipe.RecipeId = TEXT("PotionRecipe");
    FAetherCraftIngredient Input;
    Input.ItemDefinitionId = FAetherItemDefinitionId::FromString(TEXT("Herb"));
    Input.Quantity = 2;
    Recipe.Ingredients.Add(Input);
    FAetherCraftIngredient Output;
    Output.ItemDefinitionId = FAetherItemDefinitionId::FromString(TEXT("Potion"));
    Output.Quantity = 1;
    Recipe.Outputs.Add(Output);

    TestTrue(TEXT("shop registers"), Economy.RegisterShop(Shop));
    TestFalse(TEXT("duplicate shop rejected"), Economy.RegisterShop(Shop));
    TestTrue(TEXT("recipe registers"), Economy.RegisterRecipe(Recipe));
    TestFalse(TEXT("duplicate recipe rejected"), Economy.RegisterRecipe(Recipe));
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAetherEconomyWalletTest, "AgeOfAether.Economy.Wallet",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::EngineFilter)

bool FAetherEconomyWalletTest::RunTest(const FString&)
{
    FAetherEconomyService Economy;
    const FAetherCharacterId Character = TestCharacter(TEXT("wallet"));
    TestTrue(TEXT("set balance"), Economy.SetBalance(Character, EAetherCurrency::Gold, 100));
    FAetherEconomyTransaction Tx;
    TestTrue(TEXT("remove funds"), Economy.RemoveCurrency(Character, EAetherCurrency::Gold, 40, Tx));
    TestEqual(TEXT("remaining funds"), Economy.GetBalance(Character, EAetherCurrency::Gold), int64(60));
    TestFalse(TEXT("overspend rejected"), Economy.RemoveCurrency(Character, EAetherCurrency::Gold, 61, Tx));
    TestEqual(TEXT("overspend leaves balance"), Economy.GetBalance(Character, EAetherCurrency::Gold), int64(60));
    TestFalse(TEXT("negative balance rejected"), Economy.SetBalance(Character, EAetherCurrency::Gold, -1));
    TestTrue(TEXT("set maximum balance"), Economy.SetBalance(Character, EAetherCurrency::Gold, MAX_int64));
    TestFalse(TEXT("currency overflow rejected"), Economy.AddCurrency(Character, EAetherCurrency::Gold, 1, Tx));
    TestEqual(TEXT("overflow leaves balance"), Economy.GetBalance(Character, EAetherCurrency::Gold), MAX_int64);
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAetherEconomyBuySellTest, "AgeOfAether.Economy.BuySell",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::EngineFilter)

bool FAetherEconomyBuySellTest::RunTest(const FString&)
{
    FAetherEconomyService Economy;
    FAetherItemService Items;
    const FAetherCharacterId Character = TestCharacter(TEXT("merchant"));

    Items.RegisterDefinition(TestItem(TEXT("Potion"), 20));

    FAetherShopDefinition Shop;
    Shop.ShopId = TEXT("General");
    FAetherShopEntry Entry;
    Entry.ItemDefinitionId = FAetherItemDefinitionId::FromString(TEXT("Potion"));
    Entry.BuyPrice = 10;
    Entry.SellPrice = 5;
    Entry.MaxQuantityPerTransaction = 10;
    Shop.Entries.Add(Entry);
    TestTrue(TEXT("shop registers"), Economy.RegisterShop(Shop));
    TestTrue(TEXT("seed gold"), Economy.SetBalance(Character, EAetherCurrency::Gold, 100));

    FAetherEconomyTransaction Tx;
    TestTrue(TEXT("buy succeeds"), Economy.Buy(Character, TEXT("General"), Entry.ItemDefinitionId, 3, Items, Tx));
    TestEqual(TEXT("gold after buy"), Economy.GetBalance(Character, EAetherCurrency::Gold), int64(70));
    TArray<FAetherInventorySlot> Inventory;
    Items.GetInventory(Character, Inventory);
    int32 PotionCount = 0;
    for (const FAetherInventorySlot& Slot : Inventory)
        if (Slot.IsOccupied() && Slot.Item.DefinitionId == Entry.ItemDefinitionId) PotionCount += Slot.Item.Quantity;
    TestEqual(TEXT("three potions exist"), PotionCount, 3);

    Items.GetInventory(Character, Inventory);
    FAetherItemInstanceId Instance;
    for (const FAetherInventorySlot& Slot : Inventory) if (Slot.IsOccupied()) { Instance = Slot.Item.InstanceId; break; }

    TestTrue(TEXT("sell succeeds"), Economy.Sell(Character, TEXT("General"), Instance, 2, Items, Tx));
    TestEqual(TEXT("gold after sell"), Economy.GetBalance(Character, EAetherCurrency::Gold), int64(80));
    TestFalse(TEXT("oversell rejected"), Economy.Sell(Character, TEXT("General"), Instance, 5, Items, Tx));
    TestEqual(TEXT("gold unchanged after rejected sell"), Economy.GetBalance(Character, EAetherCurrency::Gold), int64(80));
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAetherEconomyCraftingTest, "AgeOfAether.Economy.Crafting",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::EngineFilter)

bool FAetherEconomyCraftingTest::RunTest(const FString&)
{
    FAetherEconomyService Economy;
    FAetherItemService Items;
    const FAetherCharacterId Character = TestCharacter(TEXT("crafter"));
    const FAetherItemDefinition Herb = TestItem(TEXT("Herb"), 20);
    const FAetherItemDefinition Potion = TestItem(TEXT("Potion"), 20);
    TestTrue(TEXT("herb definition"), Items.RegisterDefinition(Herb));
    TestTrue(TEXT("potion definition"), Items.RegisterDefinition(Potion));

    TArray<FAetherInventorySlot> Inventory;
    TestTrue(TEXT("add herbs"), Items.AddItem(Character, Herb.DefinitionId, 4, Inventory));

    FAetherCraftRecipe Recipe;
    Recipe.RecipeId = TEXT("PotionRecipe");
    FAetherCraftIngredient Input;
    Input.ItemDefinitionId = Herb.DefinitionId;
    Input.Quantity = 2;
    Recipe.Ingredients.Add(Input);
    FAetherCraftIngredient Output;
    Output.ItemDefinitionId = Potion.DefinitionId;
    Output.Quantity = 1;
    Recipe.Outputs.Add(Output);
    Recipe.CurrencyCost = 5;
    TestTrue(TEXT("recipe registers"), Economy.RegisterRecipe(Recipe));
    TestTrue(TEXT("seed crafting currency"), Economy.SetBalance(Character, EAetherCurrency::Gold, 10));

    FAetherEconomyTransaction Tx;
    TestTrue(TEXT("craft succeeds"), Economy.Craft(Character, Recipe.RecipeId, 2, 10, Items, Tx));
    TestEqual(TEXT("craft currency consumed"), Economy.GetBalance(Character, EAetherCurrency::Gold), int64(0));

    Items.GetInventory(Character, Inventory);
    int32 HerbCount = 0;
    int32 PotionCount = 0;
    for (const FAetherInventorySlot& Slot : Inventory)
    {
        if (!Slot.IsOccupied()) continue;
        if (Slot.Item.DefinitionId == Herb.DefinitionId) HerbCount += Slot.Item.Quantity;
        if (Slot.Item.DefinitionId == Potion.DefinitionId) PotionCount += Slot.Item.Quantity;
    }
    TestEqual(TEXT("ingredients consumed"), HerbCount, 0);
    TestEqual(TEXT("outputs created"), PotionCount, 2);

    TestFalse(TEXT("missing ingredients reject"), Economy.Craft(Character, Recipe.RecipeId, 1, 10, Items, Tx));
    TestEqual(TEXT("failed craft does not create output"), PotionCount, 2);
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAetherEconomyValidationTest, "AgeOfAether.Economy.Validation",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::EngineFilter)

bool FAetherEconomyValidationTest::RunTest(const FString&)
{
    FAetherEconomyService Economy;
    const FAetherCharacterId Character = TestCharacter(TEXT("validation"));
    FAetherEconomyTransaction Tx;

    TestFalse(TEXT("zero currency rejected"), Economy.AddCurrency(Character, EAetherCurrency::Gold, 0, Tx));
    TestFalse(TEXT("negative currency rejected"), Economy.AddCurrency(Character, EAetherCurrency::Gold, -1, Tx));
    FAetherItemService Items;
    TestFalse(TEXT("missing recipe rejected"), Economy.Craft(Character, TEXT("missing"), 1, 1, Items, Tx));
    TestEqual(TEXT("invalid transaction result"), Tx.Result, EAetherEconomyResult::RecipeNotFound);
    return true;
}


IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAetherEconomyCraftingCapacityAtomicityTest, "AgeOfAether.Economy.CraftingCapacityAtomicity",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::EngineFilter)

bool FAetherEconomyCraftingCapacityAtomicityTest::RunTest(const FString&)
{
    FAetherEconomyService Economy;
    FAetherItemService Items;
    const FAetherCharacterId Character = TestCharacter(TEXT("full-crafter"));
    const FAetherItemDefinition Filler = TestItem(TEXT("Filler"), 1);
    const FAetherItemDefinition Herb = TestItem(TEXT("Herb"), 20);
    const FAetherItemDefinition Potion = TestItem(TEXT("Potion"), 20);
    TestTrue(TEXT("filler definition"), Items.RegisterDefinition(Filler));
    TestTrue(TEXT("herb definition"), Items.RegisterDefinition(Herb));
    TestTrue(TEXT("potion definition"), Items.RegisterDefinition(Potion));

    TArray<FAetherInventorySlot> Inventory;
    TestTrue(TEXT("fill 63 slots"), Items.AddItem(Character, Filler.DefinitionId, 63, Inventory));
    TestTrue(TEXT("place ingredient in last slot"), Items.AddItem(Character, Herb.DefinitionId, 1, Inventory));

    FAetherCraftRecipe Recipe;
    Recipe.RecipeId = TEXT("BlockedRecipe");
    FAetherCraftIngredient Input;
    Input.ItemDefinitionId = Herb.DefinitionId;
    Input.Quantity = 1;
    Recipe.Ingredients.Add(Input);
    FAetherCraftIngredient Output;
    Output.ItemDefinitionId = Potion.DefinitionId;
    Output.Quantity = 1;
    Recipe.Outputs.Add(Output);
    TestTrue(TEXT("blocked recipe registers"), Economy.RegisterRecipe(Recipe));

    FAetherEconomyTransaction Tx;
    TestFalse(TEXT("full inventory rejects craft"), Economy.Craft(Character, Recipe.RecipeId, 1, 10, Items, Tx));
    TestEqual(TEXT("capacity result"), Tx.Result, EAetherEconomyResult::InventoryFull);

    Items.GetInventory(Character, Inventory);
    int32 HerbCount = 0;
    for (const FAetherInventorySlot& Slot : Inventory)
        if (Slot.IsOccupied() && Slot.Item.DefinitionId == Herb.DefinitionId) HerbCount += Slot.Item.Quantity;
    TestEqual(TEXT("failed craft preserves ingredients"), HerbCount, 1);
    return true;
}
