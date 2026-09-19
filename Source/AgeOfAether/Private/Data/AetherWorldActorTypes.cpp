#include "Data/AetherWorldActorTypes.h"

namespace
{
bool ValidNonNegative(const double V) { return FMath::IsFinite(V) && V >= 0.0; }
bool ValidCombat(const FAetherCombatStats& C, FString& E)
{
    if (C.Level <= 0 || C.MaxHealth <= 0 || C.MaxMana < 0) { E = TEXT("Combat level/health/mana is invalid."); return false; }
    if (!ValidNonNegative(C.MinDamage) || !ValidNonNegative(C.MaxDamage) || C.MinDamage > C.MaxDamage) { E = TEXT("Damage range is invalid."); return false; }
    if (!ValidNonNegative(C.Defense) || !ValidNonNegative(C.MagicDefense) || !ValidNonNegative(C.AttackSpeed) || C.AttackSpeed <= 0.0 || !ValidNonNegative(C.MoveSpeed) || C.MoveSpeed <= 0.0) { E = TEXT("Combat defensive/speed values are invalid."); return false; }
    return true;
}
bool ValidAI(const FAetherAIProfile& A, FString& E)
{
    if (!ValidNonNegative(A.AggroRange) || !ValidNonNegative(A.LeashRange) || !ValidNonNegative(A.AttackRange) || A.AttackRange <= 0.0) { E = TEXT("AI ranges are invalid."); return false; }
    if (!FMath::IsFinite(A.FleeHealthPercent) || A.FleeHealthPercent < 0.0 || A.FleeHealthPercent > 1.0) { E = TEXT("FleeHealthPercent must be in [0,1]."); return false; }
    if (A.BehaviorProfileID.TrimStartAndEnd().IsEmpty() && A.Behavior != EAetherAIBehavior::Passive) { E = TEXT("Non-passive AI requires a behavior profile ID."); return false; }
    return true;
}
}

bool FAetherMonsterDefinition::IsStructurallyValid(FString& OutError) const
{
    OutError.Reset();
    if (DefinitionID.TrimStartAndEnd().IsEmpty() || DisplayName.TrimStartAndEnd().IsEmpty()) { OutError = TEXT("Monster ID and DisplayName are required."); return false; }
    if (!ValidCombat(Combat, OutError) || !ValidAI(AI, OutError)) return false;
    if (!ValidNonNegative(ExperienceReward) || CurrencyReward < 0) { OutError = TEXT("Monster rewards cannot be negative/non-finite."); return false; }
    if (bCanRespawn && (!FMath::IsFinite(RespawnSeconds) || RespawnSeconds < 0.0)) { OutError = TEXT("RespawnSeconds is invalid."); return false; }
    return true;
}

bool FAetherNPCDefinition::IsStructurallyValid(FString& OutError) const
{
    OutError.Reset();
    if (DefinitionID.TrimStartAndEnd().IsEmpty() || DisplayName.TrimStartAndEnd().IsEmpty()) { OutError = TEXT("NPC ID and DisplayName are required."); return false; }
    if (Level <= 0) { OutError = TEXT("NPC Level must be positive."); return false; }
    return true;
}

bool FAetherBossDefinition::IsStructurallyValid(FString& OutError) const
{
    OutError.Reset();
    if (DefinitionID.TrimStartAndEnd().IsEmpty() || DisplayName.TrimStartAndEnd().IsEmpty()) { OutError = TEXT("Boss ID and DisplayName are required."); return false; }
    if (!ValidCombat(Combat, OutError) || !ValidAI(AI, OutError)) return false;
    if (AI.Behavior != EAetherAIBehavior::Boss) { OutError = TEXT("Boss AI behavior must be Boss."); return false; }
    if (!ValidNonNegative(ExperienceReward) || CurrencyReward < 0 || !ValidNonNegative(RespawnSeconds)) { OutError = TEXT("Boss rewards/respawn values are invalid."); return false; }
    if (Phases.Num() <= 0) { OutError = TEXT("Boss requires at least one phase."); return false; }
    TSet<int32> PhaseNumbers;
    for (const FAetherBossPhase& P : Phases)
    {
        if (P.Phase <= 0 || PhaseNumbers.Contains(P.Phase)) { OutError = TEXT("Boss phases must have unique positive numbers."); return false; }
        PhaseNumbers.Add(P.Phase);
        if (!FMath::IsFinite(P.TriggerValue) || P.TriggerValue < 0.0) { OutError = TEXT("Boss phase TriggerValue is invalid."); return false; }
        if (P.Trigger == EAetherBossPhaseTrigger::HealthPercent && P.TriggerValue > 1.0) { OutError = TEXT("HealthPercent TriggerValue must be in [0,1]."); return false; }
    }
    return true;
}
