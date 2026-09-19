#include "Economy/AetherEconomyService.h"
#include "Items/AetherItemService.h"

namespace
{
    bool CheckedAddInt64(int64 A, int64 B, int64& Out)
    {
        if ((B > 0 && A > MAX_int64 - B) || (B < 0 && A < MIN_int64 - B)) return false;
        Out = A + B;
        return true;
    }
}

bool FAetherEconomyService::ValidateShop(const FAetherShopDefinition& Shop)
{
    if (!Shop.IsValid()) return false;
    TSet<FAetherItemDefinitionId> Seen;
    for (const FAetherShopEntry& Entry : Shop.Entries)
    {
        if (!Entry.ItemDefinitionId.IsValid() || Entry.BuyPrice < 0 || Entry.SellPrice < 0
            || Entry.MaxQuantityPerTransaction <= 0 || Seen.Contains(Entry.ItemDefinitionId))
            return false;
        Seen.Add(Entry.ItemDefinitionId);
    }
    return true;
}

bool FAetherEconomyService::ValidateRecipe(const FAetherCraftRecipe& Recipe)
{
    if (!Recipe.IsValid() || Recipe.CurrencyCost < 0 || Recipe.RequiredLevel < 1) return false;
    TSet<FAetherItemDefinitionId> SeenInputs;
    for (const FAetherCraftIngredient& Ingredient : Recipe.Ingredients)
    {
        if (!Ingredient.ItemDefinitionId.IsValid() || Ingredient.Quantity <= 0) return false;
        SeenInputs.Add(Ingredient.ItemDefinitionId);
    }
    for (const FAetherCraftIngredient& Output : Recipe.Outputs)
    {
        if (!Output.ItemDefinitionId.IsValid() || Output.Quantity <= 0) return false;
    }
    return SeenInputs.Num() > 0;
}

bool FAetherEconomyService::RegisterShop(const FAetherShopDefinition& Shop)
{
    if (!ValidateShop(Shop) || Shops.Contains(Shop.ShopId.TrimStartAndEnd())) return false;
    FAetherShopDefinition Copy = Shop;
    Copy.ShopId = Copy.ShopId.TrimStartAndEnd();
    Shops.Add(Copy.ShopId, Copy);
    return true;
}

bool FAetherEconomyService::RegisterRecipe(const FAetherCraftRecipe& Recipe)
{
    if (!ValidateRecipe(Recipe) || Recipes.Contains(Recipe.RecipeId.TrimStartAndEnd())) return false;
    FAetherCraftRecipe Copy = Recipe;
    Copy.RecipeId = Copy.RecipeId.TrimStartAndEnd();
    Recipes.Add(Copy.RecipeId, Copy);
    return true;
}

bool FAetherEconomyService::FindShop(const FString& ShopId, FAetherShopDefinition& OutShop) const
{
    const FAetherShopDefinition* Found = Shops.Find(ShopId.TrimStartAndEnd());
    if (!Found) return false;
    OutShop = *Found;
    return true;
}

bool FAetherEconomyService::FindRecipe(const FString& RecipeId, FAetherCraftRecipe& OutRecipe) const
{
    const FAetherCraftRecipe* Found = Recipes.Find(RecipeId.TrimStartAndEnd());
    if (!Found) return false;
    OutRecipe = *Found;
    return true;
}

void FAetherEconomyService::InitializeTransaction(const FAetherCharacterId& CharacterId, FAetherEconomyTransaction& OutTransaction)
{
    OutTransaction = FAetherEconomyTransaction();
    OutTransaction.TransactionId = NextTransactionId++;
    OutTransaction.CharacterId = CharacterId;
}

int64 FAetherEconomyService::GetBalance(const FAetherCharacterId& CharacterId, EAetherCurrency Currency) const
{
    const FAetherWallet* Wallet = Wallets.Find(CharacterId);
    if (!Wallet) return 0;
    for (const FAetherCurrencyBalance& Balance : Wallet->Balances)
        if (Balance.Currency == Currency) return Balance.Amount;
    return 0;
}

bool FAetherEconomyService::SetBalance(const FAetherCharacterId& CharacterId, EAetherCurrency Currency, int64 Amount)
{
    if (!CharacterId.IsValid() || Amount < 0) return false;
    FAetherWallet& Wallet = Wallets.FindOrAdd(CharacterId);
    Wallet.CharacterId = CharacterId;
    for (FAetherCurrencyBalance& Balance : Wallet.Balances)
    {
        if (Balance.Currency == Currency) { Balance.Amount = Amount; return true; }
    }
    FAetherCurrencyBalance NewBalance;
    NewBalance.Currency = Currency;
    NewBalance.Amount = Amount;
    Wallet.Balances.Add(NewBalance);
    return true;
}

