#include "Data/AetherAssetPipelineTypes.h"

bool FAetherAssetImportProfile::IsValid(FString& OutError) const
{
    OutError.Reset();

    if (ProfileID.TrimStartAndEnd().IsEmpty())
    {
        OutError = TEXT("ProfileID is required.");
        return false;
    }

    if (!SourceFormat.Equals(TEXT("FBX"), ESearchCase::IgnoreCase))
    {
        OutError = TEXT("Phase 38 currently requires the FBX source pipeline.");
        return false;
    }

    if (ExpectedFBXVersion.TrimStartAndEnd().IsEmpty())
    {
        OutError = TEXT("ExpectedFBXVersion is required.");
        return false;
    }

    if (!FMath::IsFinite(ImportScale) || ImportScale <= 0.0f)
    {
        OutError = TEXT("ImportScale must be finite and positive.");
        return false;
    }

    return true;
}

bool FAetherAssetSourceMetadata::IsValid(FString& OutError) const
{
    OutError.Reset();

    if (SourceReference.TrimStartAndEnd().IsEmpty())
    {
        OutError = TEXT("SourceReference is required.");
        return false;
    }

    if (SourceFormat.TrimStartAndEnd().IsEmpty())
    {
        OutError = TEXT("SourceFormat is required.");
        return false;
    }

    if (Origin.TrimStartAndEnd().IsEmpty())
    {
        OutError = TEXT("Origin is required.");
        return false;
    }

    if (License.TrimStartAndEnd().IsEmpty())
    {
        OutError = TEXT("License is required.");
        return false;
    }

    if (SourceRevision <= 0)
    {
        OutError = TEXT("SourceRevision must be positive.");
        return false;
    }

    return true;
}

bool FAetherAssetPipelineRecord::IsValid(FString& OutError) const
{
    OutError.Reset();

    if (AssetID.TrimStartAndEnd().IsEmpty())
    {
        OutError = TEXT("AssetID is required.");
        return false;
    }

    if (AssetType == EAetherAssetType::Unknown)
    {
        OutError = TEXT("AssetType cannot be Unknown.");
        return false;
    }

    if (ContentVersion <= 0)
    {
        OutError = TEXT("ContentVersion must be positive.");
        return false;
    }

    if (UnrealAssetPath.TrimStartAndEnd().IsEmpty())
    {
        OutError = TEXT("UnrealAssetPath is required.");
        return false;
    }

    if (FallbackAssetID.Equals(AssetID, ESearchCase::IgnoreCase))
    {
        OutError = TEXT("FallbackAssetID cannot reference the same asset.");
        return false;
    }

    if (!Source.IsValid(OutError))
    {
        return false;
    }

    return true;
}
