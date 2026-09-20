#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Engine/AssetManagerTypes.h"

#include "AetherVisualFoundationDataAsset.generated.h"

UCLASS(BlueprintType)
class AGEOFAETHER_API UAetherVisualFoundationDataAsset : public UPrimaryDataAsset
{
    GENERATED_BODY()

public:
    virtual FPrimaryAssetId GetPrimaryAssetId() const override;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Age of Aether|Visual Foundation")
    FName AssetID;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Age of Aether|Visual Foundation")
    FName DefinitionID;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Age of Aether|Visual Foundation")
    TSoftObjectPtr<UObject> VisualAsset;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Age of Aether|Visual Foundation")
    TSoftObjectPtr<UObject> FallbackAsset;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Age of Aether|Visual Foundation")
    FName Version = TEXT("1");
};