bool FAetherEconomyService::AddCurrency(const FAetherCharacterId& CharacterId, EAetherCurrency Currency, int64 Amount, FAetherEconomyTransaction& OutTransaction)
{
    InitializeTransaction(CharacterId, OutTransaction);
    if (!CharacterId.IsValid() || Amount <= 0) { OutTransaction.Result = EAetherEconomyResult::InvalidRequest; return false; }
    const int64 Current = GetBalance(CharacterId, Currency);
    int64 NewAmount = 0;
    if (!CheckedAddInt64(Current, Amount, NewAmount)) { OutTransaction.Result = EAetherEconomyResult::Overflow; return false; }
    SetBalance(CharacterId, Currency, NewAmount);
    OutTransaction.Result = EAetherEconomyResult::Accepted;
    OutTransaction.Currency = Currency;
    OutTransaction.Amount = Amount;
    OutTransaction.BalanceAfter = NewAmount;
    return true;
}

bool FAetherEconomyService::RemoveCurrency(const FAetherCharacterId& CharacterId, EAetherCurrency Currency, int64 Amount, FAetherEconomyTransaction& OutTransaction)
{
    InitializeTransaction(CharacterId, OutTransaction);
    if (!CharacterId.IsValid() || Amount <= 0) { OutTransaction.Result = EAetherEconomyResult::InvalidRequest; return false; }
    const int64 Current = GetBalance(CharacterId, Currency);
    if (Current < Amount) { OutTransaction.Result = EAetherEconomyResult::InsufficientFunds; return false; }
    const int64 NewAmount = Current - Amount;
    SetBalance(CharacterId, Currency, NewAmount);
    OutTransaction.Result = EAetherEconomyResult::Accepted;
    OutTransaction.Currency = Currency;
    OutTransaction.Amount = -Amount;
    OutTransaction.BalanceAfter = NewAmount;
    return true;
}

int32 FAetherEconomyService::CountDefinition(const TArray<FAetherInventorySlot>& Inventory, const FAetherItemDefinitionId& DefinitionId)
{
    int32 Count = 0;
    for (const FAetherInventorySlot& Slot : Inventory)
        if (Slot.IsOccupied() && Slot.Item.DefinitionId == DefinitionId) ++Count;
    return Count;
}

int32 FAetherEconomyService::CountDefinitionQuantity(const TArray<FAetherInventorySlot>& Inventory, const FAetherItemDefinitionId& DefinitionId)
{
    int32 Count = 0;
    for (const FAetherInventorySlot& Slot : Inventory)
        if (Slot.IsOccupied() && Slot.Item.DefinitionId == DefinitionId) Count += Slot.Item.Quantity;
    return Count;
}

bool FAetherEconomyService::CanAddToInventory(const FAetherCharacterId& CharacterId, const FAetherItemDefinitionId& DefinitionId, int32 Quantity, const FAetherItemService& Items)
{
    FAetherItemDefinition Definition;
    if (!Items.FindDefinition(DefinitionId, Definition)) return false;
    TArray<FAetherInventorySlot> Working;
    if (!Items.GetInventory(CharacterId, Working)) return false;
    int32 Remaining = Quantity;
    for (const FAetherInventorySlot& Slot : Working)
        if (Slot.IsOccupied() && Slot.Item.DefinitionId == DefinitionId)
            Remaining -= FMath::Max(0, Definition.MaxStack - Slot.Item.Quantity);
    if (Remaining <= 0) return true;
    int32 EmptySlots = 0;
    for (const FAetherInventorySlot& Slot : Working) EmptySlots += Slot.IsOccupied() ? 0 : 1;
    return FMath::DivideAndRoundUp(Remaining, Definition.MaxStack) <= EmptySlots;
}

bool FAetherEconomyService::ConsumeDefinitions(const FAetherCharacterId& CharacterId, const TArray<FAetherCraftIngredient>& Requirements, int32 Multiplier, FAetherItemService& Items)
{
    for (const FAetherCraftIngredient& Requirement : Requirements)
    {
        int64 Needed64 = static_cast<int64>(Requirement.Quantity) * Multiplier;
        if (Needed64 > MAX_int32) return false;
        TArray<FAetherInventorySlot> Inventory;
        if (!Items.GetInventory(CharacterId, Inventory)
            || CountDefinitionQuantity(Inventory, Requirement.ItemDefinitionId) < static_cast<int32>(Needed64))
            return false;
    }
    for (const FAetherCraftIngredient& Requirement : Requirements)
    {
        int32 Remaining = Requirement.Quantity * Multiplier;
        TArray<FAetherInventorySlot> Inventory;
        Items.GetInventory(CharacterId, Inventory);
        for (const FAetherInventorySlot& Slot : Inventory)
        {
            if (Remaining <= 0) break;
            if (!Slot.IsOccupied() || Slot.Item.DefinitionId != Requirement.ItemDefinitionId) continue;
            const int32 Remove = FMath::Min(Remaining, Slot.Item.Quantity);
            TArray<FAetherInventorySlot> Updated;
            if (!Items.RemoveItem(CharacterId, Slot.Item.InstanceId, Remove, Updated)) return false;
            Remaining -= Remove;
        }
    }
    return true;
}

