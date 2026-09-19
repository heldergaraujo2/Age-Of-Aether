#include "Economy/AetherEconomySubsystem.h"
#include "Items/AetherItemSubsystem.h"
#include "Engine/GameInstance.h"

bool UAetherEconomySubsystem::RegisterShop(const FAetherShopDefinition& Shop) { return EconomyService.RegisterShop(Shop); }
bool UAetherEconomySubsystem::RegisterRecipe(const FAetherCraftRecipe& Recipe) { return EconomyService.RegisterRecipe(Recipe); }
bool UAetherEconomySubsystem::FindShop(const FString& ShopId, FAetherShopDefinition& OutShop) const { return EconomyService.FindShop(ShopId, OutShop); }
bool UAetherEconomySubsystem::FindRecipe(const FString& RecipeId, FAetherCraftRecipe& OutRecipe) const { return EconomyService.FindRecipe(RecipeId, OutRecipe); }
int64 UAetherEconomySubsystem::GetBalance(const FAetherCharacterId& CharacterId, EAetherCurrency Currency) const { return EconomyService.GetBalance(CharacterId, Currency); }
bool UAetherEconomySubsystem::SetBalance(const FAetherCharacterId& CharacterId, EAetherCurrency Currency, int64 Amount) { return EconomyService.SetBalance(CharacterId, Currency, Amount); }
bool UAetherEconomySubsystem::AddCurrency(const FAetherCharacterId& CharacterId, EAetherCurrency Currency, int64 Amount, FAetherEconomyTransaction& OutTransaction) { return EconomyService.AddCurrency(CharacterId, Currency, Amount, OutTransaction); }
bool UAetherEconomySubsystem::RemoveCurrency(const FAetherCharacterId& CharacterId, EAetherCurrency Currency, int64 Amount, FAetherEconomyTransaction& OutTransaction) { return EconomyService.RemoveCurrency(CharacterId, Currency, Amount, OutTransaction); }
bool UAetherEconomySubsystem::Buy(const FAetherCharacterId& CharacterId, const FString& ShopId, const FAetherItemDefinitionId& ItemDefinitionId, int32 Quantity, FAetherEconomyTransaction& OutTransaction)
{
    UAetherItemSubsystem* Items = GetGameInstance() ? GetGameInstance()->GetSubsystem<UAetherItemSubsystem>() : nullptr;
    if (!Items) { OutTransaction = FAetherEconomyTransaction(); OutTransaction.Result = EAetherEconomyResult::InvalidRequest; return false; }
    return EconomyService.Buy(CharacterId, ShopId, ItemDefinitionId, Quantity, Items->GetItemService(), OutTransaction);
}
bool UAetherEconomySubsystem::Sell(const FAetherCharacterId& CharacterId, const FString& ShopId, const FAetherItemInstanceId& InstanceId, int32 Quantity, FAetherEconomyTransaction& OutTransaction)
{
    UAetherItemSubsystem* Items = GetGameInstance() ? GetGameInstance()->GetSubsystem<UAetherItemSubsystem>() : nullptr;
    if (!Items) { OutTransaction = FAetherEconomyTransaction(); OutTransaction.Result = EAetherEconomyResult::InvalidRequest; return false; }
    return EconomyService.Sell(CharacterId, ShopId, InstanceId, Quantity, Items->GetItemService(), OutTransaction);
}
bool UAetherEconomySubsystem::Craft(const FAetherCharacterId& CharacterId, const FString& RecipeId, int32 Quantity, int32 CharacterLevel, FAetherEconomyTransaction& OutTransaction)
{
    UAetherItemSubsystem* Items = GetGameInstance() ? GetGameInstance()->GetSubsystem<UAetherItemSubsystem>() : nullptr;
    if (!Items) { OutTransaction = FAetherEconomyTransaction(); OutTransaction.Result = EAetherEconomyResult::InvalidRequest; return false; }
    return EconomyService.Craft(CharacterId, RecipeId, Quantity, CharacterLevel, Items->GetItemService(), OutTransaction);
}
