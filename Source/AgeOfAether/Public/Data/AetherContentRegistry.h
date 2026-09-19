#pragma once
#include "CoreMinimal.h"
#include "Data/AetherContentTypes.h"

struct FAetherContentValidationIssue
{
    FString DefinitionID;
    FString Code;
    FString Message;
};

class FAetherContentRegistry
{
public:
    bool RegisterDefinition(const FAetherContentDefinition& Definition, FString& OutError);
    bool RegisterDefinitions(const TArray<FAetherContentDefinition>& Definitions, TArray<FAetherContentValidationIssue>& OutIssues);
    bool Resolve(const FString& DefinitionID, FAetherContentDefinition& OutDefinition) const;
    bool Contains(const FString& DefinitionID) const;
    bool Remove(const FString& DefinitionID);
    void Reset();

    bool Validate(TArray<FAetherContentValidationIssue>& OutIssues) const;
    void GetDefinitionIDs(TArray<FString>& OutIDs) const;
    int32 Num() const { return Definitions.Num(); }

private:
    TMap<FString, FAetherContentDefinition> Definitions;

    void AddIssue(TArray<FAetherContentValidationIssue>& OutIssues,
        const FString& DefinitionID, const FString& Code, const FString& Message) const;
};
