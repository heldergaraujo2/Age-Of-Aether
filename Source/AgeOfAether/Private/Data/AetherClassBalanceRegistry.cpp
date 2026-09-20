#include "Data/AetherClassBalanceRegistry.h"
#include "Data/AetherClassRegistry.h"
FString FAetherClassBalanceRegistry::Normalize(const FString& In){return In.TrimStartAndEnd().ToLower();}
void FAetherClassBalanceRegistry::AddIssue(TArray<FAetherBalanceValidationIssue>& I,const FString&S,const FString&C,const FString&M){FAetherBalanceValidationIssue X;X.ScopeID=S;X.Code=C;X.Message=M;I.Add(X);}
const FAetherBalanceProfileDefinition* FAetherClassBalanceRegistry::FindProfile(const FString& ID) const{return Profiles.Find(Normalize(ID));}
bool FAetherClassBalanceRegistry::RegisterProfile(const FAetherBalanceProfileDefinition& D,FString&E)
{
 E.Reset();FAetherBalanceProfileDefinition C=D;C.ProfileID=Normalize(C.ProfileID);if(C.ProfileID.IsEmpty()){E=TEXT("ProfileID is required.");return false;}if(C.Version<=0){E=TEXT("Profile version must be positive.");return false;}if(Profiles.Contains(C.ProfileID)){E=TEXT("Duplicate balance profile.");return false;}TSet<FString> Seen;
 for(const auto& B:C.Definitions){FAetherClassBalanceDefinition X=B;X.DefinitionID=Normalize(X.DefinitionID);if(!X.IsStructurallyValid(E))return false;if(Seen.Contains(X.DefinitionID)){E=TEXT("Duplicate balance definition ID.");return false;}Seen.Add(X.DefinitionID);}
 Profiles.Add(C.ProfileID,C);return true;
}
bool FAetherClassBalanceRegistry::SetActiveProfile(const FString& ID,FString&E){const FString N=Normalize(ID);if(!Profiles.Contains(N)){E=TEXT("Active profile does not exist.");return false;}ActiveProfileID=N;E.Reset();return true;}
bool FAetherClassBalanceRegistry::SetFallbackProfile(const FString& ID,FString&E){const FString N=Normalize(ID);if(!Profiles.Contains(N)){E=TEXT("Fallback profile does not exist.");return false;}FallbackProfileID=N;E.Reset();return true;}
bool FAetherClassBalanceRegistry::Resolve(const FString& P,const FString& C,const FString& Ev,bool bPvP,FAetherClassBalanceModifiers&O,FString&E) const
{
 E.Reset();O=FAetherClassBalanceModifiers();const FAetherBalanceProfileDefinition* Profile=FindProfile(P);if(!Profile){E=TEXT("Balance profile not found.");return false;}
 const FString CN=Normalize(C),EN=Normalize(Ev);FAetherClassBalanceModifiers ClassMod, EvoMod;
 bool FoundClass=false,FoundEvo=false;
 for(const auto& D:Profile->Definitions)
 {
  if(Normalize(D.ClassID)==CN&&D.EvolutionID.IsEmpty()){ClassMod=bPvP?D.PvP:D.PvE;FoundClass=true;}
  if(!EN.IsEmpty()&&Normalize(D.EvolutionID)==EN){EvoMod=bPvP?D.PvP:D.PvE;FoundEvo=true;}
 }
 if(!FoundClass){E=TEXT("Class balance definition not found.");return false;}
 O=FoundEvo?ClassMod.Multiply(EvoMod):ClassMod;
 if(!O.IsFinite()){E=TEXT("Resolved balance is not finite.");return false;}
 return true;
}
bool FAetherClassBalanceRegistry::Validate(TArray<FAetherBalanceValidationIssue>&I,const FAetherClassRegistry* R) const
{
 I.Reset();if(Profiles.Num()==0){AddIssue(I,TEXT("registry"),TEXT("empty"),TEXT("No balance profiles registered."));return false;}
 if(ActiveProfileID.IsEmpty())AddIssue(I,TEXT("registry"),TEXT("active_missing"),TEXT("Active profile is not set."));
 if(!FallbackProfileID.IsEmpty()&&!Profiles.Contains(FallbackProfileID))AddIssue(I,TEXT("registry"),TEXT("fallback_missing"),TEXT("Fallback profile does not exist."));
 for(const auto& Pair:Profiles)
 {
  const auto& P=Pair.Value;if(P.Version<=0)AddIssue(I,P.ProfileID,TEXT("version"),TEXT("Profile version must be positive."));
  TSet<FString> Defs;
  for(const auto& D:P.Definitions)
  {
   FString E;if(!D.IsStructurallyValid(E))AddIssue(I,P.ProfileID,TEXT("definition"),E);
   const FString ID=Normalize(D.DefinitionID);if(Defs.Contains(ID))AddIssue(I,P.ProfileID,TEXT("duplicate_definition"),ID);Defs.Add(ID);
   if(R)
   {
    if(!R->ContainsClass(D.ClassID))AddIssue(I,ID,TEXT("missing_class"),D.ClassID);
    if(!D.EvolutionID.IsEmpty()){FAetherClassEvolutionDefinition Evo;if(!R->ResolveEvolution(D.EvolutionID,Evo))AddIssue(I,ID,TEXT("missing_evolution"),D.EvolutionID);else if(Normalize(Evo.ClassID)!=Normalize(D.ClassID))AddIssue(I,ID,TEXT("evolution_class_mismatch"),D.EvolutionID);}
   }
  }
 }
 return I.Num()==0;
}
void FAetherClassBalanceRegistry::Reset(){Profiles.Reset();ActiveProfileID.Reset();FallbackProfileID=TEXT("production");}
int32 FAetherClassBalanceRegistry::NumProfiles() const{return Profiles.Num();}
bool FAetherClassBalanceRegistry::HasProfile(const FString& ID) const{return Profiles.Contains(Normalize(ID));}
