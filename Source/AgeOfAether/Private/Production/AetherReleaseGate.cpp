#include "Production/AetherReleaseGate.h"

bool FAetherReleaseManifest::IsValid() const
{
    return !Version.TrimStartAndEnd().IsEmpty()
        && !BuildId.TrimStartAndEnd().IsEmpty()
        && !ContentPackageID.TrimStartAndEnd().IsEmpty()
        && !ContentVersion.TrimStartAndEnd().IsEmpty();
}

void UAetherReleaseGate::Configure(const FAetherReleaseManifest& InManifest)
{
    Manifest = InManifest;
    Checks.Reset();
    for (uint8 I = 0; I <= static_cast<uint8>(EAetherReleaseCheck::Audio); ++I)
    {
        FAetherReleaseCheckResult Result;
        Result.Check = static_cast<EAetherReleaseCheck>(I);
        Result.bPassed = false;
        Checks.Add(Result);
    }
}

bool UAetherReleaseGate::SetCheck(EAetherReleaseCheck Check, bool bPassed, const FString& Details)
{
    if (!Manifest.IsValid()) return false;
    for (FAetherReleaseCheckResult& Result : Checks)
    {
        if (Result.Check == Check)
        {
            Result.bPassed = bPassed;
            Result.Details = Details;
            return true;
        }
    }
    return false;
}

bool UAetherReleaseGate::IsReadyForStage(EAetherReleaseStage TargetStage) const
{
    if (!Manifest.IsValid()) return false;

    const uint8 Target = static_cast<uint8>(TargetStage);
    if (Target <= static_cast<uint8>(EAetherReleaseStage::Development)) return true;

    for (const FAetherReleaseCheckResult& Result : Checks)
    {
        if (!Result.bPassed) return false;
    }
    return true;
}

bool UAetherReleaseGate::IsFullyPassed() const
{
    return IsReadyForStage(EAetherReleaseStage::Release);
}
