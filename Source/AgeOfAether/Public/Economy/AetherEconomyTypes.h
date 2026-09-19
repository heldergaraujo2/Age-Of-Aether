#pragma once

#include "CoreMinimal.h"
#include "Characters/AetherCharacterTypes.h"
#include "Items/AetherItemTypes.h"
#include "AetherEconomyTypes.generated.h"

UENUM(BlueprintType)
enum class EAetherCurrency : uint8
{
    Gold
};

UENUM(BlueprintType)
enum class EAetherEconomyResult : uint8
{
    Accepted,
    InvalidRequest,
    CharacterNotFound,
    NotOwned,
    InvalidCurrency,
    InsufficientFunds,
    DefinitionNotFound,
    ShopNotFound,
    ItemUnavailable,
    InvalidQuantity,
    InventoryFull,
    RecipeNotFound,
    MissingIngredients,
    CraftingDisabled,
    DuplicateRequest,
    InvalidPrice,
    Overflow
};

USTRUCT(BlueprintType)
struct FAetherCurrencyBalance
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    EAetherCurrency Currency = EAetherCurrency::Gold;

    UPROPERTY(BlueprintReadOnly)
    int64 Amount = 0;
};

USTRUCT(BlueprintType)
struct FAetherWallet
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    FAetherCharacterId CharacterId;

    UPROPERTY(BlueprintReadOnly)
    TArray<FAetherCurrencyBalance> Balances;
};

USTRUCT(BlueprintType)
struct FAetherShopEntry
{
    GENERATED_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    FAetherItemDefinitionId ItemDefinitionId;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    int64 BuyPrice = 0;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    int64 SellPrice = 0;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    int32 MaxQuantityPerTransaction = 99;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    bool bCanBuy = true;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    bool bCanSell = true;
};

USTRUCT(BlueprintType)
struct FAetherShopDefinition
{
    GENERATED_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    FString ShopId;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    FString DisplayName;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    TArray<FAetherShopEntry> Entries;

    bool IsValid() const { return !ShopId.TrimStartAndEnd().IsEmpty(); }
};

USTRUCT(BlueprintType)
struct FAetherCraftIngredient
{
    GENERATED_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    FAetherItemDefinitionId ItemDefinitionId;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    int32 Quantity = 1;
};

USTRUCT(BlueprintType)
struct FAetherCraftRecipe
{
    GENERATED_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    FString RecipeId;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    TArray<FAetherCraftIngredient> Ingredients;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    TArray<FAetherCraftIngredient> Outputs;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    EAetherCurrency Currency = EAetherCurrency::Gold;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    int64 CurrencyCost = 0;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    int32 RequiredLevel = 1;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    bool bEnabled = true;

    bool IsValid() const { return !RecipeId.TrimStartAndEnd().IsEmpty() && Ingredients.Num() > 0 && Outputs.Num() > 0; }
};

USTRUCT(BlueprintType)
struct FAetherEconomyTransaction
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    uint64 TransactionId = 0;

    UPROPERTY(BlueprintReadOnly)
    FAetherCharacterId CharacterId;

    UPROPERTY(BlueprintReadOnly)
    EAetherEconomyResult Result = EAetherEconomyResult::InvalidRequest;

    UPROPERTY(BlueprintReadOnly)
    EAetherCurrency Currency = EAetherCurrency::Gold;

    UPROPERTY(BlueprintReadOnly)
    int64 Amount = 0;

    UPROPERTY(BlueprintReadOnly)
    int64 BalanceAfter = 0;
};