bool FAetherEconomyService::Buy(const FAetherCharacterId& CharacterId, const FString& ShopId, const FAetherItemDefinitionId& ItemDefinitionId, int32 Quantity, FAetherItemService& Items, FAetherEconomyTransaction& OutTransaction)
{
    InitializeTransaction(CharacterId, OutTransaction);
    FAetherShopDefinition Shop;
    if (!FindShop(ShopId, Shop)) { OutTransaction.Result = EAetherEconomyResult::ShopNotFound; return false; }
    const FAetherShopEntry* Entry = nullptr;
    for (const FAetherShopEntry& Candidate : Shop.Entries) if (Candidate.ItemDefinitionId == ItemDefinitionId) { Entry = &Candidate; break; }
    if (!Entry || !Entry->bCanBuy) { OutTransaction.Result = EAetherEconomyResult::ItemUnavailable; return false; }
    if (!IsPositiveQuantity(Quantity) || Quantity > Entry->MaxQuantityPerTransaction || Entry->BuyPrice <= 0) { OutTransaction.Result = EAetherEconomyResult::InvalidQuantity; return false; }
    FAetherItemDefinition Definition;
    if (!Items.FindDefinition(ItemDefinitionId, Definition)) { OutTransaction.Result = EAetherEconomyResult::DefinitionNotFound; return false; }
    if (!CanAddToInventory(CharacterId, ItemDefinitionId, Quantity, Items)) { OutTransaction.Result = EAetherEconomyResult::InventoryFull; return false; }
    int64 Total = 0;
    if (Quantity > 0 && Entry->BuyPrice > MAX_int64 / Quantity) { OutTransaction.Result = EAetherEconomyResult::Overflow; return false; }
    Total = Entry->BuyPrice * Quantity;
    if (GetBalance(CharacterId, EAetherCurrency::Gold) < Total) { OutTransaction.Result = EAetherEconomyResult::InsufficientFunds; return false; }
    TArray<FAetherInventorySlot> Updated;
    if (!Items.AddItem(CharacterId, ItemDefinitionId, Quantity, Updated)) { OutTransaction.Result = EAetherEconomyResult::InventoryFull; return false; }
    SetBalance(CharacterId, EAetherCurrency::Gold, GetBalance(CharacterId, EAetherCurrency::Gold) - Total);
    OutTransaction.Result = EAetherEconomyResult::Accepted;
    OutTransaction.Currency = EAetherCurrency::Gold;
    OutTransaction.Amount = -Total;
    OutTransaction.BalanceAfter = GetBalance(CharacterId, EAetherCurrency::Gold);
    return true;
}

bool FAetherEconomyService::Sell(const FAetherCharacterId& CharacterId, const FString& ShopId, const FAetherItemInstanceId& InstanceId, int32 Quantity, FAetherItemService& Items, FAetherEconomyTransaction& OutTransaction)
{
    InitializeTransaction(CharacterId, OutTransaction);
    FAetherShopDefinition Shop;
    if (!FindShop(ShopId, Shop)) { OutTransaction.Result = EAetherEconomyResult::ShopNotFound; return false; }
    FAetherInventorySlot FoundSlot;
    TArray<FAetherInventorySlot> Inventory;
    if (!Items.GetInventory(CharacterId, Inventory)) { OutTransaction.Result = EAetherEconomyResult::CharacterNotFound; return false; }
    const FAetherShopEntry* Entry = nullptr;
    FAetherItemDefinitionId DefinitionId;
    for (const FAetherInventorySlot& Slot : Inventory)
        if (Slot.IsOccupied() && Slot.Item.InstanceId == InstanceId) { DefinitionId = Slot.Item.DefinitionId; FoundSlot = Slot; break; }
    if (!DefinitionId.IsValid()) { OutTransaction.Result = EAetherEconomyResult::DefinitionNotFound; return false; }
    for (const FAetherShopEntry& Candidate : Shop.Entries) if (Candidate.ItemDefinitionId == DefinitionId) { Entry = &Candidate; break; }
    if (!Entry || !Entry->bCanSell || Entry->SellPrice <= 0) { OutTransaction.Result = EAetherEconomyResult::ItemUnavailable; return false; }
    if (!IsPositiveQuantity(Quantity) || Quantity > Entry->MaxQuantityPerTransaction || Quantity > FoundSlot.Item.Quantity) { OutTransaction.Result = EAetherEconomyResult::InvalidQuantity; return false; }
    if (Entry->SellPrice > MAX_int64 / Quantity) { OutTransaction.Result = EAetherEconomyResult::Overflow; return false; }
    const int64 Total = Entry->SellPrice * Quantity;
    TArray<FAetherInventorySlot> Updated;
    if (!Items.RemoveItem(CharacterId, InstanceId, Quantity, Updated)) { OutTransaction.Result = EAetherEconomyResult::InvalidRequest; return false; }
    int64 NewBalance = 0;
    if (!CheckedAddInt64(GetBalance(CharacterId, EAetherCurrency::Gold), Total, NewBalance)) return false;
    SetBalance(CharacterId, EAetherCurrency::Gold, NewBalance);
    OutTransaction.Result = EAetherEconomyResult::Accepted;
    OutTransaction.Currency = EAetherCurrency::Gold;
    OutTransaction.Amount = Total;
    OutTransaction.BalanceAfter = NewBalance;
    return true;
}

