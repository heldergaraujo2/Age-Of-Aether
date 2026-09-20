#include "Characters/AetherClassEvolutionPresentationComponent.h"
#include "Characters/AetherCharacter.h"
#include "Characters/AetherClassEvolutionPresentationCatalog.h"
#include "Characters/AetherPlayableCharacterVisualComponent.h"
UAetherClassEvolutionPresentationComponent::UAetherClassEvolutionPresentationComponent(){PrimaryComponentTick.bCanEverTick=false;}
UAetherClassEvolutionPresentationCatalog* UAetherClassEvolutionPresentationComponent::GetOrCreateCatalog(){if(!Catalog)Catalog=NewObject<UAetherClassEvolutionPresentationCatalog>(this);return Catalog;}
bool UAetherClassEvolutionPresentationComponent::ApplyClassEvolution(FName ClassID,FName EvolutionID)
{
 if(GetNetMode()==NM_DedicatedServer||ClassID.IsNone()||EvolutionID.IsNone())return false;
 auto* Active=GetOrCreateCatalog();const auto* Exact=Active->Find(ClassID,EvolutionID);const auto* Def=Active->FindWithFallback(ClassID,EvolutionID);if(!Def)return false;
 bUsingFallback=Exact!=Def;
 if(AAetherCharacter* Character=Cast<AAetherCharacter>(GetOwner())){if(auto* Visual=Character->GetVisualComponent()){if(Def->VisualProfile)Visual->ApplyProfileAsset(Def->VisualProfile); if(Def->AnimationProfile)Visual->ApplyAnimationProfile(Def->AnimationProfile);}}
 AppliedClassID=Def->ClassID;AppliedEvolutionID=Def->EvolutionID;return true;
}