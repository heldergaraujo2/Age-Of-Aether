#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Combat/AetherCombatTypes.h"

#include "AetherCombatConfigDataAsset.generated.h"

UCLASS(BlueprintType)
class AGEOFAETHER_API UAetherCombatConfigDataAsset : public UDataAsset
{
    GENERATED_BODY()

public:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat")
    FAetherCombatConfig Config;
};
