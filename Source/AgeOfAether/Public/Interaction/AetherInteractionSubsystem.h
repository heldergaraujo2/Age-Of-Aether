#pragma once
#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "Interaction/AetherInteractionRegistry.h"
#include "AetherInteractionSubsystem.generated.h"
UCLASS() class AGEOFAETHER_API UAetherInteractionSubsystem:public UWorldSubsystem{
 GENERATED_BODY()
public:
 bool RegisterInteraction(const FAetherInteractionDefinition&D){return R.RegisterInteraction(D);}
 bool RegisterQuest(const FAetherQuestDefinition&D){return R.RegisterQuest(D);}
 bool RegisterEvent(const FAetherEventDefinition&D){return R.RegisterEvent(D);}
 UFUNCTION(BlueprintCallable,Category="Age of Aether|Interaction") bool StartQuest(const FString&QuestID,int32 Level);
 UFUNCTION(BlueprintCallable,Category="Age of Aether|Interaction") bool AdvanceQuest(const FString&QuestID,int32 Amount=1);
 UFUNCTION(BlueprintCallable,Category="Age of Aether|Interaction") bool CompleteQuest(const FString&QuestID);
 UFUNCTION(BlueprintPure,Category="Age of Aether|Interaction") EAetherQuestState GetQuestState(const FString&QuestID)const;
 UFUNCTION(BlueprintPure,Category="Age of Aether|Interaction") int32 GetQuestProgress(const FString&QuestID)const;
 UFUNCTION(BlueprintCallable,Category="Age of Aether|Interaction") bool CanInteract(const FString&InteractionID,int32 Level)const;
 bool Validate(TArray<FString>&Errors)const{return R.Validate(Errors);}
 void Reset(){R.Reset();States.Reset();}
private: FAetherInteractionRegistry R; TMap<FString,FAetherQuestRuntime>States;
};