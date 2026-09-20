#include "Data/AetherClassBalanceSimulation.h"

namespace
{
bool IsSafeDamage(double Value)
{
    return FMath::IsFinite(Value) && Value >= 0.0 && Value <= 1.0e12;
}

double ComputeDamage(double BaseDamage, double Defense, double ResistancePercent, const FAetherClassBalanceModifiers& Attacker, const FAetherClassBalanceModifiers& Target)
{
    const double DefenseValue = FMath::Max(0.0, Defense * Target.Defense);
    const double Mitigation = 100.0 / (100.0 + DefenseValue);
    const double Resistance = 1.0 - FMath::Clamp(ResistancePercent, 0.0, 75.0) / 100.0;
    return FMath::Max(0.0, BaseDamage * Attacker.Damage * Attacker.OutgoingDamage * Target.IncomingDamage * Mitigation * Resistance);
}
}

bool FAetherClassBalanceSimulation::Simulate(
    const FAetherClassBalanceRegistry& Registry,
    const TArray<FAetherBalanceSimulationCase>& Cases,
    bool bPvP,
    TArray<FAetherBalanceSimulationResult>& OutResults,
    FAetherBalanceSimulationReport& OutReport)
{
    OutResults.Reset();
    OutReport = FAetherBalanceSimulationReport();

    for (const FAetherBalanceSimulationCase& Case : Cases)
    {
        FAetherBalanceSimulationResult Result;
        Result.ClassID = Case.ClassID;
        Result.EvolutionID = Case.EvolutionID;
        Result.bPvP = bPvP;

        if (!FMath::IsFinite(Case.BaseDamage) || Case.BaseDamage < 0.0
            || !FMath::IsFinite(Case.TargetDefense) || Case.TargetDefense < 0.0
            || !FMath::IsFinite(Case.TargetResistancePercent))
        {
            Result.bWithinSafetyBounds = false;
            OutReport.Failures++;
            OutReport.Issues.Add(Case.ClassID + TEXT(": invalid simulation input."));
            OutResults.Add(Result);
            continue;
        }

        FAetherClassBalanceModifiers AttackerModifiers;
        FAetherClassBalanceModifiers TargetModifiers;
        FString Error;
        const FString TargetClassID = Case.TargetClassID.IsEmpty() ? Case.ClassID : Case.TargetClassID;
        const FString TargetEvolutionID = Case.TargetEvolutionID.IsEmpty() ? Case.EvolutionID : Case.TargetEvolutionID;
        if (!Registry.ResolveActive(Case.ClassID, Case.EvolutionID, bPvP, AttackerModifiers, Error))
        {
            OutReport.Failures++;
            OutReport.Issues.Add(Case.ClassID + TEXT(": ") + Error);
            OutResults.Add(Result);
            continue;
        }

        if (!Registry.ResolveActive(TargetClassID, TargetEvolutionID, bPvP, TargetModifiers, Error))
        {
            OutReport.Failures++;
            OutReport.Issues.Add(TargetClassID + TEXT(": ") + Error);
            OutResults.Add(Result);
            continue;
        }
        Result.OutgoingMultiplier = AttackerModifiers.Damage * AttackerModifiers.OutgoingDamage;
        Result.IncomingMultiplier = TargetModifiers.IncomingDamage;
        Result.EffectiveDamage = ComputeDamage(Case.BaseDamage, Case.TargetDefense, Case.TargetResistancePercent, AttackerModifiers, TargetModifiers);
        Result.bFinite = FMath::IsFinite(Result.EffectiveDamage);
        Result.bWithinSafetyBounds = Result.bFinite && IsSafeDamage(Result.EffectiveDamage);

        if (!Result.bFinite)
        {
            OutReport.bFinite = false;
            OutReport.Failures++;
            OutReport.Issues.Add(Case.ClassID + TEXT(": non-finite simulation result."));
        }
        if (!Result.bWithinSafetyBounds)
        {
            OutReport.bSafetyBoundsPassed = false;
            OutReport.Failures++;
            OutReport.Issues.Add(Case.ClassID + TEXT(": simulation result exceeded safety bounds."));
        }

        OutReport.CasesExecuted++;
        OutReport.MinimumDamage = FMath::Min(OutReport.MinimumDamage, Result.EffectiveDamage);
        OutReport.MaximumDamage = FMath::Max(OutReport.MaximumDamage, Result.EffectiveDamage);
        OutResults.Add(Result);
    }

    if (OutReport.CasesExecuted == 0)
    {
        OutReport.MinimumDamage = 0.0;
    }
    return OutReport.Failures == 0;
}

bool FAetherClassBalanceSimulation::SimulateBothContexts(
    const FAetherClassBalanceRegistry& Registry,
    const TArray<FAetherBalanceSimulationCase>& Cases,
    TArray<FAetherBalanceSimulationResult>& OutResults,
    FAetherBalanceSimulationReport& OutPvEReport,
    FAetherBalanceSimulationReport& OutPvPReport)
{
    TArray<FAetherBalanceSimulationResult> PvE;
    TArray<FAetherBalanceSimulationResult> PvP;
    const bool PvEOk = Simulate(Registry, Cases, false, PvE, OutPvEReport);
    const bool PvPOk = Simulate(Registry, Cases, true, PvP, OutPvPReport);
    OutResults = PvE;
    OutResults.Append(PvP);
    return PvEOk && PvPOk;
}

bool FAetherClassBalanceSimulation::ValidateExtremeMultipliers(
    const FAetherClassBalanceRegistry& Registry,
    const TArray<FAetherBalanceSimulationCase>& Cases,
    bool bPvP,
    FString& OutError)
{
    OutError.Reset();
    TArray<FAetherBalanceSimulationResult> Results;
    FAetherBalanceSimulationReport Report;
    if (!FAetherClassBalanceSimulation::Simulate(Registry, Cases, bPvP, Results, Report))
    {
        OutError = TEXT("Extreme multiplier simulation produced an unsafe or invalid result.");
        return false;
    }
    return Report.bFinite && Report.bSafetyBoundsPassed;
}

bool FAetherClassBalanceSimulation::ValidateSymmetryAndDeterminism(
    const FAetherClassBalanceRegistry& Registry,
    const TArray<FAetherBalanceSimulationCase>& Cases,
    FString& OutError)
{
    OutError.Reset();
    TArray<FAetherBalanceSimulationResult> First;
    TArray<FAetherBalanceSimulationResult> Second;
    FAetherBalanceSimulationReport A, B;
    if (!Simulate(Registry, Cases, false, First, A) || !Simulate(Registry, Cases, false, Second, B))
    {
        OutError = TEXT("Simulation failed while checking determinism.");
        return false;
    }
    if (First.Num() != Second.Num())
    {
        OutError = TEXT("Simulation result count is not deterministic.");
        return false;
    }
    for (int32 I = 0; I < First.Num(); ++I)
    {
        if (!FMath::IsNearlyEqual(First[I].EffectiveDamage, Second[I].EffectiveDamage, 1e-9))
        {
            OutError = TEXT("Simulation output is not deterministic.");
            return false;
        }
    }
    return true;
}
