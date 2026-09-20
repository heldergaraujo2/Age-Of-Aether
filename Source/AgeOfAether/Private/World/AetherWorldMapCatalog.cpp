#include "World/AetherWorldMapCatalog.h"
#include "World/AetherWorldMapSubsystem.h"
bool UAetherWorldMapCatalog::IsValid(TArray<FString>& E)const
{
 E.Reset();TSet<FString> IDs;
 for(const auto&M:Maps){if(!M.IsValid()){E.Add(TEXT("Invalid map: ")+M.MapID);continue;}auto K=M.MapID.TrimStartAndEnd().ToLower();if(IDs.Contains(K))E.Add(TEXT("Duplicate map: ")+K);IDs.Add(K);}
 TSet<FString>LIDs;
 for(const auto&L:Links){if(!L.IsValid()){E.Add(TEXT("Invalid link: ")+L.LinkID);continue;}auto K=L.LinkID.TrimStartAndEnd().ToLower();if(LIDs.Contains(K))E.Add(TEXT("Duplicate link: ")+K);LIDs.Add(K);if(!IDs.Contains(L.SourceMapID.TrimStartAndEnd().ToLower())||!IDs.Contains(L.TargetMapID.TrimStartAndEnd().ToLower()))E.Add(TEXT("Missing map in link: ")+K);}
 return E.IsEmpty();
}
void UAetherWorldMapCatalog::RegisterInto(UAetherWorldMapSubsystem*S)const{if(!S)return;for(const auto&M:Maps)S->RegisterMap(M);for(const auto&L:Links)S->RegisterLink(L);}