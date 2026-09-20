#include "Presentation/AetherClassPresentationSubsystem.h"
FString UAetherClassPresentationSubsystem::Key(const FString& C,const FString& E){FString R=C.TrimStartAndEnd()+TEXT("|")+E.TrimStartAndEnd();R.ToLowerInline();return R;}
void UAetherClassPresentationSubsystem::ResetClassPresentation(){Definitions.Reset();States.Reset();}
void UAetherClassPresentationSubsystem::SetPresentation(const FAetherClassPresentationDefinition& D){FString E;if(!D.IsValid(E))return;const FString K=Key(D.ClassID,D.EvolutionID);Definitions.Add(K,D);States.Add(K,EAetherClassPresentationLoadState::NotRequested);}
bool UAetherClassPresentationSubsystem::ResolvePresentation(const FString& C,const FString& E,FAetherClassPresentationBinding& O) const
{
 const auto* D=Definitions.Find(Key(C,E)); if(!D)return false; O.ClassID=D->ClassID;O.EvolutionID=D->EvolutionID;O.MeshAssetID=D->MeshAssetID;O.AnimationSetAssetID=D->AnimationSetAssetID;O.IconAssetID=D->IconAssetID;O.SelectionWidgetAssetID=D->SelectionWidgetAssetID;O.TransitionVFXAssetID=D->TransitionVFXAssetID;O.TransitionSFXAssetID=D->TransitionSFXAssetID; return true;
}
void UAetherClassPresentationSubsystem::SetLoadState(const FString& C,const FString& E,EAetherClassPresentationLoadState S){const FString K=Key(C,E);if(!Definitions.Contains(K))return;States.Add(K,S);OnPresentationChanged.Broadcast(C,E,S);}
EAetherClassPresentationLoadState UAetherClassPresentationSubsystem::GetLoadState(const FString& C,const FString& E) const{const auto* S=States.Find(Key(C,E));return S?*S:EAetherClassPresentationLoadState::NotRequested;}
