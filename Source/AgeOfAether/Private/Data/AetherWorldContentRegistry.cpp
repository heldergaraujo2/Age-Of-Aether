#include "Data/AetherWorldContentRegistry.h"
#include "Data/AetherContentRegistry.h"
#include "Data/AetherAssetRegistry.h"
#include "Data/AetherItemRegistry.h"
#include "Data/AetherWorldActorRegistry.h"
#include "Data/AetherLootRewardRegistry.h"
#include "Data/AetherQuestDialogueEventRegistry.h"
#include "Data/AetherRecipeRegistry.h"
#include "Data/AetherSkillEffectRegistry.h"

namespace { FString N(const FString& ID){return ID.TrimStartAndEnd();} }

void FAetherWorldContentRegistry::AddIssue(TArray<FAetherWorldContentValidationIssue>& O,const FString& ID,const FString& C,const FString& M)const
{auto&I=O.AddDefaulted_GetRef();I.DefinitionID=ID;I.Code=C;I.Message=M;}

bool FAetherWorldContentRegistry::RegisterMap(const FAetherMapDefinition& D,FString& E)
{
    E.Reset(); FAetherMapDefinition C=D; C.DefinitionID=N(C.DefinitionID);
    if(!C.IsStructurallyValid(E))return false;
    if(C.DefinitionID.IsEmpty()||Maps.Contains(C.DefinitionID)){E=FString::Printf(TEXT("Duplicate or empty map DefinitionID '%s'."),*C.DefinitionID);return false;}
    TSet<FString> Seen;
    for(const auto& Cell:C.StreamingCells){const FString X=N(Cell.DefinitionID);if(Seen.Contains(X)){E=FString::Printf(TEXT("Duplicate streaming cell ID '%s'."),*X);return false;}Seen.Add(X);}
    Seen.Reset(); for(const auto& P:C.Points){const FString X=N(P.DefinitionID);if(Seen.Contains(X)){E=FString::Printf(TEXT("Duplicate world point ID '%s'."),*X);return false;}Seen.Add(X);}
    Seen.Reset(); for(const auto& Z:C.Zones){const FString X=N(Z.DefinitionID);if(Seen.Contains(X)){E=FString::Printf(TEXT("Duplicate zone ID '%s'."),*X);return false;}Seen.Add(X);}
    Seen.Reset(); for(const auto& A:C.ActorPlacements){const FString X=N(A.DefinitionID);if(Seen.Contains(X)){E=FString::Printf(TEXT("Duplicate actor placement ID '%s'."),*X);return false;}Seen.Add(X);}
    Seen.Reset(); for(const auto& I:C.Interactions){const FString X=N(I.DefinitionID);if(Seen.Contains(X)){E=FString::Printf(TEXT("Duplicate interaction ID '%s'."),*X);return false;}Seen.Add(X);}
    Seen.Reset(); for(const auto& X:C.Connections){const FString ID=N(X.DefinitionID);if(Seen.Contains(ID)){E=FString::Printf(TEXT("Duplicate connection ID '%s'."),*ID);return false;}Seen.Add(ID);}
    Maps.Add(C.DefinitionID,MoveTemp(C)); return true;
}

bool FAetherWorldContentRegistry::ResolveMap(const FString& ID,FAetherMapDefinition& O)const{const auto*F=Maps.Find(N(ID));if(!F)return false;O=*F;return true;}
bool FAetherWorldContentRegistry::ContainsMap(const FString& ID)const{return Maps.Contains(N(ID));}
void FAetherWorldContentRegistry::GetMapIDs(TArray<FString>& O)const{Maps.GetKeys(O);O.Sort();}
void FAetherWorldContentRegistry::Reset(){Maps.Reset();}

