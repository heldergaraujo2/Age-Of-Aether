#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "World/AetherWorldTypes.h"

#include "AetherWorldConfigDataAsset.generated.h"

UCLASS(BlueprintType)
class AGEOFAETHER_API UAetherWorldConfigDataAsset : public UDataAsset
{
    GENERATED_BODY()

public:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "World")
    FAetherWorldConfig Config;
};
