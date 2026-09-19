#pragma once
#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "AI/AetherAITypes.h"
#include "AetherAIConfigDataAsset.generated.h"
UCLASS(BlueprintType)class AGEOFAETHER_API UAetherAIConfigDataAsset:public UDataAsset{GENERATED_BODY()public:UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="AI")FAetherAIConfig Config;};
