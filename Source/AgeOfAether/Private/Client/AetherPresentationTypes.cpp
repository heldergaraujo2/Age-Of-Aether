#include "Client/AetherPresentationTypes.h"

bool FAetherAssetRuntimeBinding::IsUsable() const
{
    return !AssetID.TrimStartAndEnd().IsEmpty()
        && !RuntimePath.TrimStartAndEnd().IsEmpty()
        && bResolved;
}

bool FAetherPresentationActorSnapshot::IsValid() const
{
    return !EntityID.TrimStartAndEnd().IsEmpty()
        && !DefinitionID.TrimStartAndEnd().IsEmpty()
        && MoveSpeed >= 0.0f
        && FMath::IsFinite(MoveSpeed)
        && NormalizedHealth >= 0.0f
        && NormalizedHealth <= 1.0f
        && FMath::IsFinite(NormalizedHealth);
}

bool FAetherPerformanceSample::IsWithinBudget() const
{
    return FMath::IsFinite(Value) && FMath::IsFinite(Budget) && Budget >= 0.0 && Value <= Budget;
}

void FAetherClientAcceptanceResult::Reset()
{
    bPassed = false;
    Diagnostic.Reset();
}
