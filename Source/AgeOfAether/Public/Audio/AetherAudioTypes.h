#pragma once
#include "CoreMinimal.h"
#include "Sound/SoundBase.h"
#include "AetherAudioTypes.generated.h"

UENUM(BlueprintType)
enum class EAetherAudioBus : uint8 { Music, Ambience, UI, Combat, Creature, Voice };

UENUM(BlueprintType)
enum class EAetherAudioPlayback : uint8 { OneShot, Loop };

USTRUCT(BlueprintType)
struct FAetherAudioDefinition
{
    GENERATED_BODY()
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly) FString AudioID;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly) EAetherAudioBus Bus = EAetherAudioBus::Ambience;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly) EAetherAudioPlayback Playback = EAetherAudioPlayback::OneShot;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly) TSoftObjectPtr<USoundBase> Sound;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly) float Volume = 1.0f;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly) float Pitch = 1.0f;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly) bool bSpatialized = false;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly) float MaxDistance = 3000.0f;
    bool IsValid() const;
};

USTRUCT(BlueprintType)
struct FAetherAudioRuntimeHandle
{
    GENERATED_BODY()
    UPROPERTY(BlueprintReadOnly) FString AudioID;
    UPROPERTY(BlueprintReadOnly) bool bPlaying = false;
};
