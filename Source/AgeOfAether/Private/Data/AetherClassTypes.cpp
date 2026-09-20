#include "Data/AetherClassTypes.h"
#include <cmath>

namespace
{
bool ValidText(const FString& V) { return !V.TrimStartAndEnd().IsEmpty(); }
bool ValidMap(const TMap<FString,double>& M)
{
    for (const auto& P : M) if (P.Key.TrimStartAndEnd().IsEmpty() || !FMath::IsFinite(P.Value)) return false;
    return true;
}
}

bool FAetherClassDefinition::IsStructurallyValid(FString& OutError) const
{
    if (!ValidText(ClassID) || !ValidText(DisplayName) || !ValidText(PathTitle)) { OutError=TEXT("ClassID, DisplayName and PathTitle are required."); return false; }
    if (StartingLevel < 1) { OutError=TEXT("StartingLevel must be >= 1."); return false; }
    if (!ValidMap(BaseStats.Values)) { OutError=TEXT("BaseStats contains an invalid entry."); return false; }
    return true;
}

bool FAetherClassEvolutionDefinition::IsStructurallyValid(FString& OutError) const
{
    if (!ValidText(EvolutionID) || !ValidText(ClassID) || !ValidText(DisplayName) || !ValidText(EvolutionTitle)) { OutError=TEXT("Evolution identity fields are required."); return false; }
    if (Stage < 1 || RequiredLevel < 1) { OutError=TEXT("Stage and RequiredLevel must be >= 1."); return false; }
    if (!ValidMap(StatModifiers.Values)) { OutError=TEXT("StatModifiers contains an invalid entry."); return false; }
    for (const FString& ID : PrerequisiteEvolutionIDs) if (!ValidText(ID)) { OutError=TEXT("Evolution prerequisites cannot be empty."); return false; }
    return true;
}
