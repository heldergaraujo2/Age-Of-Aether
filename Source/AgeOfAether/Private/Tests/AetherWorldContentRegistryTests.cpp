#include "Misc/AutomationTest.h"
#include "Data/AetherWorldContentRegistry.h"
#include "Data/AetherWorldActorRegistry.h"
#include "Data/AetherLootRewardRegistry.h"
#include "Data/AetherQuestDialogueEventRegistry.h"
#include "Data/AetherRecipeRegistry.h"
#include "Data/AetherItemRegistry.h"
#include "Data/AetherSkillEffectRegistry.h"

namespace
{
FAetherMapDefinition MakeMap(const FString& ID)
{
    FAetherMapDefinition M; M.DefinitionID=ID; M.DisplayName=ID; M.MinBounds=FVector2D(0,0); M.MaxBounds=FVector2D(2000,2000); M.StreamingCellSize=1000;
    FAetherStreamingCellDefinition C; C.DefinitionID=ID+TEXT(".Cell.0"); C.Coordinates=FIntPoint(0,0); C.MinBounds=FVector2D(0,0); C.MaxBounds=FVector2D(1000,1000); M.StreamingCells.Add(C);
    FAetherWorldPointDefinition P; P.DefinitionID=ID+TEXT(".Spawn"); P.Type=EAetherWorldPointType::Spawn; P.Location=FVector(100,100,0); P.StreamingCellID=C.DefinitionID; M.Points.Add(P); M.DefaultSpawnPointID=P.DefinitionID;
    return M;
}
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAetherWorldContentBasicTest,"AgeOfAether.Data.WorldContent.Basic",EAutomationTestFlags::EditorContext|EAutomationTestFlags::EngineFilter)
bool FAetherWorldContentBasicTest::RunTest(const FString&)
{
    FAetherWorldContentRegistry R; FString E; auto M=MakeMap(TEXT("Map.A")); TestTrue(TEXT("register"),R.RegisterMap(M,E)); TestEqual(TEXT("count"),R.NumMaps(),1);
    FAetherMapDefinition Out; TestTrue(TEXT("normalized resolve"),R.ResolveMap(TEXT(" Map.A "),Out)); TestEqual(TEXT("id"),Out.DefinitionID,TEXT("Map.A")); return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAetherWorldContentInvalidMapTest,"AgeOfAether.Data.WorldContent.InvalidMap",EAutomationTestFlags::EditorContext|EAutomationTestFlags::EngineFilter)
bool FAetherWorldContentInvalidMapTest::RunTest(const FString&)
{
    FAetherWorldContentRegistry R; FString E; auto A=MakeMap(TEXT("Map.BadBounds")); A.MinBounds=FVector2D(10,10); A.MaxBounds=FVector2D(1,1); TestFalse(TEXT("invalid bounds"),R.RegisterMap(A,E));
    auto B=MakeMap(TEXT("Map.BadLevels")); B.RecommendedMinimumLevel=20; B.RecommendedMaximumLevel=10; TestFalse(TEXT("invalid levels"),R.RegisterMap(B,E));
    auto C=MakeMap(TEXT("Map.BadStreaming")); C.StreamingCellSize=0; TestFalse(TEXT("invalid streaming size"),R.RegisterMap(C,E)); return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAetherWorldContentDuplicateTest,"AgeOfAether.Data.WorldContent.Duplicates",EAutomationTestFlags::EditorContext|EAutomationTestFlags::EngineFilter)
bool FAetherWorldContentDuplicateTest::RunTest(const FString&)
{
    FAetherWorldContentRegistry R; FString E; auto A=MakeMap(TEXT("Map.Dup")); TestTrue(TEXT("first map"),R.RegisterMap(A,E)); TestFalse(TEXT("duplicate map"),R.RegisterMap(A,E));
    auto B=MakeMap(TEXT("Map.InternalDup")); auto DuplicateCell=B.StreamingCells[0]; B.StreamingCells.Add(DuplicateCell); TestFalse(TEXT("duplicate cell id"),R.RegisterMap(B,E));
    auto C=MakeMap(TEXT("Map.PointDup")); auto DuplicatePoint=C.Points[0]; C.Points.Add(DuplicatePoint); TestFalse(TEXT("duplicate point id"),R.RegisterMap(C,E)); return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAetherWorldContentStreamingTest,"AgeOfAether.Data.WorldContent.Streaming",EAutomationTestFlags::EditorContext|EAutomationTestFlags::EngineFilter)
