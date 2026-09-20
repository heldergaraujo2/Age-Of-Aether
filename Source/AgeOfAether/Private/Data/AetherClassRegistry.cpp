#include "Data/AetherClassRegistry.h"
#include "Data/AetherItemRegistry.h"
#include "Data/AetherSkillEffectRegistry.h"
#include "Data/AetherQuestDialogueEventRegistry.h"
#include "Data/AetherContentRegistry.h"
#include "Data/AetherAssetRegistry.h"

FString FAetherClassRegistry::Normalize(const FString& ID){ return ID.TrimStartAndEnd().ToLower(); }
void FAetherClassRegistry::AddIssue(TArray<FAetherClassValidationIssue>& O,const FString& I,const FString& C,const FString& M){ FAetherClassValidationIssue X; X.DefinitionID=I; X.Code=C; X.Message=M; O.Add(X); }

bool FAetherClassRegistry::RegisterClass(const FAetherClassDefinition& D,FString& E)
{
    E.Reset(); FString ID=Normalize(D.ClassID); FString Err; if(!D.IsStructurallyValid(Err)){E=Err;return false;}
    if(Classes.Contains(ID)||Evolutions.Contains(ID)){E=TEXT("Duplicate class/content ID: ")+ID;return false;}
    FAetherClassDefinition C=D; C.ClassID=ID; Classes.Add(ID,C); return true;
}
bool FAetherClassRegistry::RegisterEvolution(const FAetherClassEvolutionDefinition& D,FString& E)
{
    E.Reset(); FString ID=Normalize(D.EvolutionID); FString Err; if(!D.IsStructurallyValid(Err)){E=Err;return false;}
    if(Evolutions.Contains(ID)||Classes.Contains(ID)){E=TEXT("Duplicate class/content ID: ")+ID;return false;}
    FAetherClassEvolutionDefinition X=D; X.EvolutionID=ID; X.ClassID=Normalize(D.ClassID); Evolutions.Add(ID,X); return true;
}
bool FAetherClassRegistry::ResolveClass(const FString& I,FAetherClassDefinition& O)const{const auto* P=Classes.Find(Normalize(I));if(!P)return false;O=*P;return true;}
bool FAetherClassRegistry::ResolveEvolution(const FString& I,FAetherClassEvolutionDefinition& O)const{const auto* P=Evolutions.Find(Normalize(I));if(!P)return false;O=*P;return true;}
bool FAetherClassRegistry::ContainsClass(const FString& I)const{return Classes.Contains(Normalize(I));}
bool FAetherClassRegistry::ContainsEvolution(const FString& I)const{return Evolutions.Contains(Normalize(I));}
bool FAetherClassRegistry::RemoveClass(const FString& I){return Classes.Remove(Normalize(I))>0;}
bool FAetherClassRegistry::RemoveEvolution(const FString& I){return Evolutions.Remove(Normalize(I))>0;}
void FAetherClassRegistry::Reset(){Classes.Reset();Evolutions.Reset();}
void FAetherClassRegistry::GetClassIDs(TArray<FString>& O)const{O.Reset();Classes.GetKeys(O);O.Sort();}
void FAetherClassRegistry::GetEvolutionIDs(TArray<FString>& O)const{O.Reset();Evolutions.GetKeys(O);O.Sort();}
void FAetherClassRegistry::GetEvolutionIDsForClass(const FString& I,TArray<FString>& O)const{O.Reset();const FString C=Normalize(I);for(const auto& P:Evolutions)if(P.Value.ClassID==C)O.Add(P.Key);O.Sort();}

bool FAetherClassRegistry::Validate(TArray<FAetherClassValidationIssue>& O,const FAetherItemRegistry* Items,const FAetherSkillEffectRegistry* Skills,const FAetherQuestDialogueEventRegistry* Quests,const FAetherContentRegistry* Content,const FAetherAssetRegistry* Assets)const
{
    O.Reset(); bool Good=true;
    for(const auto& P:Classes){
        const auto& C=P.Value; FString E; if(!C.IsStructurallyValid(E)){AddIssue(O,C.ClassID,TEXT("Invalid"),E);Good=false;}
        for(const FString& I:C.StartingItemIDs) if(Items && !Items->Contains(I)){AddIssue(O,C.ClassID,TEXT("MissingItem"),I);Good=false;}
        for(const FString& S:C.StartingSkillIDs) if(Skills && !Skills->ContainsSkill(S)){AddIssue(O,C.ClassID,TEXT("MissingSkill"),S);Good=false;}
    }
    for(const auto& P:Evolutions){
        const auto& E=P.Value; FString X; if(!E.IsStructurallyValid(X)){AddIssue(O,E.EvolutionID,TEXT("Invalid"),X);Good=false;}
        if(!Classes.Contains(E.ClassID)){AddIssue(O,E.EvolutionID,TEXT("MissingClass"),E.ClassID);Good=false;}
        for(const FString& R:E.PrerequisiteEvolutionIDs) if(!Evolutions.Contains(Normalize(R))){AddIssue(O,E.EvolutionID,TEXT("MissingPrerequisite"),R);Good=false;}
        for(const FString& Q:E.RequiredQuestIDs) if(Quests && !Quests->ContainsQuest(Q)){AddIssue(O,E.EvolutionID,TEXT("MissingQuest"),Q);Good=false;}
        for(const FString& C:E.RequiredContentIDs) if(Content && !Content->Contains(C)){AddIssue(O,E.EvolutionID,TEXT("MissingContent"),C);Good=false;}
        for(const FString& S:E.GrantedSkillIDs) if(Skills && !Skills->ContainsSkill(S)){AddIssue(O,E.EvolutionID,TEXT("MissingSkill"),S);Good=false;}
        for(const FString& F:E.GrantedEffectIDs) if(Skills && !Skills->ContainsEffect(F)){AddIssue(O,E.EvolutionID,TEXT("MissingEffect"),F);Good=false;}
        for(const FString& A:E.VisualAssetIDs) if(Assets && !Assets->Contains(A)){AddIssue(O,E.EvolutionID,TEXT("MissingAsset"),A);Good=false;}
    }
    for(const auto& P:Evolutions){
        const auto& E=P.Value;
        for(const auto& Q:Evolutions) if(P.Key!=Q.Key && E.ClassID==Q.Value.ClassID && E.Stage==Q.Value.Stage){AddIssue(O,E.EvolutionID,TEXT("DuplicateStage"),FString::FromInt(E.Stage));Good=false;break;}
        if(E.Stage>1 && E.PrerequisiteEvolutionIDs.Num()==0){AddIssue(O,E.EvolutionID,TEXT("MissingStagePrerequisite"),TEXT("Stage > 1 requires a prerequisite evolution."));Good=false;}
    }
    return Good;
}
