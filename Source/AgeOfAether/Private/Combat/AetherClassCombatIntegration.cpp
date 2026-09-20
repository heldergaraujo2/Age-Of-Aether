#include "Combat/AetherClassCombatIntegration.h"

namespace
{
FString Normalize(const FString& Value)
{
    return Value.TrimStartAndEnd().ToLower();
}
}

bool FAetherClassCombatIntegration::ResolveAuthoritativeContext(
    const FAetherCharacterRecord& Attacker,
    const FAetherCharacterRecord& Target,
    EAetherCombatMode Mode,
    const FAetherClassRegistry& ClassRegistry,
    const FAetherClassBalanceRegistry& BalanceRegistry,
    FAetherCombatBalanceContext& OutContext,
    FString& OutError) const
{
    OutContext = FAetherCombatBalanceContext();
    OutContext.Mode = Mode;
    OutError.Reset();

    FAetherClassDefinition AttackerClass;
    FAetherClassDefinition TargetClass;
    if (!ClassRegistry.ResolveClass(Attacker.ClassID, AttackerClass))
    {
        OutError = TEXT("Attacker class is not authoritative or registered.");
        return false;
    }
    if (!ClassRegistry.ResolveClass(Target.ClassID, TargetClass))
    {
        OutError = TEXT("Target class is not authoritative or registered.");
        return false;
    }

    FAetherClassEvolutionDefinition AttackerEvolution;
    FAetherClassEvolutionDefinition TargetEvolution;
    if (!ClassRegistry.ResolveEvolution(Attacker.EvolutionID, AttackerEvolution)
        || Normalize(AttackerEvolution.ClassID) != Normalize(Attacker.ClassID))
    {
        OutError = TEXT("Attacker evolution does not belong to the authoritative class.");
        return false;
    }
    if (!ClassRegistry.ResolveEvolution(Target.EvolutionID, TargetEvolution)
        || Normalize(TargetEvolution.ClassID) != Normalize(Target.ClassID))
    {
        OutError = TEXT("Target evolution does not belong to the authoritative class.");
        return false;
    }

    const bool bPvP = Mode == EAetherCombatMode::PvP;
    if (!BalanceRegistry.ResolveActive(Attacker.ClassID, Attacker.EvolutionID, bPvP, OutContext.AttackerModifiers, OutError))
    {
        return false;
    }
    if (!BalanceRegistry.ResolveActive(Target.ClassID, Target.EvolutionID, bPvP, OutContext.TargetModifiers, OutError))
    {
        return false;
    }

    OutContext.bAuthoritative = true;
    return true;
}

bool FAetherClassCombatIntegration::ValidateEvolutionTransition(
    const FAetherCharacterRecord& Character,
    const FString& TargetEvolutionID,
    int32 CurrentLevel,
    const FAetherClassRegistry& ClassRegistry,
    FString& OutError) const
{
    OutError.Reset();
    FAetherClassEvolutionDefinition Current;
    FAetherClassEvolutionDefinition Target;
    if (!ClassRegistry.ResolveEvolution(Character.EvolutionID, Current)
        || Normalize(Current.ClassID) != Normalize(Character.ClassID))
    {
        OutError = TEXT("Current evolution is not authoritative for the character class.");
        return false;
    }
    if (!ClassRegistry.ResolveEvolution(TargetEvolutionID, Target)
        || Normalize(Target.ClassID) != Normalize(Character.ClassID))
    {
        OutError = TEXT("Target evolution does not belong to the character class.");
        return false;
    }
    if (Target.Stage <= Current.Stage)
    {
        OutError = TEXT("Evolution must advance to a later stage.");
        return false;
    }
    if (CurrentLevel < Target.RequiredLevel)
    {
        OutError = TEXT("Character level does not meet the evolution requirement.");
        return false;
    }
    if (Target.PrerequisiteEvolutionIDs.Num() > 0)
    {
        bool bCurrentIsPrerequisite = false;
        for (const FString& Prerequisite : Target.PrerequisiteEvolutionIDs)
        {
            if (Normalize(Prerequisite) == Normalize(Character.EvolutionID))
            {
                bCurrentIsPrerequisite = true;
                break;
            }
        }
        if (!bCurrentIsPrerequisite)
        {
            OutError = TEXT("Current evolution is not an allowed prerequisite for the target evolution.");
            return false;
        }
    }
    return true;
}

bool FAetherClassCombatIntegration::ValidateClientClassClaims(
    const FAetherCharacterRecord& AuthoritativeCharacter,
    const FString& ClientClassID,
    const FString& ClientEvolutionID,
    FString& OutError) const
{
    OutError.Reset();
    if (!ClientClassID.IsEmpty() && Normalize(ClientClassID) != Normalize(AuthoritativeCharacter.ClassID))
    {
        OutError = TEXT("Client class claim does not match authoritative character state.");
        return false;
    }
    if (!ClientEvolutionID.IsEmpty() && Normalize(ClientEvolutionID) != Normalize(AuthoritativeCharacter.EvolutionID))
    {
        OutError = TEXT("Client evolution claim does not match authoritative character state.");
        return false;
    }
    return true;
}
