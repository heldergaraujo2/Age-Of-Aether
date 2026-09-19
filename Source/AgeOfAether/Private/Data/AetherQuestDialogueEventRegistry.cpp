#include "Data/AetherQuestDialogueEventRegistry.h"
#include "Data/AetherContentRegistry.h"
#include "Data/AetherAssetRegistry.h"
#include "Data/AetherItemRegistry.h"
#include "Data/AetherWorldActorRegistry.h"
#include "Data/AetherLootRewardRegistry.h"

namespace { FString N(const FString& ID){ return ID.TrimStartAndEnd(); } }

void FAetherQuestDialogueEventRegistry::AddIssue(TArray<FAetherQuestDialogueEventValidationIssue>& OutIssues,const FString& ID,const FString& Code,const FString& Message) const
{ FAetherQuestDialogueEventValidationIssue I; I.DefinitionID=ID; I.Code=Code; I.Message=Message; OutIssues.Add(I); }

bool FAetherQuestDialogueEventRegistry::IsIDAvailable(const FString& ID) const
{ const FString X=N(ID); return !X.IsEmpty()&&!Quests.Contains(X)&&!Dialogues.Contains(X)&&!WorldEvents.Contains(X)&&!WorldContents.Contains(X); }

bool FAetherQuestDialogueEventRegistry::RegisterQuest(const FAetherQuestDefinition& D,FString& E)
{ E.Reset(); auto C=D; C.DefinitionID=N(C.DefinitionID); if(!C.IsStructurallyValid(E)||!IsIDAvailable(C.DefinitionID)){if(E.IsEmpty())E=TEXT("DefinitionID is already registered or invalid.");return false;} Quests.Add(C.DefinitionID,C);return true; }
bool FAetherQuestDialogueEventRegistry::RegisterDialogue(const FAetherDialogueDefinition& D,FString& E)
{ E.Reset(); auto C=D; C.DefinitionID=N(C.DefinitionID); if(!C.IsStructurallyValid(E)||!IsIDAvailable(C.DefinitionID)){if(E.IsEmpty())E=TEXT("DefinitionID is already registered or invalid.");return false;} Dialogues.Add(C.DefinitionID,C);return true; }
bool FAetherQuestDialogueEventRegistry::RegisterWorldEvent(const FAetherWorldEventDefinition& D,FString& E)
{ E.Reset(); auto C=D; C.DefinitionID=N(C.DefinitionID); if(!C.IsStructurallyValid(E)||!IsIDAvailable(C.DefinitionID)){if(E.IsEmpty())E=TEXT("DefinitionID is already registered or invalid.");return false;} WorldEvents.Add(C.DefinitionID,C);return true; }
bool FAetherQuestDialogueEventRegistry::RegisterWorldContent(const FAetherWorldContentDefinition& D,FString& E)
{ E.Reset(); auto C=D; C.DefinitionID=N(C.DefinitionID); if(!C.IsStructurallyValid(E)||!IsIDAvailable(C.DefinitionID)){if(E.IsEmpty())E=TEXT("DefinitionID is already registered or invalid.");return false;} WorldContents.Add(C.DefinitionID,C);return true; }

bool FAetherQuestDialogueEventRegistry::ResolveQuest(const FString& ID,FAetherQuestDefinition& O)const{auto*F=Quests.Find(N(ID));if(!F)return false;O=*F;return true;}
bool FAetherQuestDialogueEventRegistry::ResolveDialogue(const FString& ID,FAetherDialogueDefinition& O)const{auto*F=Dialogues.Find(N(ID));if(!F)return false;O=*F;return true;}
bool FAetherQuestDialogueEventRegistry::ResolveWorldEvent(const FString& ID,FAetherWorldEventDefinition& O)const{auto*F=WorldEvents.Find(N(ID));if(!F)return false;O=*F;return true;}
bool FAetherQuestDialogueEventRegistry::ResolveWorldContent(const FString& ID,FAetherWorldContentDefinition& O)const{auto*F=WorldContents.Find(N(ID));if(!F)return false;O=*F;return true;}
bool FAetherQuestDialogueEventRegistry::Contains(const FString& ID)const{const FString X=N(ID);return Quests.Contains(X)||Dialogues.Contains(X)||WorldEvents.Contains(X)||WorldContents.Contains(X);}
void FAetherQuestDialogueEventRegistry::GetDefinitionIDs(TArray<FString>& O)const{O.Reset();for(auto&P:Quests)O.Add(P.Key);for(auto&P:Dialogues)O.Add(P.Key);for(auto&P:WorldEvents)O.Add(P.Key);for(auto&P:WorldContents)O.Add(P.Key);O.Sort();}
void FAetherQuestDialogueEventRegistry::Reset(){Quests.Reset();Dialogues.Reset();WorldEvents.Reset();WorldContents.Reset();}

