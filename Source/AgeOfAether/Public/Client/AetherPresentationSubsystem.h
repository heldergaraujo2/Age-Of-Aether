#pragma once
#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Client/AetherPresentationTypes.h"
#include "AetherPresentationSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAetherPresentationActorEvent, const FAetherPresentationActorSnapshot&, Snapshot);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAetherPresentationPerformanceEvent, const FAetherPerformanceSample&, Sample);

UCLASS()
class AGEOFAETHER_API UAetherPresentationSubsystem : public UGameInstanceSubsystem
{
    GENERATED_BODY()

public:
    virtual void Deinitialize() override;

    UFUNCTION(BlueprintCallable, Category="Age of Aether|Presentation")
    void ResetPresentation();

    UFUNCTION(BlueprintCallable, Category="Age of Aether|Presentation")
    void RegisterAssetBinding(const FString& AssetID, const FString& RuntimePath, const FString& PlaceholderPath);

    UFUNCTION(BlueprintPure, Category="Age of Aether|Presentation")
    bool ResolveAsset(const FString& AssetID, FAetherAssetRuntimeBinding& OutBinding) const;

    UFUNCTION(BlueprintCallable, Category="Age of Aether|Presentation")
    void ApplyActorSnapshot(const FAetherPresentationActorSnapshot& Snapshot);

    UFUNCTION(BlueprintPure, Category="Age of Aether|Presentation")
    bool GetActorSnapshot(const FString& EntityID, FAetherPresentationActorSnapshot& OutSnapshot) const;

    UFUNCTION(BlueprintCallable, Category="Age of Aether|Presentation")
    void RecordPerformanceSample(EAetherPerformanceMetric Metric, double Value, double Budget, double TimestampSeconds);

    UFUNCTION(BlueprintPure, Category="Age of Aether|Presentation")
    bool GetLatestPerformanceSample(EAetherPerformanceMetric Metric, FAetherPerformanceSample& OutSample) const;

    UFUNCTION(BlueprintCallable, Category="Age of Aether|Presentation")
    void SetAcceptanceResult(EAetherClientAcceptanceStep Step, bool bPassed, const FString& Diagnostic);

    UFUNCTION(BlueprintPure, Category="Age of Aether|Presentation")
    bool IsAcceptanceComplete() const;

    UFUNCTION(BlueprintPure, Category="Age of Aether|Presentation")
    int32 GetAcceptancePassedCount() const;

    UPROPERTY(BlueprintAssignable, Category="Age of Aether|Presentation")
    FAetherPresentationActorEvent OnActorPresentationChanged;

    UPROPERTY(BlueprintAssignable, Category="Age of Aether|Presentation")
    FAetherPresentationPerformanceEvent OnPerformanceSample;

private:
    TMap<FString, FAetherAssetRuntimeBinding> AssetBindings;
    TMap<FString, FAetherPresentationActorSnapshot> ActorSnapshots;
    TMap<EAetherPerformanceMetric, FAetherPerformanceSample> LatestPerformance;
    TMap<EAetherClientAcceptanceStep, FAetherClientAcceptanceResult> Acceptance;

    static FString NormalizeID(const FString& ID);
};
