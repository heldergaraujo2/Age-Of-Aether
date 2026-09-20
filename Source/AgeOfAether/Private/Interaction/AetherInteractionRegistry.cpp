#include "Interaction/AetherInteractionRegistry.h"
bool FAetherInteractionRegistry::RegisterInteraction(const FAetherInteractionDefinition&D){if(!D.IsValid())return false;auto K=N(D.InteractionID);if(I.Contains(K))return false;auto C=D;C.InteractionID=K;I.Add(K,C);return true;}
bool FAetherInteractionRegistry::RegisterQuest(const FAetherQuestDefinition&D){if(!D.IsValid())return false;auto K=N(D.QuestID);if(Q.Contains(K))return false;auto C=D;C.QuestID=K;Q.Add(K,C);return true;}
bool FAetherInteractionRegistry::RegisterEvent(const FAetherEventDefinition&D){if(!D.IsValid())return false;auto K=N(D.EventID);if(E.Contains(K))return false;auto C=D;C.EventID=K;E.Add(K,C);return true;}
const FAetherInteractionDefinition* FAetherInteractionRegistry::FindInteraction(const FString&ID)const{return I.Find(N(ID));}
const FAetherQuestDefinition* FAetherInteractionRegistry::FindQuest(const FString&ID)const{return Q.Find(N(ID));}
const FAetherEventDefinition* FAetherInteractionRegistry::FindEvent(const FString&ID)const{return E.Find(N(ID));}
bool FAetherInteractionRegistry::Validate(TArray<FString>&Errors)const{Errors.Reset();for(auto&P:I)if(!P.Value.IsValid())Errors.Add(TEXT("Invalid interaction: ")+P.Key);for(auto&P:Q)if(!P.Value.IsValid())Errors.Add(TEXT("Invalid quest: ")+P.Key);for(auto&P:E)if(!P.Value.IsValid())Errors.Add(TEXT("Invalid event: ")+P.Key);return Errors.IsEmpty();}
void FAetherInteractionRegistry::Reset(){I.Reset();Q.Reset();E.Reset();}