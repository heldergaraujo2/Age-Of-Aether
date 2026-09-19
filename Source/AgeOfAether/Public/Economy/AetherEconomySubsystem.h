#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Economy/AetherEconomyService.h"
#include "Economy/AetherEconomySubsystem.generated.h"

UCLASS()
class AGEOFAETHER_API UAetherEconomySubsystem : public UGameInstanceSubsystem
{
    GENERATED_BODY()

public:
    bool RegisterShop(const FAetherShopDefinition& Shop);
    bool RegisterRecipe(const FAetherCraftRecipe& Recipe);
    bool FindShop(const FString& ShopId, FAetherShopDefinition& OutShop) const;
    bool FindRecipe(const FString& RecipeId, FAetherCraftRecipe& OutRecipe) const;

    int64 GetBalance(const FAetherCharacterId& CharacterId, EAetherCurrency Currency) const;
    bool SetBalance(const FAetherCharacterId& CharacterId, EAetherCurrency Currency, int64 Amount);
    bool AddCurrency(const FAetherCharacterId& CharacterId, EAetherCurrency Currency, int64 Amount, FAetherEconomyTransaction& OutTransaction);
    bool RemoveCurrency(const FAetherCharacterId& CharacterId, EAetherCurrency Currency, int64 Amount, FAetherEconomyTransaction& OutTransaction);

    bool Buy(const FAetherCharacterId& CharacterId, const FString& ShopId, const FAetherItemDefinitionId& ItemDefinitionId, int32 Quantity, FAetherEconomyTransaction& OutTransaction);
    bool Sell(const FAetherCharacterId& CharacterId, const FString& ShopId, const FAetherItemInstanceId& InstanceId, int32 Quantity, FAetherEconomyTransaction& OutTransaction);
    bool Craft(const FAetherCharacterId& CharacterId, const FString& RecipeId, int32 Quantity, int32 CharacterLevel, FAetherEconomyTransaction& OutTransaction);

private:
    FAetherEconomyService EconomyService;
};
