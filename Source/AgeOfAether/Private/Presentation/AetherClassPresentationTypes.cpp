#include "Presentation/AetherClassPresentationTypes.h"
bool FAetherClassPresentationDefinition::IsValid(FString& OutError) const
{
    OutError.Reset();
    if (ClassID.TrimStartAndEnd().IsEmpty()) { OutError=TEXT("ClassID is required."); return false; }
    if (EvolutionID.TrimStartAndEnd().IsEmpty()) { OutError=TEXT("EvolutionID is required."); return false; }
    if (MeshAssetID.TrimStartAndEnd().IsEmpty()) { OutError=TEXT("MeshAssetID is required."); return false; }
    if (AnimationSetAssetID.TrimStartAndEnd().IsEmpty()) { OutError=TEXT("AnimationSetAssetID is required."); return false; }
    if (IconAssetID.TrimStartAndEnd().IsEmpty()) { OutError=TEXT("IconAssetID is required."); return false; }
    if (SelectionWidgetAssetID.TrimStartAndEnd().IsEmpty()) { OutError=TEXT("SelectionWidgetAssetID is required."); return false; }
    if (ContentVersion <= 0) { OutError=TEXT("ContentVersion must be positive."); return false; }
    const FString SelfKey = ClassID.TrimStartAndEnd() + TEXT("|") + EvolutionID.TrimStartAndEnd();
    if (FallbackPresentationID.TrimStartAndEnd().Equals(SelfKey,ESearchCase::IgnoreCase))
    { OutError=TEXT("Presentation cannot fallback to itself."); return false; }
    return true;
}
