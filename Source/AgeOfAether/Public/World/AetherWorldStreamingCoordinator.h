#pragma once
#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "World/AetherWorldMapSubsystem.h"
#include "AetherWorldStreamingCoordinator.generated.h"

UCLASS()
class AGEOFAETHER_API UAetherWorldStreamingCoordinator:public UWorldSubsystem
{
 GENERATED_BODY()
public:
 UFUNCTION(BlueprintCallable,Category="Age of Aether|World") bool PrepareStream(const FString& LinkID);
 UFUNCTION(BlueprintCallable,Category="Age of Aether|World") bool ActivateTarget(const FString& MapID);
 UFUNCTION(BlueprintCallable,Category="Age of Aether|World") bool DeactivateMap(const FString& MapID);
};