bool FAetherQuestDialogueEventRegistry::Validate(TArray<FAetherQuestDialogueEventValidationIssue>& Issues,const FAetherContentRegistry* C,const FAetherAssetRegistry*,const FAetherItemRegistry* I,const FAetherWorldActorRegistry* W,const FAetherLootRewardRegistry* L)const
{
 Issues.Reset();
 auto Content=[&](const FString& Owner,const FString& ID){if(C&&!C->Contains(ID))AddIssue(Issues,Owner,TEXT("MissingContentReference"),FString::Printf(TEXT("Missing content '%s'."),*ID));};
 auto Item=[&](const FString& Owner,const FString& ID){if(I&&!I->Contains(ID))AddIssue(Issues,Owner,TEXT("MissingItemReference"),FString::Printf(TEXT("Missing item '%s'."),*ID));};
 auto Actor=[&](const FString& Owner,const FString& ID){if(W&&!W->Contains(ID))AddIssue(Issues,Owner,TEXT("MissingWorldActorReference"),FString::Printf(TEXT("Missing world actor '%s'."),*ID));};
 auto Reward=[&](const FString& Owner,const FString& ID){if(L&&!L->Contains(ID))AddIssue(Issues,Owner,TEXT("MissingRewardReference"),FString::Printf(TEXT("Missing loot/reward '%s'."),*ID));};
 for(const auto&P:Quests){const auto&D=P.Value; if(W){if(!D.StartNPCID.IsEmpty()&&!W->Contains(D.StartNPCID))AddIssue(Issues,D.DefinitionID,TEXT("MissingNPCReference"),TEXT("StartNPCID is missing."));if(!D.TurnInNPCID.IsEmpty()&&!W->Contains(D.TurnInNPCID))AddIssue(Issues,D.DefinitionID,TEXT("MissingNPCReference"),TEXT("TurnInNPCID is missing."));} for(const auto&Q:D.PrerequisiteQuestIDs)if(!Quests.Contains(N(Q)))AddIssue(Issues,D.DefinitionID,TEXT("MissingQuestReference"),FString::Printf(TEXT("Missing prerequisite quest '%s'."),*Q)); for(const auto&Q:D.NextQuestIDs)if(!Quests.Contains(N(Q)))AddIssue(Issues,D.DefinitionID,TEXT("MissingQuestReference"),FString::Printf(TEXT("Missing next quest '%s'."),*Q)); for(const auto&R:D.RewardIDs)Reward(D.DefinitionID,R); for(const auto&O:D.Objectives){if(O.Type==EAetherQuestObjectiveType::Kill||O.Type==EAetherQuestObjectiveType::Talk||O.Type==EAetherQuestObjectiveType::Interact)Actor(D.DefinitionID,O.TargetID); if(O.Type==EAetherQuestObjectiveType::Collect||O.Type==EAetherQuestObjectiveType::Craft)Item(D.DefinitionID,O.TargetID); if(O.Type==EAetherQuestObjectiveType::WorldEvent&&!WorldEvents.Contains(N(O.EventID)))AddIssue(Issues,D.DefinitionID,TEXT("MissingWorldEventReference"),FString::Printf(TEXT("Missing world event '%s'."),*O.EventID));}}
 for(const auto&P:Dialogues){const auto&D=P.Value;for(const auto&Node:D.Nodes){if(W&&!Node.SpeakerNPCID.IsEmpty()&&!W->Contains(Node.SpeakerNPCID))AddIssue(Issues,D.DefinitionID,TEXT("MissingNPCReference"),FString::Printf(TEXT("Missing NPC '%s'."),*Node.SpeakerNPCID));for(const auto&ID:Node.OutcomeIDs)if(!ID.IsEmpty()) { /* outcome target validation below */ } for(const auto&Choice:Node.Choices)for(const auto&ID:Choice.OutcomeIDs)if(!ID.IsEmpty()){} }}
 for(const auto&P:WorldEvents){const auto&D=P.Value;if(D.TriggerType==EAetherWorldEventTriggerType::PreviousEvent&&!WorldEvents.Contains(N(D.PreviousEventID)))AddIssue(Issues,D.DefinitionID,TEXT("MissingPreviousEvent"),FString::Printf(TEXT("Missing previous event '%s'."),*D.PreviousEventID));for(const auto&A:D.ParticipatingWorldActorIDs)Actor(D.DefinitionID,A);for(const auto&R:D.RewardIDs)Reward(D.DefinitionID,R);}
 for(const auto&P:WorldContents){const auto&D=P.Value;for(const auto&A:D.WorldActorIDs)Actor(D.DefinitionID,A);for(const auto&Q:D.QuestIDs)if(!Quests.Contains(N(Q)))AddIssue(Issues,D.DefinitionID,TEXT("MissingQuestReference"),FString::Printf(TEXT("Missing quest '%s'."),*Q));for(const auto&DID:D.DialogueIDs)if(!Dialogues.Contains(N(DID)))AddIssue(Issues,D.DefinitionID,TEXT("MissingDialogueReference"),FString::Printf(TEXT("Missing dialogue '%s'."),*DID));for(const auto&EID:D.EventIDs)if(!WorldEvents.Contains(N(EID)))AddIssue(Issues,D.DefinitionID,TEXT("MissingWorldEventReference"),FString::Printf(TEXT("Missing world event '%s'."),*EID));}
 for(const auto&P:Quests)Content(P.Key,P.Key);for(const auto&P:Dialogues)Content(P.Key,P.Key);for(const auto&P:WorldEvents)Content(P.Key,P.Key);for(const auto&P:WorldContents)Content(P.Key,P.Key);
 return Issues.Num()==0;
}
