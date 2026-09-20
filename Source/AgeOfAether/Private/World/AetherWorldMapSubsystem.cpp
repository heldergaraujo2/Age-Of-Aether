#include "World/AetherWorldMapSubsystem.h"
bool UAetherWorldMapSubsystem::FindMap(const FString&ID,FAetherWorldMapDefinition&Out)const{auto*M=Registry.FindMap(ID);if(!M)return false;Out=*M;return true;}
bool UAetherWorldMapSubsystem::CanStreamLink(const FString&ID)const{auto*L=Registry.FindLink(ID);if(!L||!L->bEnabled)return false;return IsMapActive(L->SourceMapID)&&L->TargetMapID!=L->SourceMapID;}
bool UAetherWorldMapSubsystem::SetMapActive(const FString&ID,bool bActive){auto*M=Registry.FindMap(ID);if(!M||!M->bEnabled)return false;FString K=M->MapID.TrimStartAndEnd().ToLower();if(bActive)ActiveMaps.Add(K);else ActiveMaps.Remove(K);return true;}
bool UAetherWorldMapSubsystem::IsMapActive(const FString&ID)const{return ActiveMaps.Contains(ID.TrimStartAndEnd().ToLower());}