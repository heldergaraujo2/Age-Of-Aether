#include "World/AetherWorldMapRegistry.h"
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAetherWorldMapRegistryTest,"AgeOfAether.World.MapRegistry",EAutomationTestFlags::ApplicationContextMask|EAutomationTestFlags::ProductFilter)
bool FAetherWorldMapRegistryTest::RunTest(const FString&)
{
 FAetherWorldMapRegistry R; FAetherWorldMapDefinition A;A.MapID=TEXT("Starter");A.DisplayName=TEXT("Starter");A.MapAssetPath=TEXT("/Game/Aether/Maps/Starter");A.ZoneID.Value=TEXT("starter");
 TestTrue(TEXT("Map registers"),R.RegisterMap(A));TestFalse(TEXT("Duplicate rejected"),R.RegisterMap(A));TestNotNull(TEXT("Normalized lookup"),R.FindMap(TEXT("STARTER")));
 FAetherWorldMapDefinition B=A;B.MapID=TEXT("Forest");B.ZoneID.Value=TEXT("forest");TestTrue(TEXT("Second map registers"),R.RegisterMap(B));
 FAetherWorldStreamingLink L;L.LinkID=TEXT("starter_forest");L.SourceMapID=TEXT("STARTER");L.TargetMapID=TEXT("forest");TestTrue(TEXT("Link registers"),R.RegisterLink(L));
 TArray<FString>E;TestTrue(TEXT("Registry validates"),R.Validate(E));TestEqual(TEXT("No errors"),E.Num(),0);return true;
}
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAetherWorldMapBoundsTest,"AgeOfAether.World.MapValidation",EAutomationTestFlags::ApplicationContextMask|EAutomationTestFlags::ProductFilter)
bool FAetherWorldMapBoundsTest::RunTest(const FString&){FAetherWorldMapDefinition M;M.MapID=TEXT("x");M.DisplayName=TEXT("X");M.MapAssetPath=TEXT("/Game/X");M.ZoneID.Value=TEXT("z");TestTrue(TEXT("Valid map"),M.IsValid());M.MinimumLevel=0;TestFalse(TEXT("Invalid level rejected"),M.IsValid());return true;}