bool FAetherWorldContentStreamingTest::RunTest(const FString&)
{
    FAetherWorldContentRegistry R; FString E; auto A=MakeMap(TEXT("Map.Stream"));
    A.StreamingCells[0].MaxBounds=FVector2D(1501,1000); TestTrue(TEXT("registration is structural"),R.RegisterMap(A,E));
    TArray<FAetherWorldContentValidationIssue> Issues; TestFalse(TEXT("oversized cell rejected by validation"),R.Validate(Issues)); TestTrue(TEXT("oversized issue"),Issues.Num()>0);
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAetherWorldContentBoundsTest,"AgeOfAether.Data.WorldContent.Bounds",EAutomationTestFlags::EditorContext|EAutomationTestFlags::EngineFilter)
bool FAetherWorldContentBoundsTest::RunTest(const FString&)
{
    FAetherWorldContentRegistry R; FString E; auto A=MakeMap(TEXT("Map.Bounds"));
    A.Points[0].Location=FVector(2500,100,0); TestTrue(TEXT("register"),R.RegisterMap(A,E));
    TArray<FAetherWorldContentValidationIssue> Issues; TestFalse(TEXT("point outside map"),R.Validate(Issues)); TestTrue(TEXT("bounds issue"),Issues.Num()>0);
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAetherWorldContentGraphTest,"AgeOfAether.Data.WorldContent.Graph",EAutomationTestFlags::EditorContext|EAutomationTestFlags::EngineFilter)
bool FAetherWorldContentGraphTest::RunTest(const FString&)
{
    FAetherWorldContentRegistry R; FString E; auto A=MakeMap(TEXT("Map.A")); auto B=MakeMap(TEXT("Map.B"));
    FAetherMapConnectionDefinition AB; AB.DefinitionID=TEXT("Conn.AB"); AB.SourcePointID=A.DefaultSpawnPointID; AB.TargetMapID=B.DefinitionID; AB.TargetPointID=B.DefaultSpawnPointID; AB.bBidirectional=true; A.Connections.Add(AB);
    FAetherMapConnectionDefinition BA; BA.DefinitionID=TEXT("Conn.BA"); BA.SourcePointID=B.DefaultSpawnPointID; BA.TargetMapID=A.DefinitionID; BA.TargetPointID=A.DefaultSpawnPointID; B.Connections.Add(BA);
    TestTrue(TEXT("register A"),R.RegisterMap(A,E)); TestTrue(TEXT("register B"),R.RegisterMap(B,E));
    TArray<FAetherWorldContentValidationIssue> Issues; TestTrue(TEXT("cycles are valid"),R.Validate(Issues)); return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAetherWorldContentInteractionTest,"AgeOfAether.Data.WorldContent.Interaction",EAutomationTestFlags::EditorContext|EAutomationTestFlags::EngineFilter)
bool FAetherWorldContentInteractionTest::RunTest(const FString&)
{
    FAetherMapDefinition A=MakeMap(TEXT("Map.Interaction")); FString Error;
    FAetherWorldInteractionDefinition I; I.DefinitionID=TEXT("Interaction.Bad"); I.PointID=A.DefaultSpawnPointID; I.WorldActorID=TEXT("Actor.AlsoSet"); TestFalse(TEXT("exactly one target"),I.IsStructurallyValid(Error));
    I.WorldActorID.Reset(); I.InteractionRadius=0; TestFalse(TEXT("radius positive"),I.IsStructurallyValid(Error));
    I.InteractionRadius=100; FAetherWorldRequirement Q; Q.Type=EAetherWorldRequirementType::WorldTag; Q.RequiredTag=TEXT("Season.1"); I.Requirements.Add(Q);
    FAetherWorldOutcome O; O.Type=EAetherWorldOutcomeType::Teleport; O.TargetMapID=TEXT("Map.Target"); O.TargetPointID=TEXT("Point.Target"); I.Outcomes.Add(O); A.Interactions.Add(I);
    FAetherWorldContentRegistry R; TestTrue(TEXT("register interaction map"),R.RegisterMap(A,Error)); return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAetherWorldContentReferenceTest,"AgeOfAether.Data.WorldContent.References",EAutomationTestFlags::EditorContext|EAutomationTestFlags::EngineFilter)
