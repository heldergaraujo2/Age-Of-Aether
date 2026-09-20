#pragma once
#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "World/AetherWorldMapRegistry.h"
#include "AetherWorldMapSubsystem.generated.h"
UCLASS()
class AGEOFAETHER_API UAetherWorldMapSubsystem:public UWorldSubsystem
{
 GENERATED_BODY()
public:
 bool RegisterMap(const FAetherWorldMapDefinition&Map){return Registry.RegisterMap(Map);}
 bool RegisterLink(const FAetherWorldStreamingLink&Link){return Registry.RegisterLink(Link);}
 UFUNCTION(BlueprintPure,Category="Age of Aether|World") bool FindMap(const FString&MapID,FAetherWorldMapDefinition&Out)const;
 UFUNCTION(BlueprintCallable,Category="Age of Aether|World") bool CanStreamLink(const FString&LinkID)const;
 UFUNCTION(BlueprintCallable,Category="Age of Aether|World") bool SetMapActive(const FString&MapID,bool bActive);
 UFUNCTION(BlueprintPure,Category="Age of Aether|World") bool IsMapActive(const FString&MapID)const;
 bool Validate(TArray<FString>&Errors)const{return Registry.Validate(Errors);}
 void Reset(){Registry.Reset();ActiveMaps.Reset();}
private:
 FAetherWorldMapRegistry Registry;
 TSet<FString> ActiveMaps;
};