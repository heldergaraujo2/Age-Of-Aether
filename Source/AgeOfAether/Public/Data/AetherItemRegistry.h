#pragma once
#include "CoreMinimal.h"
#include "Data/AetherItemDataTypes.h"

class FAetherContentRegistry;
class FAetherAssetRegistry;

class FAetherItemRegistry
{
public:
    bool RegisterItem(const FAetherDataItemDefinition& Definition, FString& OutError);
    bool RegisterItems(const TArray<FAetherDataItemDefinition>& Definitions, TArray<FAetherItemValidationIssue>& OutIssues);
    bool Resolve(const FString& DefinitionID, FAetherDataItemDefinition& OutDefinition) const;
    bool Contains(const FString& DefinitionID) const;
    bool Remove(const FString& DefinitionID);
    void Reset();
    bool Validate(TArray<FAetherItemValidationIssue>& OutIssues,
        const FAetherContentRegistry* ContentRegistry = nullptr,
        const FAetherAssetRegistry* AssetRegistry = nullptr) const;
    void GetDefinitionIDs(TArray<FString>& OutIDs) const;
    int32 Num() const { return Definitions.Num(); }

private:
    TMap<FString, FAetherDataItemDefinition> Definitions;
    void AddIssue(TArray<FAetherItemValidationIssue>& OutIssues,
        const FString& DefinitionID, const FString& Code, const FString& Message) const;
};