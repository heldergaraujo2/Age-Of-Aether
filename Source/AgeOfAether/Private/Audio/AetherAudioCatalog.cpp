#include "Audio/AetherAudioCatalog.h"
#include "Audio/AetherAudioSubsystem.h"
bool UAetherAudioCatalog::IsValid(TArray<FString>& Errors) const
{
 Errors.Reset(); TSet<FString> IDs;
 for(const auto& E:Entries){ if(!E.IsValid()){Errors.Add(E.AudioID);continue;} const FString K=E.AudioID.TrimStartAndEnd().ToLower(); if(IDs.Contains(K)) Errors.Add(K); IDs.Add(K); }
 return Errors.IsEmpty();
}
void UAetherAudioCatalog::RegisterInto(UAetherAudioSubsystem* S) const { if(!S)return; for(const auto& E:Entries) S->RegisterAudio(E); }