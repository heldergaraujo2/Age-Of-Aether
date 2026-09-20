#pragma once
#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "AetherContentPackageTypes.generated.h"

UENUM(BlueprintType)
enum class EAetherContentPackageStatus : uint8 { Draft, Validated, Approved };

USTRUCT(BlueprintType)
struct FAetherContentPackageEntry
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadOnly) FString ContentID;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) FString DisplayName;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) FString Version = TEXT("1.0.0");
    UPROPERTY(EditAnywhere, BlueprintReadOnly) bool bEnabled = true;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) TArray<FString> Dependencies;
    bool IsValid() const;
};

UCLASS(BlueprintType)
class AGEOFAETHER_API UAetherContentPackage : public UDataAsset
{
    GENERATED_BODY()
public:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Package") FString PackageID;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Package") FString DisplayName;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Package") FString Version = TEXT("1.0.0");
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Package") EAetherContentPackageStatus Status = EAetherContentPackageStatus::Draft;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Package") TArray<FAetherContentPackageEntry> Entries;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Package") TArray<FString> RequiredAssetIDs;
    bool Validate(TArray<FString>& OutErrors) const;
};