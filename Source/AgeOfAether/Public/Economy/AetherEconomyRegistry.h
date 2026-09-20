#pragma once
#include "CoreMinimal.h"
#include "Economy/AetherEconomyTypes.h"
class FAetherEconomyRegistry
{
public:
 bool RegisterRecipe(const FAetherRecipeDefinition& D);
 bool RegisterShop(const FAetherShopDefinition& D);
 const FAetherRecipeDefinition* FindRecipe(const FString& ID) const;
 const FAetherShopDefinition* FindShop(const FString& ID) const;
 bool Validate(TArray<FString>& Errors) const;
 void Reset();
private:
 static FString N(const FString& V);
 TMap<FString,FAetherRecipeDefinition> Recipes;
 TMap<FString,FAetherShopDefinition> Shops;
};