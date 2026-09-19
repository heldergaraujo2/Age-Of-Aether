#pragma once

#include "CoreMinimal.h"
#include "Economy/AetherEconomyTypes.h"

class FAetherItemService;

class FAetherEconomyService
{
public:
    bool RegisterShop(const FAetherShopDefinition& Shop);
    bool RegisterRecipe(const FAetherCraftRecipe& Recipe);
    bool FindShop(const FString& ShopId, FAetherShopDefinition& OutShop) const;
    bool FindRecipe(const FString& RecipeId, FAetherCraftRecipe& OutRecipe) const;

    int64 GetBalance(const FAetherCharacterId& CharacterId, EAetherCurrency Currency) const;
    bool GetWallet(const FAetherCharacterId& CharacterId, FAetherWallet& OutWallet) const;
    bool SetBalance(const FAetherCharacterId& CharacterId, EAetherCurrency Currency, int64 Amount);
    bool RestoreWallet(const FAetherWallet& Wallet);
    bool AddCurrency(const FAetherCharacterId& CharacterId, EAetherCurrency Currency, int64 Amount, FAetherEconomyTransaction& OutTransaction);
    bool RemoveCurrency(const FAetherCharacterId& CharacterId, EAetherCurrency Currency, int64 Amount, FAetherEconomyTransaction& OutTransaction);

    bool Buy(const FAetherCharacterId& CharacterId, const FString& ShopId, const FAetherItemDefinitionId& ItemDefinitionId, int32 Quantity, FAetherItemService& Items, FAetherEconomyTransaction& OutTransaction);
    bool Sell(const FAetherCharacterId& CharacterId, const FString& ShopId, const FAetherItemInstanceId& InstanceId, int32 Quantity, FAetherItemService& Items, FAetherEconomyTransaction& OutTransaction);
    bool Craft(const FAetherCharacterId& CharacterId, const FString& RecipeId, int32 Quantity, int32 CharacterLevel, FAetherItemService& Items, FAetherEconomyTransaction& OutTransaction);

    int32 NumShops() const { return Shops.Num(); }
    int32 NumRecipes() const { return Recipes.Num(); }

private:
    uint64 NextTransactionId = 1;
    TMap<FAetherCharacterId, FAetherWallet> Wallets;
    TMap<FString, FAetherShopDefinition> Shops;
    TMap<FString, FAetherCraftRecipe> Recipes;
    TSet<uint64> ProcessedTransactions;

    static bool IsPositiveQuantity(int32 Quantity);
    static bool ValidateShop(const FAetherShopDefinition& Shop);
    static bool ValidateRecipe(const FAetherCraftRecipe& Recipe);
    static bool CanAddToInventory(const FAetherCharacterId& CharacterId, const FAetherItemDefinitionId& DefinitionId, int32 Quantity, const FAetherItemService& Items);
    static bool ConsumeDefinitions(const FAetherCharacterId& CharacterId, const TArray<FAetherCraftIngredient>& Requirements, int32 Multiplier, FAetherItemService& Items);
    static int32 CountDefinition(const TArray<FAetherInventorySlot>& Inventory, const FAetherItemDefinitionId& DefinitionId);
    static int32 CountDefinitionQuantity(const TArray<FAetherInventorySlot>& Inventory, const FAetherItemDefinitionId& DefinitionId);
    void InitializeTransaction(const FAetherCharacterId& CharacterId, FAetherEconomyTransaction& OutTransaction);
};
