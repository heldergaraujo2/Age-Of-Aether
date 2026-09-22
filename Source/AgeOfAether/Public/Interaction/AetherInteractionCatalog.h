#pragma once
#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Interaction/AetherInteractionTypes.h"
#include "AetherInteractionCatalog.generated.h"
UCLASS(BlueprintType) class AGEOFAETHER_API UAetherInteractionCatalog:public UDataAsset{
 GENERATED_BODY()
public:
 UPROPERTY(EditAnywhere,BlueprintReadOnly) TArray<FAetherInteractionDefinition> Interactions;
 UPROPERTY(EditAnywhere,BlueprintReadOnly) TArray<FAetherInteractionQuestDefinition> Quests;
 UPROPERTY(EditAnywhere,BlueprintReadOnly) TArray<FAetherEventDefinition> Events;
 bool IsValid(TArray<FString>&Errors)const;
 void RegisterInto(class UAetherInteractionSubsystem*Subsystem)const;
};