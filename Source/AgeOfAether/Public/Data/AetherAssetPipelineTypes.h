#pragma once

#include "CoreMinimal.h"
#include "AetherAssetPipelineTypes.generated.h"

UENUM(BlueprintType)
enum class EAetherAssetValidationStatus : uint8
{
    Draft,
    Imported,
    Validated,
    Approved,
    Rejected
};

USTRUCT(BlueprintType)
struct FAetherAssetImportProfile
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Import")
    FString ProfileID = TEXT("default_fbx");

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Import")
    FString SourceFormat = TEXT("FBX");

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Import")
    FString ExpectedFBXVersion = TEXT("2020.2");

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Import")
    float ImportScale = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Import")
    bool bForceFrontAxis = false;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Import")
    bool bCreatePhysicsAsset = true;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Import")
    bool bImportMaterials = false;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Import")
    bool bImportTextures = false;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Import")
    bool bImportAnimations = false;

    bool IsValid(FString& OutError) const;
};

USTRUCT(BlueprintType)
struct FAetherAssetSourceMetadata
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Source")
    FString SourceReference;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Source")
    FString SourceFormat = TEXT("FBX");

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Source")
    FString SourceVersion;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Source")
    FString Origin;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Source")
    FString License;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Source")
    FString Author;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Source")
    FString ImportedAtUTC;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Source")
    FString ContentHash;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Source")
    int32 SourceRevision = 1;

    bool IsValid(FString& OutError) const;
};

USTRUCT(BlueprintType)
struct FAetherAssetPipelineRecord
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Asset")
    FString AssetID;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Asset")
    EAetherAssetType AssetType = EAetherAssetType::Unknown;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Asset")
    EAetherAssetValidationStatus Status = EAetherAssetValidationStatus::Draft;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Asset")
    FString UnrealAssetPath;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Asset")
    FString FallbackAssetID;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Asset")
    FString SkeletonAssetID;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Asset")
    TArray<FString> DependencyAssetIDs;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Asset")
    TArray<FString> SocketNames;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Asset")
    int32 ContentVersion = 1;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Asset")
    FAetherAssetSourceMetadata Source;

    bool IsValid(FString& OutError) const;
};
