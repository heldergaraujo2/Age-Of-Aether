#pragma once
#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Audio/AetherAudioTypes.h"
#include "AetherAudioCatalog.generated.h"
UCLASS(BlueprintType)
class AGEOFAETHER_API UAetherAudioCatalog : public UDataAsset
{
 GENERATED_BODY()
public:
 UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Audio") TArray<FAetherAudioDefinition> Entries;
 bool IsValid(TArray<FString>& OutErrors) const;
 void RegisterInto(class UAetherAudioSubsystem* Subsystem) const;
};