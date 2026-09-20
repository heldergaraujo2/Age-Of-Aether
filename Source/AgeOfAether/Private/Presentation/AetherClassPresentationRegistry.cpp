#include "Presentation/AetherClassPresentationRegistry.h"
FString FAetherClassPresentationRegistry::Normalize(const FString& Value){FString R=Value.TrimStartAndEnd();R.ToLowerInline();return R;}
FString FAetherClassPresentationRegistry::Key(const FString& ClassID,const FString& EvolutionID){return Normalize(ClassID)+TEXT("|")+Normalize(EvolutionID);}
bool FAetherClassPresentationRegistry::Register(const FAetherClassPresentationDefinition& Definition,FString& OutError)
{
    OutError.Reset(); if(!Definition.IsValid(OutError)) return false;
    const FString K=Key(Definition.ClassID,Definition.EvolutionID);
    if(Definitions.Contains(K)){OutError=TEXT("Duplicate class presentation.");return false;}
    Definitions.Add(K,Definition); return true;
}
bool FAetherClassPresentationRegistry::Resolve(const FString& ClassID,const FString& EvolutionID,FAetherClassPresentationDefinition& OutDefinition) const
{
    const auto* D=Definitions.Find(Key(ClassID,EvolutionID)); if(!D) return false; OutDefinition=*D; return true;
}
bool FAetherClassPresentationRegistry::ResolveWithFallback(const FString& ClassID,const FString& EvolutionID,FAetherClassPresentationDefinition& OutDefinition,bool& bOutFallback) const
{
    bOutFallback=false;
    if(Resolve(ClassID,EvolutionID,OutDefinition)) return true;
    const FString Requested=Key(ClassID,EvolutionID);
    for(const auto& Pair:Definitions)
    {
        const auto& D=Pair.Value;
        if(Key(D.ClassID,D.EvolutionID)!=Requested && Normalize(D.FallbackPresentationID)==Requested)
        { OutDefinition=D; bOutFallback=true; return true; }
    }
    return false;
}
bool FAetherClassPresentationRegistry::Validate(TArray<FAetherClassPresentationValidationIssue>& OutIssues) const
{
    OutIssues.Reset();
    for(const auto& Pair:Definitions)
    {
        FString E; if(!Pair.Value.IsValid(E)) OutIssues.Add({Pair.Key,TEXT("INVALID"),E});
        if(!Pair.Value.FallbackPresentationID.IsEmpty() && !Definitions.Contains(Normalize(Pair.Value.FallbackPresentationID)))
            OutIssues.Add({Pair.Key,TEXT("MISSING_FALLBACK"),TEXT("FallbackPresentationID does not resolve.")});
    }
    return OutIssues.Num()==0;
}
void FAetherClassPresentationRegistry::Reset(){Definitions.Reset();}
void FAetherClassPresentationRegistry::GetIDs(TArray<FString>& OutIDs) const{OutIDs.Reset();for(const auto& P:Definitions)OutIDs.Add(P.Key);OutIDs.Sort();}
