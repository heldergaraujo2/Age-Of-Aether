#pragma once
#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Production/AetherProductionTypes.h"
#include "AetherProductionConfigDataAsset.generated.h"
UCLASS(BlueprintType)class AGEOFAETHER_API UAetherProductionConfigDataAsset:public UDataAsset{GENERATED_BODY()public:UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Production")FAetherProductionConfig Config;};
