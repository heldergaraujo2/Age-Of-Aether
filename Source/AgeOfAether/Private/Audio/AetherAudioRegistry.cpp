#include "Audio/AetherAudioRegistry.h"
FString FAetherAudioRegistry::Normalize(const FString& Value){ return Value.TrimStartAndEnd().ToLower(); }
bool FAetherAudioRegistry::Register(const FAetherAudioDefinition& D){ if(!D.IsValid()) return false; const FString K=Normalize(D.AudioID); if(K.IsEmpty()||Entries.Contains(K)) return false; FAetherAudioDefinition C=D; C.AudioID=K; Entries.Add(K,MoveTemp(C)); return true; }
const FAetherAudioDefinition* FAetherAudioRegistry::Find(const FString& ID) const { return Entries.Find(Normalize(ID)); }
bool FAetherAudioRegistry::Validate(TArray<FString>& Errors) const { Errors.Reset(); for(const auto& P:Entries) if(!P.Value.IsValid()) Errors.Add(P.Key); return Errors.IsEmpty(); }
void FAetherAudioRegistry::Reset(){ Entries.Reset(); }