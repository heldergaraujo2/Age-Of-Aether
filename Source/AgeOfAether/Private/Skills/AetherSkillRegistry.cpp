#include "Skills/AetherSkillRegistry.h"

FString FAetherSkillRegistry::Normalize(const FString& Value)
{
    return Value.TrimStartAndEnd().ToLower();
}

bool FAetherSkillRegistry::Register(const FAetherSkillDefinition& Definition)
{
    if (!Definition.IsValid())
        return false;
    const FString Key = Normalize(Definition.SkillID);
    if (Key.IsEmpty() || Definitions.Contains(Key))
        return false;
    FAetherSkillDefinition Copy = Definition;
    Copy.SkillID = Key;
    Definitions.Add(Key, MoveTemp(Copy));
    return true;
}

bool FAetherSkillRegistry::RegisterBatch(const TArray<FAetherSkillDefinition>& InDefinitions)
{
    TMap<FString, FAetherSkillDefinition> Previous = Definitions;
    for (const FAetherSkillDefinition& Definition : InDefinitions)
    {
        if (!Register(Definition))
        {
            Definitions = MoveTemp(Previous);
            return false;
        }
    }
    return true;
}

const FAetherSkillDefinition* FAetherSkillRegistry::Find(const FString& SkillID) const
{
    return Definitions.Find(Normalize(SkillID));
}

bool FAetherSkillRegistry::Validate(TArray<FString>& OutErrors) const
{
    OutErrors.Reset();
    for (const TPair<FString, FAetherSkillDefinition>& Pair : Definitions)
    {
        if (!Pair.Value.IsValid())
            OutErrors.Add(FString::Printf(TEXT("Invalid skill definition: %s"), *Pair.Key));
        if (Pair.Key != Normalize(Pair.Value.SkillID))
            OutErrors.Add(FString::Printf(TEXT("Non-normalized skill ID: %s"), *Pair.Key));
    }
    return OutErrors.IsEmpty();
}

void FAetherSkillRegistry::Reset()
{
    Definitions.Reset();
}
