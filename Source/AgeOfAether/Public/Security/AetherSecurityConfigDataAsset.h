#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Security/AetherSecurityTypes.h"
#include "AetherSecurityConfigDataAsset.generated.h"

UCLASS(BlueprintType)
class AGEOFAETHER_API UAetherSecurityConfigDataAsset : public UDataAsset
{
    GENERATED_BODY()

public:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Security")
    FAetherSecurityConfig Config;
};
