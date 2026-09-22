#pragma once
#include "CoreMinimal.h"
#include "Interaction/AetherInteractionTypes.h"
class FAetherInteractionRegistry{
public:
 bool RegisterInteraction(const FAetherInteractionDefinition&D);
 bool RegisterQuest(const FAetherInteractionQuestDefinition&D);
 bool RegisterEvent(const FAetherEventDefinition&D);
 const FAetherInteractionDefinition* FindInteraction(const FString&ID)const;
 const FAetherInteractionQuestDefinition* FindQuest(const FString&ID)const;
 const FAetherEventDefinition* FindEvent(const FString&ID)const;
 bool Validate(TArray<FString>&Errors)const;
 void Reset();
private: static FString N(const FString&V){return V.TrimStartAndEnd().ToLower();}
 TMap<FString,FAetherInteractionDefinition> I; TMap<FString,FAetherInteractionQuestDefinition> Q; TMap<FString,FAetherEventDefinition>E;
};