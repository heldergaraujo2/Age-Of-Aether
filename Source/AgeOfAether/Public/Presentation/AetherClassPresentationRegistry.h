#pragma once
#include "CoreMinimal.h"
#include "Presentation/AetherClassPresentationTypes.h"

struct FAetherClassPresentationValidationIssue
{
    FString PresentationID;
    FString Code;
    FString Message;
};

class FAetherClassPresentationRegistry
{
public:
    bool Register(const FAetherClassPresentationDefinition& Definition, FString& OutError);
    bool Resolve(const FString& ClassID, const FString& EvolutionID, FAetherClassPresentationDefinition& OutDefinition) const;
    bool ResolveWithFallback(const FString& ClassID, const FString& EvolutionID, FAetherClassPresentationDefinition& OutDefinition, bool& bOutFallback) const;
    bool Validate(TArray<FAetherClassPresentationValidationIssue>& OutIssues) const;
    void Reset();
    int32 Num() const { return Definitions.Num(); }
    void GetIDs(TArray<FString>& OutIDs) const;

private:
    TMap<FString, FAetherClassPresentationDefinition> Definitions;
    static FString Key(const FString& ClassID, const FString& EvolutionID);
    static FString Normalize(const FString& Value);
};
