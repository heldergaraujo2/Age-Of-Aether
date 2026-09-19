#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Quests/AetherQuestTypes.h"
#include "AetherQuestDefinitionDataAsset.generated.h"

UCLASS(BlueprintType)
class AGEOFAETHER_API UAetherQuestDefinitionDataAsset : public UDataAsset
{
    GENERATED_BODY()

public:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Quest")
    FAetherQuestDefinition Definition;
};
