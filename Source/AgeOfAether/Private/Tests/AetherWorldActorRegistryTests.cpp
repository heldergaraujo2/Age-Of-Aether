#include "Misc/AutomationTest.h"
#include "Data/AetherWorldActorRegistry.h"
#include "Data/AetherContentRegistry.h"
#include "Data/AetherAssetRegistry.h"

namespace
{
FAetherMonsterDefinition Monster(const FString& ID)
{
    FAetherMonsterDefinition D; D.DefinitionID=ID; D.DisplayName=ID; D.Combat.Level=10; D.Combat.MaxHealth=1000; D.Combat.MinDamage=20; D.Combat.MaxDamage=30; D.Combat.Defense=10; D.Combat.AttackSpeed=1; D.Combat.MoveSpeed=1; D.AI.AttackRange=2; D.ExperienceReward=100; return D;
}
FAetherNPCDefinition NPC(const FString& ID){FAetherNPCDefinition D;D.DefinitionID=ID;D.DisplayName=ID;return D;}
FAetherBossDefinition Boss(const FString& ID)
{
    FAetherBossDefinition D; D.DefinitionID=ID; D.DisplayName=ID; D.Combat.Level=50; D.Combat.MaxHealth=100000; D.Combat.MinDamage=500; D.Combat.MaxDamage=800; D.Combat.Defense=200; D.Combat.AttackSpeed=1; D.Combat.MoveSpeed=1; D.AI.Behavior=EAetherAIBehavior::Boss; D.AI.BehaviorProfileID=TEXT("boss"); D.AI.AttackRange=5; FAetherBossPhase P;P.Phase=1;P.TriggerValue=.5;D.Phases.Add(P);return D;
}
FAetherContentDefinition Content(const FString& ID, EAetherContentType T){FAetherContentDefinition C;C.Type=T;C.Metadata.DefinitionID=ID;C.Metadata.DisplayName=FText::FromString(ID);return C;}
FAetherAssetDefinition Asset(const FString& ID,EAetherAssetType T){FAetherAssetDefinition A;A.Asset.AssetID=ID;A.Asset.AssetType=T;A.Asset.UnrealAssetPath=TEXT("/Game/Test/")+ID;return A;}
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAetherWorldActorBasicTest,"AgeOfAether.Data.WorldActor.Basic",EAutomationTestFlags::EditorContext|EAutomationTestFlags::EngineFilter)
bool FAetherWorldActorBasicTest::RunTest(const FString&)
{
    FAetherWorldActorRegistry R; FString E;
    TestTrue(TEXT("monster"),R.RegisterMonster(Monster(TEXT("Monster.Goblin.001")),E));
    TestTrue(TEXT("npc"),R.RegisterNPC(NPC(TEXT("NPC.Blacksmith.001")),E));
    TestTrue(TEXT("boss"),R.RegisterBoss(Boss(TEXT("Boss.Dragon.001")),E));
    TestEqual(TEXT("count"),R.NumMonsters()+R.NumNPCs()+R.NumBosses(),3);
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAetherWorldActorValidationTest,"AgeOfAether.Data.WorldActor.Validation",EAutomationTestFlags::EditorContext|EAutomationTestFlags::EngineFilter)
bool FAetherWorldActorValidationTest::RunTest(const FString&)
{
    FAetherWorldActorRegistry R; FString E;
    auto M=Monster(TEXT("Monster.Bad.001")); M.Combat.MinDamage=50;M.Combat.MaxDamage=10;
    TestFalse(TEXT("damage range rejected"),R.RegisterMonster(M,E));
    auto B=Boss(TEXT("Boss.Bad.001")); B.Phases[0].TriggerValue=1.5;
    TestFalse(TEXT("boss health trigger rejected"),R.RegisterBoss(B,E));
    auto A=Monster(TEXT("Monster.BadAI.001")); A.AI.Behavior=EAetherAIBehavior::Aggressive;
    TestFalse(TEXT("non-passive requires profile"),R.RegisterMonster(A,E));
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAetherWorldActorCrossReferenceTest,"AgeOfAether.Data.WorldActor.CrossReferences",EAutomationTestFlags::EditorContext|EAutomationTestFlags::EngineFilter)
bool FAetherWorldActorCrossReferenceTest::RunTest(const FString&)
{
    FAetherWorldActorRegistry R;FAetherContentRegistry C;FAetherAssetRegistry A;FString E;
    auto M=Monster(TEXT("Monster.Goblin.002")); M.SpawnAssetID=TEXT("Monster.Goblin.Mesh.002");
    TestTrue(TEXT("content"),C.RegisterDefinition(Content(M.DefinitionID,EAetherContentType::Monster),E));
    TestTrue(TEXT("asset"),A.RegisterAsset(Asset(M.SpawnAssetID,EAetherAssetType::SkeletalMesh),E));
    TestTrue(TEXT("monster"),R.RegisterMonster(M,E));
    TArray<FAetherWorldActorValidationIssue> I;
    TestTrue(TEXT("cross refs"),R.Validate(I,&C,&A)); TestEqual(TEXT("issues"),I.Num(),0);
    M=Monster(TEXT("Monster.Missing.001")); M.SpawnAssetID=TEXT("Asset.DoesNotExist"); TestTrue(TEXT("missing ref registers structurally"),R.RegisterMonster(M,E));
    I.Reset(); TestFalse(TEXT("missing asset detected"),R.Validate(I,nullptr,&A));
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAetherWorldActorDuplicateTest,"AgeOfAether.Data.WorldActor.Duplicate",EAutomationTestFlags::EditorContext|EAutomationTestFlags::EngineFilter)
bool FAetherWorldActorDuplicateTest::RunTest(const FString&)
{
    FAetherWorldActorRegistry R;FString E;
    TestTrue(TEXT("monster"),R.RegisterMonster(Monster(TEXT("World.Goblin.001")),E));
    TestFalse(TEXT("npc cannot reuse monster ID"),R.RegisterNPC(NPC(TEXT("World.Goblin.001")),E));
    TestFalse(TEXT("boss cannot reuse monster ID"),R.RegisterBoss(Boss(TEXT("World.Goblin.001")),E));
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAetherWorldActorDeterminismTest,"AgeOfAether.Data.WorldActor.Determinism",EAutomationTestFlags::EditorContext|EAutomationTestFlags::EngineFilter)
bool FAetherWorldActorDeterminismTest::RunTest(const FString&)
{
    FAetherWorldActorRegistry R;FString E;R.RegisterNPC(NPC(TEXT("NPC.Z")),E);R.RegisterMonster(Monster(TEXT("Monster.A")),E);R.RegisterBoss(Boss(TEXT("Boss.M")),E);
    TArray<FString> IDs;R.GetDefinitionIDs(IDs);
    TestEqual(TEXT("sorted 0"),IDs[0],FString(TEXT("Boss.M")));TestEqual(TEXT("sorted 1"),IDs[1],FString(TEXT("Monster.A")));TestEqual(TEXT("sorted 2"),IDs[2],FString(TEXT("NPC.Z")));
    return true;
}
