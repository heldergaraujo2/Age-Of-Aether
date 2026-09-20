#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"

#include "AetherAssetManager.generated.h"

UCLASS()
class AGEOFAETHER_API UAetherAssetManager : public UAssetManager
{
    GENERATED_BODY()

public:
    static UAetherAssetManager& Get();

    virtual void StartInitialLoading() override;
};
