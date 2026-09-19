#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Items/AetherItemTypes.h"
#include "AetherItemDefinitionDataAsset.generated.h"

UCLASS(BlueprintType)
class AGEOFAETHER_API UAetherItemDefinitionDataAsset : public UDataAsset
{
    GENERATED_BODY()

public:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item")
    FAetherItemDefinition Definition;
};
