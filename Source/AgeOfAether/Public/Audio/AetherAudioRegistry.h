#pragma once
#include "CoreMinimal.h"
#include "Audio/AetherAudioTypes.h"
class FAetherAudioRegistry
{
public:
 bool Register(const FAetherAudioDefinition& Definition);
 const FAetherAudioDefinition* Find(const FString& AudioID) const;
 bool Validate(TArray<FString>& OutErrors) const;
 void Reset();
 int32 Num() const { return Entries.Num(); }
private:
 static FString Normalize(const FString& Value);
 TMap<FString, FAetherAudioDefinition> Entries;
};