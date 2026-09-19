#pragma once

#include "CoreMinimal.h"
#include "Data/AetherAssetTypes.h"

struct FAetherAssetValidationIssue
{
    FString AssetID;
    FString Code;
    FString Message;
};

class FAetherAssetRegistry
{
public:
    bool RegisterAsset(const FAetherAssetDefinition& Definition, FString& OutError);
    bool RegisterAssets(const TArray<FAetherAssetDefinition>& Definitions, TArray<FAetherAssetValidationIssue>& OutIssues);
    bool Resolve(const FString& AssetID, FAetherAssetDefinition& OutDefinition) const;
    bool Contains(const FString& AssetID) const;
    bool Remove(const FString& AssetID);
    void Reset();

    bool Validate(TArray<FAetherAssetValidationIssue>& OutIssues) const;
    void GetAssetIDs(TArray<FString>& OutIDs) const;
    int32 Num() const { return Assets.Num(); }

private:
    TMap<FString, FAetherAssetDefinition> Assets;

    void AddIssue(TArray<FAetherAssetValidationIssue>& OutIssues,
        const FString& AssetID, const FString& Code, const FString& Message) const;
};