bool FAetherWorldContentReferenceTest::RunTest(const FString&)
{
    FAetherWorldContentRegistry R; FString E; auto A=MakeMap(TEXT("Map.Refs"));
    FAetherWorldActorPlacementDefinition P; P.DefinitionID=TEXT("Placement.Bad"); P.WorldActorID=TEXT("Actor.Missing"); P.StreamingCellID=TEXT("Cell.Missing"); P.SpawnGroupID=TEXT("Spawn.Missing"); A.ActorPlacements.Add(P);
    FAetherWorldInteractionDefinition I; I.DefinitionID=TEXT("Interaction.BadRefs"); I.PointID=TEXT("Point.Missing"); FAetherWorldRequirement Item; Item.Type=EAetherWorldRequirementType::ItemOwned; Item.ReferenceID=TEXT("Item.Missing"); I.Requirements.Add(Item); FAetherWorldOutcome Recipe; Recipe.Type=EAetherWorldOutcomeType::StartRecipe; Recipe.ReferenceID=TEXT("Recipe.Missing"); I.Outcomes.Add(Recipe); A.Interactions.Add(I);
    TestTrue(TEXT("structural registration"),R.RegisterMap(A,E));
    FAetherWorldActorRegistry Actors; FAetherLootRewardRegistry Loot; FAetherQuestDialogueEventRegistry Quests; FAetherRecipeRegistry Recipes; FAetherItemRegistry Items; FAetherSkillEffectRegistry Skills;
    TArray<FAetherWorldContentValidationIssue> Issues; TestFalse(TEXT("missing references detected"),R.Validate(Issues,nullptr,nullptr,&Items,&Actors,&Loot,&Quests,&Recipes,&Skills)); TestTrue(TEXT("multiple reference issues"),Issues.Num()>=5); return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAetherWorldContentTeleportTest,"AgeOfAether.Data.WorldContent.Teleport",EAutomationTestFlags::EditorContext|EAutomationTestFlags::EngineFilter)
bool FAetherWorldContentTeleportTest::RunTest(const FString&)
{
    FAetherWorldContentRegistry R; FString E; auto A=MakeMap(TEXT("Map.A")); auto B=MakeMap(TEXT("Map.B"));
    FAetherWorldInteractionDefinition I; I.DefinitionID=TEXT("Portal.A"); I.PointID=A.DefaultSpawnPointID; FAetherWorldOutcome O; O.Type=EAetherWorldOutcomeType::Teleport; O.TargetMapID=B.DefinitionID; O.TargetPointID=B.DefaultSpawnPointID; I.Outcomes.Add(O); A.Interactions.Add(I);
    TestTrue(TEXT("register A"),R.RegisterMap(A,E)); TestTrue(TEXT("register B"),R.RegisterMap(B,E)); TArray<FAetherWorldContentValidationIssue> Issues; TestTrue(TEXT("valid teleport"),R.Validate(Issues));
    auto C=MakeMap(TEXT("Map.C")); FAetherWorldInteractionDefinition Bad=I; Bad.DefinitionID=TEXT("Portal.Bad"); Bad.Outcomes[0].TargetPointID=TEXT("Point.Missing"); C.Interactions.Add(Bad); TestTrue(TEXT("register C"),R.RegisterMap(C,E)); Issues.Reset(); TestFalse(TEXT("bad target rejected"),R.Validate(Issues)); TestTrue(TEXT("target issue"),Issues.Num()>0); return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAetherWorldContentDeterminismTest,"AgeOfAether.Data.WorldContent.Determinism",EAutomationTestFlags::EditorContext|EAutomationTestFlags::EngineFilter)
bool FAetherWorldContentDeterminismTest::RunTest(const FString&)
{
    FAetherWorldContentRegistry R; FString E; for(const FString& ID:{TEXT("Map.Z"),TEXT("Map.A"),TEXT("Map.M")})TestTrue(TEXT("register"),R.RegisterMap(MakeMap(ID),E));
    TArray<FString> IDs; R.GetMapIDs(IDs); TestTrue(TEXT("sorted"),IDs.Num()==3&&IDs[0]==TEXT("Map.A")&&IDs[1]==TEXT("Map.M")&&IDs[2]==TEXT("Map.Z")); return true;
}
