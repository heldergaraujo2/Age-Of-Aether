#pragma once
#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Economy/AetherEconomyTypes.h"
#include "Economy/AetherEconomyCatalog.generated.h"
UCLASS(BlueprintType)
class AGEOFAETHER_API UAetherEconomyCatalog : public UDataAsset
{
 GENERATED_BODY()
public:
 UPROPERTY(EditAnywhere, BlueprintReadOnly) TArray<FAetherRecipeDefinition> Recipes;
 UPROPERTY(EditAnywhere, BlueprintReadOnly) TArray<FAetherShopDefinition> Shops;
 bool IsValid(TArray<FString>& Errors) const;
};