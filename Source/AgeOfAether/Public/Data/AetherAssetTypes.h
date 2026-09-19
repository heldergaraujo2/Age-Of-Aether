#pragma once

#include "CoreMinimal.h"
#include "AetherAssetTypes.generated.h"

UENUM(BlueprintType)
enum class EAetherAssetType : uint8
{
    Unknown,
    SkeletalMesh,
    StaticMesh,
    Animation,
    Texture,
    Material,
    MaterialInstance,
    Niagara,
    Sound,
    Widget,
    Icon,
    Map
};

USTRUCT(BlueprintType)
struct FAetherAssetReference
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FString AssetID;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    EAetherAssetType AssetType = EAetherAssetType::Unknown;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FString UnrealAssetPath;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FString FallbackAssetID;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FString SkeletonAssetID;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TArray<FString> AnimationSetIDs;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TArray<FString> MaterialAssetIDs;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FString IconAssetID;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TArray<FString> VFXAssetIDs;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TArray<FString> SFXAssetIDs;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FVector Scale = FVector::OneVector;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TArray<FString> SocketNames;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    int32 ContentVersion = 1;

    bool IsStructurallyValid(FString& OutError) const
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

        if (Scale.X <= 0.0 || Scale.Y <= 0.0 || Scale.Z <= 0.0 ||
            !FMath::IsFinite(Scale.X) || !FMath::IsFinite(Scale.Y) || !FMath::IsFinite(Scale.Z))
        {
            OutError = TEXT("Scale must contain finite positive values.");
            return false;
        }

        return true;
    }
};

USTRUCT(BlueprintType)
struct FAetherAssetDefinition
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FAetherAssetReference Asset;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FString SourceFormat = TEXT("FBX");

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FString SourceReference;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FString SkeletonProfile;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    bool bRequiresSkeleton = false;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    bool bHasFallback = false;

    bool IsStructurallyValid(FString& OutError) const
    {
        if (!Asset.IsStructurallyValid(OutError))
        {
            return false;
        }

        if (SourceFormat.TrimStartAndEnd().IsEmpty())
        {
            OutError = TEXT("SourceFormat is required.");
            return false;
        }

        if (bRequiresSkeleton && Asset.SkeletonAssetID.TrimStartAndEnd().IsEmpty())
        {
            OutError = TEXT("A skeleton is required but SkeletonAssetID is missing.");
            return false;
        }

        if (bHasFallback && Asset.FallbackAssetID.TrimStartAndEnd().IsEmpty())
        {
            OutError = TEXT("bHasFallback is true but FallbackAssetID is missing.");
            return false;
        }

        return true;
    }
};
