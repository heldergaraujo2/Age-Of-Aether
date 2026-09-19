#pragma once
#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Scale/AetherScaleTypes.h"
#include "AetherScaleConfigDataAsset.generated.h"

UCLASS(BlueprintType)
class AGEOFAETHER_API UAetherScaleConfigDataAsset : public UDataAsset
{
    GENERATED_BODY()
public:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Scale")
    FAetherScaleConfig Config;
};
