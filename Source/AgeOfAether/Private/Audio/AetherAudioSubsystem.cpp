#include "Audio/AetherAudioSubsystem.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
bool UAetherAudioSubsystem::RegisterAudio(const FAetherAudioDefinition& D){return Registry.Register(D);}
const FAetherAudioDefinition* UAetherAudioSubsystem::Find(const FString& ID) const{return Registry.Find(ID);}
bool UAetherAudioSubsystem::PlayOneShot(const FString& ID)
{
 const FAetherAudioDefinition* D=Find(ID); if(!D||D->Playback!=EAetherAudioPlayback::OneShot||!GetWorld()||GetWorld()->GetNetMode()==NM_DedicatedServer)return false;
 USoundBase* S=D->Sound.LoadSynchronous(); if(!S)return false;
 UGameplayStatics::PlaySound2D(GetWorld(),S,D->Volume,D->Pitch); return true;
}
bool UAetherAudioSubsystem::PlayLoop(const FString& ID)
{
 const FAetherAudioDefinition* D=Find(ID); if(!D||D->Playback!=EAetherAudioPlayback::Loop||!GetWorld()||GetWorld()->GetNetMode()==NM_DedicatedServer)return false;
 if(IsPlaying(ID))return true; USoundBase* S=D->Sound.LoadSynchronous(); if(!S)return false;
 UAudioComponent* C=UGameplayStatics::SpawnSound2D(GetWorld(),S,D->Volume,D->Pitch); if(!C)return false;
 C->bAutoDestroy=false; ActiveLoops.Add(D->AudioID,C); return true;
}
void UAetherAudioSubsystem::StopLoop(const FString& ID){if(TObjectPtr<UAudioComponent>* C=ActiveLoops.Find(ID)){if(IsValid(*C)){(*C)->Stop();(*C)->DestroyComponent();}ActiveLoops.Remove(ID);}}
bool UAetherAudioSubsystem::IsPlaying(const FString& ID) const {const TObjectPtr<UAudioComponent>* C=ActiveLoops.Find(ID);return C&&IsValid(*C)&&(*C)->IsPlaying();}
bool UAetherAudioSubsystem::Validate(TArray<FString>& Errors) const{return Registry.Validate(Errors);}