bool FAetherWorldContentRegistry::Validate(TArray<FAetherWorldContentValidationIssue>& O,const FAetherContentRegistry* CR,const FAetherAssetRegistry* AR,const FAetherItemRegistry* IR,const FAetherWorldActorRegistry* WR,const FAetherLootRewardRegistry* LR,const FAetherQuestDialogueEventRegistry* QR,const FAetherRecipeRegistry* RR,const FAetherSkillEffectRegistry* SR)const
{
    O.Reset();
    auto Issue=[&](const FString&ID,const FString&C,const FString&M){AddIssue(O,ID,C,M);};
    auto Asset=[&](const FString&Owner,const FString&ID){if(AR&&!N(ID).IsEmpty()&&!AR->Contains(ID))Issue(Owner,TEXT("MissingAssetReference"),FString::Printf(TEXT("Missing asset '%s'."),*ID));};
    auto Actor=[&](const FString&Owner,const FString&ID){if(WR&&!WR->Contains(ID))Issue(Owner,TEXT("MissingWorldActorReference"),FString::Printf(TEXT("Missing world actor '%s'."),*ID));};
    auto Item=[&](const FString&Owner,const FString&ID){if(IR&&!IR->Contains(ID))Issue(Owner,TEXT("MissingItemReference"),FString::Printf(TEXT("Missing item '%s'."),*ID));};
    auto Quest=[&](const FString&Owner,const FString&ID){if(QR&&!QR->Contains(ID))Issue(Owner,TEXT("MissingQuestReference"),FString::Printf(TEXT("Missing quest/dialogue/event '%s'."),*ID));};
    auto Recipe=[&](const FString&Owner,const FString&ID){if(RR&&!RR->Contains(ID))Issue(Owner,TEXT("MissingRecipeReference"),FString::Printf(TEXT("Missing recipe '%s'."),*ID));};
    auto Reward=[&](const FString&Owner,const FString&ID){if(LR&&!LR->Contains(ID))Issue(Owner,TEXT("MissingRewardReference"),FString::Printf(TEXT("Missing reward/loot '%s'."),*ID));};
    auto Skill=[&](const FString&Owner,const FString&ID){if(SR){FAetherDataSkillDefinition S;if(!SR->ResolveSkill(ID,S))Issue(Owner,TEXT("MissingSkillReference"),FString::Printf(TEXT("Missing skill '%s'."),*ID));}};
    auto Spawn=[&](const FString&Owner,const FString&ID){if(LR&&!LR->Contains(ID))Issue(Owner,TEXT("MissingSpawnGroupReference"),FString::Printf(TEXT("Missing spawn group '%s'."),*ID));};

    for(const auto& Pair:Maps)
    {
        const auto& D=Pair.Value; FString E; if(!D.IsStructurallyValid(E))Issue(D.DefinitionID,TEXT("InvalidDefinition"),E);
        TSet<FIntPoint> CellCoords;
        TSet<FString> CellIDs,PointIDs,ZoneIDs,PlacementIDs,InteractionIDs,ConnectionIDs;
        for(const auto& C:D.StreamingCells){const FString ID=N(C.DefinitionID);if(CellIDs.Contains(ID))Issue(D.DefinitionID,TEXT("DuplicateStreamingCellID"),ID);CellIDs.Add(ID);if(CellCoords.Contains(C.Coordinates))Issue(D.DefinitionID,TEXT("DuplicateStreamingCellCoordinates"),C.DefinitionID);CellCoords.Add(C.Coordinates);if(C.MinBounds.X<D.MinBounds.X||C.MinBounds.Y<D.MinBounds.Y||C.MaxBounds.X>D.MaxBounds.X||C.MaxBounds.Y>D.MaxBounds.Y)Issue(D.DefinitionID,TEXT("StreamingCellOutsideMap"),C.DefinitionID); if((C.MaxBounds.X-C.MinBounds.X)>D.StreamingCellSize||(C.MaxBounds.Y-C.MinBounds.Y)>D.StreamingCellSize)Issue(D.DefinitionID,TEXT("StreamingCellTooLarge"),C.DefinitionID);for(const auto&A:C.AssetIDs)Asset(D.DefinitionID,A);}
        for(const auto& Z:D.Zones){const FString ID=N(Z.DefinitionID);if(ZoneIDs.Contains(ID))Issue(D.DefinitionID,TEXT("DuplicateZoneID"),ID);ZoneIDs.Add(ID);if(Z.MinBounds.X<D.MinBounds.X||Z.MinBounds.Y<D.MinBounds.Y||Z.MaxBounds.X>D.MaxBounds.X||Z.MaxBounds.Y>D.MaxBounds.Y)Issue(D.DefinitionID,TEXT("ZoneOutsideMap"),Z.DefinitionID);}
        for(const auto& P:D.Points){const FString ID=N(P.DefinitionID);if(PointIDs.Contains(ID))Issue(D.DefinitionID,TEXT("DuplicatePointID"),ID);PointIDs.Add(ID);if(!P.StreamingCellID.IsEmpty()&&!CellIDs.Contains(N(P.StreamingCellID)))Issue(D.DefinitionID,TEXT("MissingStreamingCellReference"),P.StreamingCellID);if(P.Location.X<D.MinBounds.X||P.Location.X>D.MaxBounds.X||P.Location.Y<D.MinBounds.Y||P.Location.Y>D.MaxBounds.Y)Issue(D.DefinitionID,TEXT("PointOutsideMap"),P.DefinitionID);}
        if(!D.DefaultSpawnPointID.IsEmpty()&&!PointIDs.Contains(N(D.DefaultSpawnPointID)))Issue(D.DefinitionID,TEXT("MissingDefaultSpawnPoint"),D.DefaultSpawnPointID);
        for(const auto& C:D.Connections){const FString ID=N(C.DefinitionID);if(ConnectionIDs.Contains(ID))Issue(D.DefinitionID,TEXT("DuplicateConnectionID"),ID);ConnectionIDs.Add(ID);if(!PointIDs.Contains(N(C.SourcePointID)))Issue(D.DefinitionID,TEXT("MissingSourcePoint"),C.SourcePointID);if(!Maps.Contains(N(C.TargetMapID)))Issue(D.DefinitionID,TEXT("MissingTargetMap"),C.TargetMapID);if(Maps.Contains(N(C.TargetMapID))){const auto*Target=Maps.Find(N(C.TargetMapID));if(Target&&!C.TargetPointID.IsEmpty()){bool Found=false;for(const auto& TP:Target->Points)if(N(TP.DefinitionID)==N(C.TargetPointID)){Found=true;break;}if(!Found)Issue(D.DefinitionID,TEXT("MissingTargetPoint"),C.TargetPointID);}}for(const auto&R:C.Requirements){FString E2;if(!R.IsStructurallyValid(E2))Issue(D.DefinitionID,TEXT("InvalidConnectionRequirement"),E2);}}
        for(const auto&A:D.ActorPlacements){const FString ID=N(A.DefinitionID);if(PlacementIDs.Contains(ID))Issue(D.DefinitionID,TEXT("DuplicateActorPlacementID"),ID);PlacementIDs.Add(ID);Actor(D.DefinitionID,A.WorldActorID);if(!A.StreamingCellID.IsEmpty()&&!CellIDs.Contains(N(A.StreamingCellID)))Issue(D.DefinitionID,TEXT("MissingStreamingCellReference"),A.StreamingCellID);if(!A.SpawnGroupID.IsEmpty())Spawn(D.DefinitionID,A.SpawnGroupID);for(const auto&CID:A.ContentIDs)if(CR&&!CR->Contains(CID))Issue(D.DefinitionID,TEXT("MissingContentReference"),CID);}
        for(const auto&I:D.Interactions){const FString ID=N(I.DefinitionID);if(InteractionIDs.Contains(ID))Issue(D.DefinitionID,TEXT("DuplicateInteractionID"),ID);InteractionIDs.Add(ID);if(!I.StreamingCellID.IsEmpty()&&!CellIDs.Contains(N(I.StreamingCellID)))Issue(D.DefinitionID,TEXT("MissingStreamingCellReference"),I.StreamingCellID);if(!I.WorldActorID.IsEmpty())Actor(D.DefinitionID,I.WorldActorID);if(!I.PointID.IsEmpty()&&!PointIDs.Contains(N(I.PointID)))Issue(D.DefinitionID,TEXT("MissingInteractionPoint"),I.PointID);for(const auto&R:I.Requirements){FString RequirementError;if(!R.IsStructurallyValid(RequirementError))Issue(D.DefinitionID,TEXT("InvalidInteractionRequirement"),RequirementError);if(R.Type==EAetherWorldRequirementType::ItemOwned)Item(D.DefinitionID,R.ReferenceID);if(R.Type==EAetherWorldRequirementType::QuestCompleted)Quest(D.DefinitionID,R.ReferenceID); if(R.Type==EAetherWorldRequirementType::SkillLevelAtLeast)Skill(D.DefinitionID,R.ReferenceID);}for(const auto&Out:I.Outcomes){FString OutcomeError;if(!Out.IsStructurallyValid(OutcomeError))Issue(D.DefinitionID,TEXT("InvalidInteractionOutcome"),OutcomeError);if(Out.Type==EAetherWorldOutcomeType::GrantReward)Reward(D.DefinitionID,Out.ReferenceID);if(Out.Type==EAetherWorldOutcomeType::GrantQuest||Out.Type==EAetherWorldOutcomeType::CompleteQuest||Out.Type==EAetherWorldOutcomeType::StartEvent)Quest(D.DefinitionID,Out.ReferenceID);if(Out.Type==EAetherWorldOutcomeType::StartRecipe)Recipe(D.DefinitionID,Out.ReferenceID);if(Out.Type==EAetherWorldOutcomeType::Teleport&&(!Maps.Contains(N(Out.TargetMapID))))Issue(D.DefinitionID,TEXT("MissingTeleportTargetMap"),Out.TargetMapID);if(Out.Type==EAetherWorldOutcomeType::Teleport&&Maps.Contains(N(Out.TargetMapID))){const auto*T=Maps.Find(N(Out.TargetMapID));bool Found=false;for(const auto&P:T->Points)if(N(P.DefinitionID)==N(Out.TargetPointID)){Found=true;break;}if(!Found)Issue(D.DefinitionID,TEXT("MissingTeleportTargetPoint"),Out.TargetPointID);}}}
        for(const auto&SID:D.SpawnGroupIDs)Spawn(D.DefinitionID,SID);
        for(const auto&QID:D.WorldContentIDs)Quest(D.DefinitionID,QID);
        for(const auto&RID:D.RecipeIDs)Recipe(D.DefinitionID,RID);
        for(const auto&AssetID:D.AssetIDs)Asset(D.DefinitionID,AssetID);
    }
    return O.Num()==0;
}
