#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Social/AetherSocialTypes.h"
#include "AetherSocialConfigDataAsset.generated.h"

UCLASS(BlueprintType)
class AGEOFAETHER_API UAetherSocialConfigDataAsset : public UDataAsset
{
    GENERATED_BODY()

public:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Social")
    FAetherSocialConfig Config;
};
