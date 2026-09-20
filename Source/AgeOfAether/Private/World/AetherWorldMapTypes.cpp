#include "World/AetherWorldMapTypes.h"
bool FAetherWorldMapDefinition::IsValid() const
{
 return !MapID.TrimStartAndEnd().IsEmpty() && !DisplayName.TrimStartAndEnd().IsEmpty() &&
 !MapAssetPath.TrimStartAndEnd().IsEmpty() && ZoneID.IsValid() && MinimumLevel>=1;
}
bool FAetherWorldStreamingLink::IsValid() const
{
 return !LinkID.TrimStartAndEnd().IsEmpty() && !SourceMapID.TrimStartAndEnd().IsEmpty() &&
 !TargetMapID.TrimStartAndEnd().IsEmpty() && !SourceMapID.Equals(TargetMapID,ESearchCase::IgnoreCase);
}