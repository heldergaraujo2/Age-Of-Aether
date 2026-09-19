#pragma once
#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Multiplayer/AetherMultiplayerTypes.h"
#include "AetherMultiplayerConfigDataAsset.generated.h"

UCLASS(BlueprintType)
class AGEOFAETHER_API UAetherMultiplayerConfigDataAsset : public UDataAsset
{
    GENERATED_BODY()
public:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Multiplayer")
    FAetherMultiplayerConfig Config;
};