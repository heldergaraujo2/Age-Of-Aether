#pragma once
#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Progression/AetherProgressionTypes.h"
#include "AetherProgressionConfigDataAsset.generated.h"

UCLASS(BlueprintType)
class AGEOFAETHER_API UAetherProgressionConfigDataAsset : public UDataAsset
{
    GENERATED_BODY()

public:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Progression")
    FAetherProgressionConfig Config;
};
