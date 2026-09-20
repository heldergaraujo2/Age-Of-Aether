#pragma once
#include "CoreMinimal.h"
#include "World/AetherWorldMapTypes.h"
class FAetherWorldMapRegistry
{
public:
 bool RegisterMap(const FAetherWorldMapDefinition& Map);
 bool RegisterLink(const FAetherWorldStreamingLink& Link);
 const FAetherWorldMapDefinition* FindMap(const FString& ID) const;
 const FAetherWorldStreamingLink* FindLink(const FString& ID) const;
 bool Validate(TArray<FString>& Errors) const;
 void Reset();
 int32 NumMaps() const{return Maps.Num();}
 int32 NumLinks() const{return Links.Num();}
private:
 static FString Normalize(const FString& V){return V.TrimStartAndEnd().ToLower();}
 TMap<FString,FAetherWorldMapDefinition> Maps;
 TMap<FString,FAetherWorldStreamingLink> Links;
};