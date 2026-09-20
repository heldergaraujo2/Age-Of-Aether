#pragma once
#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "Scale/AetherPerformanceTypes.h"
#include "AetherPerformanceSubsystem.generated.h"

UCLASS()
class AGEOFAETHER_API UAetherPerformanceSubsystem:public UWorldSubsystem
{
 GENERATED_BODY()
public:
 bool Configure(const FAetherPerformanceBudget& InBudget);
 bool SubmitSnapshot(const FAetherPerformanceSnapshot& Snapshot);
 UFUNCTION(BlueprintPure,Category="Age of Aether|Performance") bool IsWithinBudget()const{return bWithinBudget;}
 UFUNCTION(BlueprintPure,Category="Age of Aether|Performance") FAetherPerformanceSnapshot GetLastSnapshot()const{return LastSnapshot;}
 UFUNCTION(BlueprintPure,Category="Age of Aether|Performance") FAetherPerformanceBudget GetBudget()const{return Budget;}
 void Reset();
private:
 FAetherPerformanceBudget Budget;
 FAetherPerformanceSnapshot LastSnapshot;
 bool bWithinBudget=true;
};
