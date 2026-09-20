#include "Client/AetherPresentationSubsystem.h"

FString UAetherPresentationSubsystem::NormalizeID(const FString& ID)
{
    FString Result = ID.TrimStartAndEnd();
    Result.ToLowerInline();
    return Result;
}

void UAetherPresentationSubsystem::Deinitialize()
{
    ResetPresentation();
    Super::Deinitialize();
}

void UAetherPresentationSubsystem::ResetPresentation()
{
    AssetBindings.Reset();
    ActorSnapshots.Reset();
    LatestPerformance.Reset();
    Acceptance.Reset();
}

void UAetherPresentationSubsystem::RegisterAssetBinding(const FString& AssetID, const FString& RuntimePath, const FString& PlaceholderPath)
{
    const FString Key = NormalizeID(AssetID);
    if (Key.IsEmpty()) return;

    FAetherAssetRuntimeBinding Binding;
    Binding.AssetID = AssetID.TrimStartAndEnd();
    Binding.RuntimePath = RuntimePath.TrimStartAndEnd();
    Binding.PlaceholderPath = PlaceholderPath.TrimStartAndEnd();
    Binding.bResolved = !Binding.RuntimePath.IsEmpty();
    AssetBindings.Add(Key, Binding);
}

bool UAetherPresentationSubsystem::ResolveAsset(const FString& AssetID, FAetherAssetRuntimeBinding& OutBinding) const
{
    const FString Key = NormalizeID(AssetID);
    const FAetherAssetRuntimeBinding* Binding = AssetBindings.Find(Key);
    if (!Binding) return false;
    OutBinding = *Binding;
    if (!OutBinding.bResolved && !OutBinding.PlaceholderPath.IsEmpty())
    {
        OutBinding.RuntimePath = OutBinding.PlaceholderPath;
    }
    return true;
}

void UAetherPresentationSubsystem::ApplyActorSnapshot(const FAetherPresentationActorSnapshot& Snapshot)
{
    if (!Snapshot.IsValid()) return;
    const FString Key = NormalizeID(Snapshot.EntityID);
    if (Key.IsEmpty()) return;
    ActorSnapshots.Add(Key, Snapshot);
    OnActorPresentationChanged.Broadcast(Snapshot);
}

bool UAetherPresentationSubsystem::GetActorSnapshot(const FString& EntityID, FAetherPresentationActorSnapshot& OutSnapshot) const
{
    const FAetherPresentationActorSnapshot* Snapshot = ActorSnapshots.Find(NormalizeID(EntityID));
    if (!Snapshot) return false;
    OutSnapshot = *Snapshot;
    return true;
}

void UAetherPresentationSubsystem::RecordPerformanceSample(EAetherPerformanceMetric Metric, double Value, double Budget, double TimestampSeconds)
{
    FAetherPerformanceSample Sample;
    Sample.Metric = Metric;
    Sample.Value = Value;
    Sample.Budget = Budget;
    Sample.TimestampSeconds = TimestampSeconds;
    if (!FMath::IsFinite(Value) || !FMath::IsFinite(Budget) || Budget < 0.0) return;
    LatestPerformance.Add(Metric, Sample);
    OnPerformanceSample.Broadcast(Sample);
}

bool UAetherPresentationSubsystem::GetLatestPerformanceSample(EAetherPerformanceMetric Metric, FAetherPerformanceSample& OutSample) const
{
    const FAetherPerformanceSample* Sample = LatestPerformance.Find(Metric);
    if (!Sample) return false;
    OutSample = *Sample;
    return true;
}

void UAetherPresentationSubsystem::SetAcceptanceResult(EAetherClientAcceptanceStep Step, bool bPassed, const FString& Diagnostic)
{
    FAetherClientAcceptanceResult Result;
    Result.Step = Step;
    Result.bPassed = bPassed;
    Result.Diagnostic = Diagnostic.TrimStartAndEnd();
    Acceptance.Add(Step, Result);
}

bool UAetherPresentationSubsystem::IsAcceptanceComplete() const
{
    constexpr int32 RequiredSteps = 30;
    return Acceptance.Num() == RequiredSteps && GetAcceptancePassedCount() == RequiredSteps;
}

int32 UAetherPresentationSubsystem::GetAcceptancePassedCount() const
{
    int32 Count = 0;
    for (const TPair<EAetherClientAcceptanceStep, FAetherClientAcceptanceResult>& Pair : Acceptance)
    {
        if (Pair.Value.bPassed) ++Count;
    }
    return Count;
}