bool FAetherEconomyService::Craft(const FAetherCharacterId& CharacterId, const FString& RecipeId, int32 Quantity, int32 CharacterLevel, FAetherItemService& Items, FAetherEconomyTransaction& OutTransaction)
{
    InitializeTransaction(CharacterId, OutTransaction);
    FAetherCraftRecipe Recipe;
    if (!FindRecipe(RecipeId, Recipe)) { OutTransaction.Result = EAetherEconomyResult::RecipeNotFound; return false; }
    if (!Recipe.bEnabled) { OutTransaction.Result = EAetherEconomyResult::CraftingDisabled; return false; }
    if (!IsPositiveQuantity(Quantity) || Quantity > 99 || CharacterLevel < Recipe.RequiredLevel) { OutTransaction.Result = EAetherEconomyResult::InvalidQuantity; return false; }
    for (const FAetherCraftIngredient& Output : Recipe.Outputs)
    {
        const int64 Needed = static_cast<int64>(Output.Quantity) * Quantity;
        if (Needed > MAX_int32 || !CanAddToInventory(CharacterId, Output.ItemDefinitionId, static_cast<int32>(Needed), Items))
        {
            OutTransaction.Result = EAetherEconomyResult::InventoryFull;
            return false;
        }
    }
    for (const FAetherCraftIngredient& Ingredient : Recipe.Ingredients)
    {
        const int64 Needed = static_cast<int64>(Ingredient.Quantity) * Quantity;
        TArray<FAetherInventorySlot> Inventory;
        if (Needed > MAX_int32 || !Items.GetInventory(CharacterId, Inventory)
            || CountDefinitionQuantity(Inventory, Ingredient.ItemDefinitionId) < Needed)
        {
            OutTransaction.Result = EAetherEconomyResult::MissingIngredients;
            return false;
        }
    }
    if (Recipe.CurrencyCost > 0)
    {
        if (Quantity > MAX_int64 / Recipe.CurrencyCost || GetBalance(CharacterId, Recipe.Currency) < Recipe.CurrencyCost * Quantity)
        {
            OutTransaction.Result = EAetherEconomyResult::InsufficientFunds;
            return false;
        }
    }
    if (!ConsumeDefinitions(CharacterId, Recipe.Ingredients, Quantity, Items))
    {
        OutTransaction.Result = EAetherEconomyResult::MissingIngredients;
        return false;
    }
    for (const FAetherCraftIngredient& Output : Recipe.Outputs)
    {
        const int64 Needed = static_cast<int64>(Output.Quantity) * Quantity;
        TArray<FAetherInventorySlot> Updated;
        if (!Items.AddItem(CharacterId, Output.ItemDefinitionId, static_cast<int32>(Needed), Updated))
        {
            OutTransaction.Result = EAetherEconomyResult::InventoryFull;
            return false;
        }
    }
    const int64 TotalCost = Recipe.CurrencyCost * Quantity;
    if (TotalCost > 0) SetBalance(CharacterId, Recipe.Currency, GetBalance(CharacterId, Recipe.Currency) - TotalCost);
    OutTransaction.Result = EAetherEconomyResult::Accepted;
    OutTransaction.Currency = Recipe.Currency;
    OutTransaction.Amount = -TotalCost;
    OutTransaction.BalanceAfter = GetBalance(CharacterId, Recipe.Currency);
    return true;
}

bool FAetherEconomyService::IsPositiveQuantity(int32 Quantity)
{
    return Quantity > 0;
}
