#include "Data/AetherWorldActorRegistry.h"
#include "Data/AetherContentRegistry.h"
#include "Data/AetherAssetRegistry.h"

bool FAetherWorldActorRegistry::IsIDAvailable(const FString& ID) const
{
    const FString N = ID.TrimStartAndEnd();
    return !Monsters.Contains(N) && !NPCs.Contains(N) && !Bosses.Contains(N);
}
bool FAetherWorldActorRegistry::RegisterMonster(const FAetherMonsterDefinition& D, FString& E)
{
    E.Reset(); if (!D.IsStructurallyValid(E)) return false;
    const FString ID=D.DefinitionID.TrimStartAndEnd();
    if (!IsIDAvailable(ID)) { E=FString::Printf(TEXT("Duplicate world actor ID '%s'."),*ID); return false; }
    FAetherMonsterDefinition C=D; C.DefinitionID=ID; Monsters.Add(ID,MoveTemp(C)); return true;
}
bool FAetherWorldActorRegistry::RegisterNPC(const FAetherNPCDefinition& D, FString& E)
{
    E.Reset(); if (!D.IsStructurallyValid(E)) return false;
    const FString ID=D.DefinitionID.TrimStartAndEnd();
    if (!IsIDAvailable(ID)) { E=FString::Printf(TEXT("Duplicate world actor ID '%s'."),*ID); return false; }
    FAetherNPCDefinition C=D; C.DefinitionID=ID; NPCs.Add(ID,MoveTemp(C)); return true;
}
bool FAetherWorldActorRegistry::RegisterBoss(const FAetherBossDefinition& D, FString& E)
{
    E.Reset(); if (!D.IsStructurallyValid(E)) return false;
    const FString ID=D.DefinitionID.TrimStartAndEnd();
    if (!IsIDAvailable(ID)) { E=FString::Printf(TEXT("Duplicate world actor ID '%s'."),*ID); return false; }
    FAetherBossDefinition C=D; C.DefinitionID=ID; Bosses.Add(ID,MoveTemp(C)); return true;
}
bool FAetherWorldActorRegistry::ResolveMonster(const FString& ID, FAetherMonsterDefinition& O) const { const auto* F=Monsters.Find(ID.TrimStartAndEnd()); if(!F)return false; O=*F; return true; }
bool FAetherWorldActorRegistry::ResolveNPC(const FString& ID, FAetherNPCDefinition& O) const { const auto* F=NPCs.Find(ID.TrimStartAndEnd()); if(!F)return false; O=*F; return true; }
bool FAetherWorldActorRegistry::ResolveBoss(const FString& ID, FAetherBossDefinition& O) const { const auto* F=Bosses.Find(ID.TrimStartAndEnd()); if(!F)return false; O=*F; return true; }
bool FAetherWorldActorRegistry::Contains(const FString& ID) const { const FString N=ID.TrimStartAndEnd(); return Monsters.Contains(N)||NPCs.Contains(N)||Bosses.Contains(N); }

bool FAetherWorldActorRegistry::Validate(TArray<FAetherWorldActorValidationIssue>& OutIssues,const FAetherContentRegistry* CR,const FAetherAssetRegistry* AR) const
{
    OutIssues.Reset(); bool OK=true;
    auto CheckContent=[&](const FString& ID, EAetherContentType Type)
    {
        if(CR){ FAetherContentDefinition C; if(!CR->Resolve(ID,C)||C.Type!=Type){ AddIssue(OutIssues,ID,TEXT("MissingContentDefinition"),TEXT("Missing or wrong content type.")); OK=false; } }
    };
    auto CheckAsset=[&](const FString& ID)
    {
        if(AR && !ID.TrimStartAndEnd().IsEmpty() && !AR->Contains(ID)){ AddIssue(OutIssues,ID,TEXT("MissingAssetReference"),TEXT("Referenced asset does not exist.")); OK=false; }
    };
    TArray<FString> IDs; GetDefinitionIDs(IDs);
    for(const FString& ID:IDs)
    {
        if(const FAetherMonsterDefinition* M=Monsters.Find(ID))
        {
            FString E; if(!M->IsStructurallyValid(E)){AddIssue(OutIssues,ID,TEXT("InvalidDefinition"),E);OK=false;}
            CheckContent(ID,EAetherContentType::Monster); CheckAsset(M->SpawnAssetID); CheckAsset(M->CombatAssetID); CheckAsset(M->DeathEffectAssetID);
        }
        if(const FAetherNPCDefinition* N=NPCs.Find(ID))
        {
            FString E; if(!N->IsStructurallyValid(E)){AddIssue(OutIssues,ID,TEXT("InvalidDefinition"),E);OK=false;}
            CheckContent(ID,EAetherContentType::NPC); CheckAsset(N->SpawnAssetID); CheckAsset(N->IconAssetID);
        }
        if(const FAetherBossDefinition* B=Bosses.Find(ID))
        {
            FString E; if(!B->IsStructurallyValid(E)){AddIssue(OutIssues,ID,TEXT("InvalidDefinition"),E);OK=false;}
            CheckContent(ID,EAetherContentType::Monster); CheckAsset(B->SpawnAssetID); CheckAsset(B->CombatAssetID); CheckAsset(B->DeathEffectAssetID);
            for(const FAetherBossPhase& P:B->Phases) CheckAsset(P.VisualAssetID);
        }
    }
    return OK && OutIssues.Num()==0;
}
void FAetherWorldActorRegistry::GetDefinitionIDs(TArray<FString>& O) const
{
    O.Reset(); Monsters.GetKeys(O); TArray<FString> N; NPCs.GetKeys(N); O.Append(N); Bosses.GetKeys(N); O.Append(N); O.Sort();
}
void FAetherWorldActorRegistry::Reset(){Monsters.Reset();NPCs.Reset();Bosses.Reset();}
void FAetherWorldActorRegistry::AddIssue(TArray<FAetherWorldActorValidationIssue>& O,const FString& ID,const FString& Code,const FString& Msg) const
{
    auto& I=O.AddDefaulted_GetRef(); I.DefinitionID=ID; I.Code=Code; I.Message=Msg;
}
