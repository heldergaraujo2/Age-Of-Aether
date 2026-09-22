#pragma once
#include "CoreMinimal.h"
#include "AetherInteractionTypes.generated.h"

UENUM(BlueprintType) enum class EAetherInteractionType:uint8 { Talk, Quest, Shop, Service, Event };
UENUM(BlueprintType) enum class EAetherQuestState:uint8 { Locked, Available, Active, Completed, Failed };

USTRUCT(BlueprintType) struct FAetherInteractionDefinition{
 GENERATED_BODY()
 UPROPERTY(EditAnywhere,BlueprintReadOnly) FString InteractionID;
 UPROPERTY(EditAnywhere,BlueprintReadOnly) EAetherInteractionType Type=EAetherInteractionType::Talk;
 UPROPERTY(EditAnywhere,BlueprintReadOnly) FString DisplayName;
 UPROPERTY(EditAnywhere,BlueprintReadOnly) FString TargetID;
 UPROPERTY(EditAnywhere,BlueprintReadOnly) FString RequiredQuestID;
 UPROPERTY(EditAnywhere,BlueprintReadOnly) int32 MinimumLevel=1;
 UPROPERTY(EditAnywhere,BlueprintReadOnly) bool bEnabled=true;
 bool IsValid()const{return !InteractionID.TrimStartAndEnd().IsEmpty()&&!TargetID.TrimStartAndEnd().IsEmpty()&&!DisplayName.TrimStartAndEnd().IsEmpty()&&MinimumLevel>=1;}
};

USTRUCT(BlueprintType) struct FAetherInteractionQuestDefinition{
 GENERATED_BODY()
 UPROPERTY(EditAnywhere,BlueprintReadOnly) FString QuestID;
 UPROPERTY(EditAnywhere,BlueprintReadOnly) FString DisplayName;
 UPROPERTY(EditAnywhere,BlueprintReadOnly) FString StartNPCID;
 UPROPERTY(EditAnywhere,BlueprintReadOnly) FString CompleteNPCID;
 UPROPERTY(EditAnywhere,BlueprintReadOnly) FString ObjectiveID;
 UPROPERTY(EditAnywhere,BlueprintReadOnly) int32 RequiredCount=1;
 UPROPERTY(EditAnywhere,BlueprintReadOnly) int32 MinimumLevel=1;
 UPROPERTY(EditAnywhere,BlueprintReadOnly) bool bRepeatable=false;
 bool IsValid()const{return !QuestID.TrimStartAndEnd().IsEmpty()&&!DisplayName.TrimStartAndEnd().IsEmpty()&&!StartNPCID.TrimStartAndEnd().IsEmpty()&&RequiredCount>0&&RequiredCount<=100000&&MinimumLevel>=1;}
};

USTRUCT(BlueprintType) struct FAetherQuestRuntime{
 GENERATED_BODY()
 UPROPERTY(BlueprintReadOnly) FString QuestID;
 UPROPERTY(BlueprintReadOnly) EAetherQuestState State=EAetherQuestState::Locked;
 UPROPERTY(BlueprintReadOnly) int32 Progress=0;
 UPROPERTY(BlueprintReadOnly) int32 RequiredCount=1;
 bool IsComplete()const{return State==EAetherQuestState::Completed;}
};

USTRUCT(BlueprintType) struct FAetherEventDefinition{
 GENERATED_BODY()
 UPROPERTY(EditAnywhere,BlueprintReadOnly) FString EventID;
 UPROPERTY(EditAnywhere,BlueprintReadOnly) FString DisplayName;
 UPROPERTY(EditAnywhere,BlueprintReadOnly) FString EventType;
 UPROPERTY(EditAnywhere,BlueprintReadOnly) FString SpawnID;
 UPROPERTY(EditAnywhere,BlueprintReadOnly) int32 MinimumLevel=1;
 UPROPERTY(EditAnywhere,BlueprintReadOnly) float DurationSeconds=300.f;
 UPROPERTY(EditAnywhere,BlueprintReadOnly) bool bEnabled=true;
 bool IsValid()const{return !EventID.TrimStartAndEnd().IsEmpty()&&!DisplayName.TrimStartAndEnd().IsEmpty()&&!EventType.TrimStartAndEnd().IsEmpty()&&MinimumLevel>=1&&FMath::IsFinite(DurationSeconds)&&DurationSeconds>0.f&&DurationSeconds<=86400.f;}
};