#pragma once

#include "CoreMinimal.h"
#include "Skills/AetherSkillTypes.h"

class FAetherSkillRegistry
{
public:
    bool Register(const FAetherSkillDefinition& Definition);
    bool RegisterBatch(const TArray<FAetherSkillDefinition>& Definitions);
    const FAetherSkillDefinition* Find(const FString& SkillID) const;
    bool Validate(TArray<FString>& OutErrors) const;
    void Reset();
    int32 Num() const { return Definitions.Num(); }

private:
    static FString Normalize(const FString& Value);
    TMap<FString, FAetherSkillDefinition> Definitions;
};
