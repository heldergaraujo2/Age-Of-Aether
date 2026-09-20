#include "Economy/AetherEconomyRegistry.h"
FString FAetherEconomyRegistry::N(const FString& V){return V.TrimStartAndEnd().ToLower();}
bool FAetherEconomyRegistry::RegisterRecipe(const FAetherRecipeDefinition& D){if(!D.IsValid())return false;FString K=N(D.RecipeID);if(K.IsEmpty()||Recipes.Contains(K))return false;auto C=D;C.RecipeID=K;Recipes.Add(K,MoveTemp(C));return true;}
bool FAetherEconomyRegistry::RegisterShop(const FAetherShopDefinition& D){if(!D.IsValid())return false;FString K=N(D.ShopID);if(K.IsEmpty()||Shops.Contains(K))return false;auto C=D;C.ShopID=K;Shops.Add(K,MoveTemp(C));return true;}
const FAetherRecipeDefinition* FAetherEconomyRegistry::FindRecipe(const FString& ID)const{return Recipes.Find(N(ID));}
const FAetherShopDefinition* FAetherEconomyRegistry::FindShop(const FString& ID)const{return Shops.Find(N(ID));}
bool FAetherEconomyRegistry::Validate(TArray<FString>& Errors)const{Errors.Reset();for(auto& P:Recipes)if(!P.Value.IsValid())Errors.Add(TEXT("Invalid recipe: ")+P.Key);for(auto& P:Shops){if(!P.Value.IsValid())Errors.Add(TEXT("Invalid shop: ")+P.Key);for(auto& E:P.Value.Entries)if(!E.IsValid())Errors.Add(TEXT("Invalid shop entry: ")+P.Key);}return Errors.IsEmpty();}
void FAetherEconomyRegistry::Reset(){Recipes.Reset();Shops.Reset();}
