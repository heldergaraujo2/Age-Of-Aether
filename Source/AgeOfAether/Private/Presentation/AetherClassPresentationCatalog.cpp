#include "Presentation/AetherClassPresentationCatalog.h"
#include "Data/AetherClassCatalog.h"
void FAetherClassPresentationCatalog::Build(TArray<FAetherClassPresentationDefinition>& OutDefinitions)
{
 OutDefinitions.Reset(); TArray<FAetherClassEvolutionDefinition> Evolutions; FAetherClassCatalog::BuildEvolutions(Evolutions);
 for(const auto& E:Evolutions){FAetherClassPresentationDefinition D;D.ClassID=E.ClassID;D.EvolutionID=E.EvolutionID;D.MeshAssetID=FString::Printf(TEXT("class.%s.mesh"),*E.ClassID);D.AnimationSetAssetID=FString::Printf(TEXT("class.%s.animations"),*E.ClassID);D.IconAssetID=FString::Printf(TEXT("class.%s.icon"),*E.EvolutionID);D.SelectionWidgetAssetID=TEXT("ui.class.selection");D.TransitionVFXAssetID=FString::Printf(TEXT("class.%s.transition.vfx"),*E.EvolutionID);D.TransitionSFXAssetID=FString::Printf(TEXT("class.%s.transition.sfx"),*E.EvolutionID);D.AllowedItemTags=E.AllowedItemTags;OutDefinitions.Add(D);}
}