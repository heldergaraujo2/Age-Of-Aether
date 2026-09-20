#pragma once
#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "World/AetherWorldMapTypes.h"
#include "AetherWorldMapCatalog.generated.h"
UCLASS(BlueprintType)
class AGEOFAETHER_API UAetherWorldMapCatalog:public UDataAsset
{
 GENERATED_BODY()
public:
 UPROPERTY(EditAnywhere,BlueprintReadOnly) TArray<FAetherWorldMapDefinition> Maps;
 UPROPERTY(EditAnywhere,BlueprintReadOnly) TArray<FAetherWorldStreamingLink> Links;
 bool IsValid(TArray<FString>& Errors)const;
 void RegisterInto(class UAetherWorldMapSubsystem* Subsystem)const;
};