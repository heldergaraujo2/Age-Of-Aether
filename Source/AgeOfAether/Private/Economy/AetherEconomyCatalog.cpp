#include "Economy/AetherEconomyCatalog.h"
bool UAetherEconomyCatalog::IsValid(TArray<FString>& Errors) const
{
 Errors.Reset(); TSet<FString> R,S;
 for(const auto& D:Recipes){if(!D.IsValid())Errors.Add(TEXT("Invalid recipe: ")+D.RecipeID);FString K=D.RecipeID.TrimStartAndEnd().ToLower();if(R.Contains(K))Errors.Add(TEXT("Duplicate recipe: ")+K);R.Add(K);}
 for(const auto& D:Shops){if(!D.IsValid())Errors.Add(TEXT("Invalid shop: ")+D.ShopID);FString K=D.ShopID.TrimStartAndEnd().ToLower();if(S.Contains(K))Errors.Add(TEXT("Duplicate shop: ")+K);S.Add(K);for(const auto& E:D.Entries)if(!E.IsValid())Errors.Add(TEXT("Invalid shop entry: ")+K);}
 return Errors.IsEmpty();
}