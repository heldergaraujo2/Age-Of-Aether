#include "Data/AetherVisualFoundationDataAsset.h"

FPrimaryAssetId UAetherVisualFoundationDataAsset::GetPrimaryAssetId() const
{
    const FName StableName = AssetID.IsNone() ? GetFName() : AssetID;
    return FPrimaryAssetId(TEXT("AetherVisual"), StableName);
}
