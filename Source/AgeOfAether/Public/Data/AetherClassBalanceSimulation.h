#pragma once
#include "CoreMinimal.h"
#include "Data/AetherClassBalanceRegistry.h"
#include "AetherClassBalanceSimulation.generated.h"

USTRUCT(BlueprintType)
struct FAetherBalanceSimulationCase
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadOnly) FString ClassID;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) FString EvolutionID;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) FString TargetClassID;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) FString TargetEvolutionID;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) double BaseDamage = 100.0;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) double TargetDefense = 0.0;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) double TargetResistancePercent = 0.0;
};

USTRUCT(BlueprintType)
struct FAetherBalanceSimulationResult
{
    GENERATED_BODY()
    UPROPERTY(BlueprintReadOnly) FString ClassID;
    UPROPERTY(BlueprintReadOnly) FString EvolutionID;
    UPROPERTY(BlueprintReadOnly) bool bPvP = false;
    UPROPERTY(BlueprintReadOnly) double OutgoingMultiplier = 1.0;
    UPROPERTY(BlueprintReadOnly) double IncomingMultiplier = 1.0;
    UPROPERTY(BlueprintReadOnly) double EffectiveDamage = 0.0;
    UPROPERTY(BlueprintReadOnly) bool bFinite = false;
    UPROPERTY(BlueprintReadOnly) bool bWithinSafetyBounds = false;
};

struct FAetherBalanceSimulationReport
{
    int32 CasesExecuted = 0;
    int32 Failures = 0;
    double MinimumDamage = TNumericLimits<double>::Max();
    double MaximumDamage = 0.0;
    bool bDeterministic = true;
    bool bFinite = true;
    bool bSafetyBoundsPassed = true;
    bool bProductionMutationAttempted = false;
    TArray<FString> Issues;
};

class FAetherClassBalanceSimulation
{
public:
    static bool Simulate(
        const FAetherClassBalanceRegistry& Registry,
        const TArray<FAetherBalanceSimulationCase>& Cases,
        bool bPvP,
        TArray<FAetherBalanceSimulationResult>& OutResults,
        FAetherBalanceSimulationReport& OutReport);

    static bool SimulateBothContexts(
        const FAetherClassBalanceRegistry& Registry,
        const TArray<FAetherBalanceSimulationCase>& Cases,
        TArray<FAetherBalanceSimulationResult>& OutResults,
        FAetherBalanceSimulationReport& OutPvEReport,
        FAetherBalanceSimulationReport& OutPvPReport);

    static bool ValidateExtremeMultipliers(const FAetherClassBalanceRegistry& Registry, const TArray<FAetherBalanceSimulationCase>& Cases, bool bPvP, FString& OutError);

    static bool ValidateSymmetryAndDeterminism(
        const FAetherClassBalanceRegistry& Registry,
        const TArray<FAetherBalanceSimulationCase>& Cases,
        FString& OutError);
};
