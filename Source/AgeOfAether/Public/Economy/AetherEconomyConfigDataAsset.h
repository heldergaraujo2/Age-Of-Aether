#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Economy/AetherEconomyTypes.h"
#include "AetherEconomyConfigDataAsset.generated.h"

UCLASS(BlueprintType)
class AGEOFAETHER_API UAetherEconomyConfigDataAsset : public UDataAsset
{
    GENERATED_BODY()

public:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Economy")
    TArray<FAetherShopDefinition> Shops;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Crafting")
    TArray<FAetherCraftRecipe> Recipes;
};
