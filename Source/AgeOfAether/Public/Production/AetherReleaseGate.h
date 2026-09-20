#pragma once

#include "CoreMinimal.h"
#include "Production/AetherProductionTypes.h"
#include "AetherReleaseGate.generated.h"

UENUM(BlueprintType)
enum class EAetherReleaseStage : uint8
{
    Development,
    Alpha,
    Beta,
    ReleaseCandidate,
    Release
};

UENUM(BlueprintType)
enum class EAetherReleaseCheck : uint8
{
    Build,
    Content,
    Security,
    Persistence,
    Networking,
    DedicatedServer,
    Multiplayer,
    Performance,
    UI,
    Audio
};

USTRUCT(BlueprintType)
struct FAetherReleaseManifest
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Release")
    FString Version = TEXT("0.1.0");

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Release")
    FString BuildId;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Release")
    EAetherReleaseStage Stage = EAetherReleaseStage::Development;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Release")
    FString ContentPackageID;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Release")
    FString ContentVersion;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Release")
    FString GitCommit;

    bool IsValid() const;
};

USTRUCT(BlueprintType)
struct FAetherReleaseCheckResult
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    EAetherReleaseCheck Check = EAetherReleaseCheck::Build;

    UPROPERTY(BlueprintReadOnly)
    bool bPassed = false;

    UPROPERTY(BlueprintReadOnly)
    FString Details;
};

UCLASS(BlueprintType)
class AGEOFAETHER_API UAetherReleaseGate : public UObject
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category="Age of Aether|Release")
    void Configure(const FAetherReleaseManifest& InManifest);

    UFUNCTION(BlueprintCallable, Category="Age of Aether|Release")
    bool SetCheck(EAetherReleaseCheck Check, bool bPassed, const FString& Details);

    UFUNCTION(BlueprintPure, Category="Age of Aether|Release")
    bool IsReadyForStage(EAetherReleaseStage TargetStage) const;

    UFUNCTION(BlueprintPure, Category="Age of Aether|Release")
    bool IsFullyPassed() const;

    UFUNCTION(BlueprintPure, Category="Age of Aether|Release")
    FAetherReleaseManifest GetManifest() const { return Manifest; }

    UFUNCTION(BlueprintPure, Category="Age of Aether|Release")
    TArray<FAetherReleaseCheckResult> GetChecks() const { return Checks; }

private:
    FAetherReleaseManifest Manifest;
    TArray<FAetherReleaseCheckResult> Checks;
};
