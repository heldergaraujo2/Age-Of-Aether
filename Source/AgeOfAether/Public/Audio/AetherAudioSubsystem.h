#pragma once
#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Audio/AetherAudioRegistry.h"
#include "AetherAudioSubsystem.generated.h"
class UAudioComponent;
UCLASS()
class AGEOFAETHER_API UAetherAudioSubsystem : public UGameInstanceSubsystem
{
 GENERATED_BODY()
public:
 bool RegisterAudio(const FAetherAudioDefinition& Definition);
 UFUNCTION(BlueprintCallable,Category="Age of Aether|Audio") bool PlayOneShot(const FString& AudioID);
 UFUNCTION(BlueprintCallable,Category="Age of Aether|Audio") bool PlayLoop(const FString& AudioID);
 UFUNCTION(BlueprintCallable,Category="Age of Aether|Audio") void StopLoop(const FString& AudioID);
 UFUNCTION(BlueprintPure,Category="Age of Aether|Audio") bool IsPlaying(const FString& AudioID) const;
 const FAetherAudioDefinition* Find(const FString& AudioID) const;
 bool Validate(TArray<FString>& OutErrors) const;
private:
 FAetherAudioRegistry Registry;
 UPROPERTY(Transient) TMap<FString,TObjectPtr<UAudioComponent>> ActiveLoops;
};