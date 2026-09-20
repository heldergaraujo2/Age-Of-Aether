#include "Data/AetherAssetPipelineRegistry.h"

FString FAetherAssetPipelineRegistry::NormalizeID(const FString& ID)
{
    FString Normalized = ID.TrimStartAndEnd();
    Normalized.ToLowerInline();
    return Normalized;
}

void FAetherAssetPipelineRegistry::AddIssue(
    TArray<FAetherAssetPipelineIssue>& OutIssues,
    const FString& AssetID,
    const FString& Code,
    const FString& Message)
{
    OutIssues.Add({AssetID, Code, Message});
}

bool FAetherAssetPipelineRegistry::Register(const FAetherAssetPipelineRecord& Record, FString& OutError)
{
    OutError.Reset();

    FAetherAssetPipelineRecord Copy = Record;
    Copy.AssetID = NormalizeID(Copy.AssetID);

    if (!Copy.IsValid(OutError))
    {
        return false;
    }

    if (Records.Contains(Copy.AssetID))
    {
        OutError = FString::Printf(TEXT("Duplicate pipeline AssetID '%s'."), *Copy.AssetID);
        return false;
    }

    Records.Add(Copy.AssetID, MoveTemp(Copy));
    return true;
}

bool FAetherAssetPipelineRegistry::RegisterBatch(
    const TArray<FAetherAssetPipelineRecord>& RecordsToAdd,
    TArray<FAetherAssetPipelineIssue>& OutIssues)
{
    OutIssues.Reset();

    for (const FAetherAssetPipelineRecord& Record : RecordsToAdd)
    {
        FString Error;
        if (!Register(Record, Error))
        {
            AddIssue(OutIssues, Record.AssetID, TEXT("RegistrationFailed"), Error);
        }
    }

    TArray<FAetherAssetPipelineIssue> ValidationIssues;
    Validate(ValidationIssues);
    OutIssues.Append(ValidationIssues);
    return OutIssues.IsEmpty();
}

bool FAetherAssetPipelineRegistry::Resolve(const FString& AssetID, FAetherAssetPipelineRecord& OutRecord) const
{
    const FString Key = NormalizeID(AssetID);
    if (const FAetherAssetPipelineRecord* Found = Records.Find(Key))
    {
        OutRecord = *Found;
        return true;
    }

    return false;
}

bool FAetherAssetPipelineRegistry::Validate(TArray<FAetherAssetPipelineIssue>& OutIssues) const
{
    OutIssues.Reset();

    for (const auto& Pair : Records)
    {
        const FAetherAssetPipelineRecord& Record = Pair.Value;
        FString Error;
        if (!Record.IsValid(Error))
        {
            AddIssue(OutIssues, Record.AssetID, TEXT("Invalid"), Error);
        }

        if (!Record.FallbackAssetID.IsEmpty() &&
            !Records.Contains(NormalizeID(Record.FallbackAssetID)))
        {
            AddIssue(OutIssues, Record.AssetID, TEXT("MissingFallback"), Record.FallbackAssetID);
        }

        for (const FString& Dependency : Record.DependencyAssetIDs)
        {
            if (!Records.Contains(NormalizeID(Dependency)))
            {
                AddIssue(OutIssues, Record.AssetID, TEXT("MissingDependency"), Dependency);
            }
        }
    }

    TSet<FString> Visiting;
    TSet<FString> Visited;

    TFunction<void(const FString&)> Visit = [&](const FString& ID)
    {
        if (Visiting.Contains(ID))
        {
            AddIssue(OutIssues, ID, TEXT("DependencyCycle"), TEXT("Asset dependency cycle detected."));
            return;
        }

        if (Visited.Contains(ID))
        {
            return;
        }

        const FAetherAssetPipelineRecord* Record = Records.Find(ID);
        if (!Record)
        {
            return;
        }

        Visiting.Add(ID);
        for (const FString& Dependency : Record->DependencyAssetIDs)
        {
            const FString DependencyID = NormalizeID(Dependency);
            if (Records.Contains(DependencyID))
            {
                Visit(DependencyID);
            }
        }
        Visiting.Remove(ID);
        Visited.Add(ID);
    };

    for (const auto& Pair : Records)
    {
        Visit(Pair.Key);
    }

    return OutIssues.IsEmpty();
}
