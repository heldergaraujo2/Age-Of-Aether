#pragma once

#include "CoreMinimal.h"
#include "Data/AetherAssetPipelineTypes.h"

struct FAetherAssetPipelineIssue
{
    FString AssetID;
    FString Code;
    FString Message;
};

class FAetherAssetPipelineRegistry
{
public:
    bool Register(const FAetherAssetPipelineRecord& Record, FString& OutError);
    bool RegisterBatch(const TArray<FAetherAssetPipelineRecord>& Records, TArray<FAetherAssetPipelineIssue>& OutIssues);
    bool Resolve(const FString& AssetID, FAetherAssetPipelineRecord& OutRecord) const;
    bool Validate(TArray<FAetherAssetPipelineIssue>& OutIssues) const;
    void Reset();
    int32 Num() const { return Records.Num(); }

private:
    TMap<FString, FAetherAssetPipelineRecord> Records;

    static FString NormalizeID(const FString& ID);
    static void AddIssue(TArray<FAetherAssetPipelineIssue>& OutIssues, const FString& AssetID, const FString& Code, const FString& Message);
};
