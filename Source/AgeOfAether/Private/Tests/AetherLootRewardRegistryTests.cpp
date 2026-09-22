#include "Misc/AutomationTest.h"
#include "Data/AetherLootRewardRegistry.h"
#include "Data/AetherItemRegistry.h"
#include "Data/AetherWorldActorRegistry.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAetherLootRewardBasicTest,"AgeOfAether.Data.LootReward.Basic",EAutomationTestFlags::EditorContext|EAutomationTestFlags::EngineFilter)
bool FAetherLootRewardBasicTest::RunTest(const FString&)
{
    FAetherLootRewardRegistry R; FString E;
    FAetherLootTableDefinition L; L.DefinitionID=TEXT("Loot.Goblin"); L.DisplayName=TEXT("Goblin Loot");
    FAetherDataLootEntry Entry; Entry.ItemID=TEXT("Item.Coin"); Entry.Weight=1.0; Entry.Chance=1.0; L.Entries.Add(Entry);
    FAetherRewardDefinition Reward; Reward.DefinitionID=TEXT("Reward.Goblin"); Reward.DisplayName=TEXT("Goblin Reward"); Reward.Experience=100.0;
    FAetherRespawnDefinition Respawn; Respawn.DefinitionID=TEXT("Respawn.Goblin"); Respawn.WorldActorID=TEXT("Monster.Goblin.001"); Respawn.RespawnSeconds=30.0;
    TestTrue(TEXT("loot registration"),R.RegisterLootTable(L,E));
    TestTrue(TEXT("reward registration"),R.RegisterReward(Reward,E));
    TestTrue(TEXT("respawn registration"),R.RegisterRespawn(Respawn,E));
    FAetherDropRuleDefinition Drop; Drop.DefinitionID=TEXT("Drop.Goblin"); Drop.SourceWorldActorID=TEXT("Monster.Goblin.001"); Drop.LootTableID=TEXT("Loot.Goblin");
    FAetherSpawnGroupDefinition Group; Group.DefinitionID=TEXT("Spawn.Goblin"); Group.WorldActorID=TEXT("Monster.Goblin.001"); Group.RespawnDefinitionID=TEXT("Respawn.Goblin"); Group.InitialCount=2; Group.MaximumCount=3;
    TestTrue(TEXT("drop registration"),R.RegisterDropRule(Drop,E));
    TestTrue(TEXT("spawn group registration"),R.RegisterSpawnGroup(Group,E));
    TestEqual(TEXT("counts"),R.NumLootTables()+R.NumRewards()+R.NumRespawns()+R.NumDropRules()+R.NumSpawnGroups(),5);
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAetherLootRewardValidationTest,"AgeOfAether.Data.LootReward.Validation",EAutomationTestFlags::EditorContext|EAutomationTestFlags::EngineFilter)
bool FAetherLootRewardValidationTest::RunTest(const FString&)
{
    FAetherLootRewardRegistry R; FString E;
    FAetherLootTableDefinition L; L.DefinitionID=TEXT("Loot.Bad"); L.DisplayName=TEXT("Bad");
    FAetherDataLootEntry Entry; Entry.ItemID=TEXT("Item.Bad"); Entry.MinQuantity=0; L.Entries.Add(Entry);
    TestFalse(TEXT("invalid quantity"),R.RegisterLootTable(L,E));
    FAetherLootTableDefinition W; W.DefinitionID=TEXT("Loot.BadWeight"); W.DisplayName=TEXT("Bad Weight");
    FAetherDataLootEntry WEntry; WEntry.ItemID=TEXT("Item.X"); WEntry.Weight=0.0; W.Entries.Add(WEntry);
    TestFalse(TEXT("zero total weight"),R.RegisterLootTable(W,E));
    FAetherRewardDefinition Empty; Empty.DefinitionID=TEXT("Reward.Empty"); Empty.DisplayName=TEXT("Empty");
    TestFalse(TEXT("empty reward"),R.RegisterReward(Empty,E));
    FAetherDropRuleDefinition BadDrop; BadDrop.DefinitionID=TEXT("Drop.Bad"); BadDrop.SourceWorldActorID=TEXT("Monster.X"); BadDrop.LootTableID=TEXT("Loot.X"); BadDrop.Chance=2; TestFalse(TEXT("bad drop chance"),R.RegisterDropRule(BadDrop,E));
    FAetherSpawnGroupDefinition BadGroup; BadGroup.DefinitionID=TEXT("Spawn.Bad"); BadGroup.WorldActorID=TEXT("Monster.X"); BadGroup.RespawnDefinitionID=TEXT("Respawn.X"); BadGroup.InitialCount=4; BadGroup.MaximumCount=3; TestFalse(TEXT("bad spawn counts"),R.RegisterSpawnGroup(BadGroup,E));
    FAetherRespawnDefinition BadR; BadR.DefinitionID=TEXT("Respawn.Bad"); BadR.WorldActorID=TEXT("Monster.X"); BadR.RespawnSeconds=5; BadR.RespawnJitterSeconds=6;
    TestFalse(TEXT("bad jitter"),R.RegisterRespawn(BadR,E));
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAetherLootRewardCrossReferenceTest,"AgeOfAether.Data.LootReward.CrossReferences",EAutomationTestFlags::EditorContext|EAutomationTestFlags::EngineFilter)
bool FAetherLootRewardCrossReferenceTest::RunTest(const FString&)
{
    FAetherLootRewardRegistry R; FString E;
    FAetherLootTableDefinition L; L.DefinitionID=TEXT("Loot.Missing"); L.DisplayName=TEXT("Missing");
    FAetherDataLootEntry Entry; Entry.ItemID=TEXT("Item.Nope"); L.Entries.Add(Entry); R.RegisterLootTable(L,E);
    FAetherRespawnDefinition Respawn; Respawn.DefinitionID=TEXT("Respawn.Missing"); Respawn.WorldActorID=TEXT("Monster.Nope"); Respawn.RespawnSeconds=10; R.RegisterRespawn(Respawn,E);
    FAetherDropRuleDefinition Drop; Drop.DefinitionID=TEXT("Drop.Missing"); Drop.SourceWorldActorID=TEXT("Monster.Nope"); Drop.LootTableID=TEXT("Loot.Nope"); R.RegisterDropRule(Drop,E);
    FAetherSpawnGroupDefinition Group; Group.DefinitionID=TEXT("Spawn.Missing"); Group.WorldActorID=TEXT("Monster.Nope"); Group.RespawnDefinitionID=TEXT("Respawn.Nope"); R.RegisterSpawnGroup(Group,E);
    FAetherItemRegistry ItemRegistry; FAetherWorldActorRegistry WorldActorRegistry;
    TArray<FAetherLootRewardValidationIssue> Issues;
    TestFalse(TEXT("missing refs"),R.Validate(Issues,nullptr,nullptr,&ItemRegistry,&WorldActorRegistry));
    TestTrue(TEXT("missing item"),Issues.ContainsByPredicate([](const FAetherLootRewardValidationIssue& I){return I.Code==TEXT("MissingItemReference");}));
    TestTrue(TEXT("missing world actor"),Issues.ContainsByPredicate([](const FAetherLootRewardValidationIssue& I){return I.Code==TEXT("MissingWorldActorReference");}));
    TestTrue(TEXT("missing loot table"),Issues.ContainsByPredicate([](const FAetherLootRewardValidationIssue& I){return I.Code==TEXT("MissingLootTableReference");}));
    TestTrue(TEXT("missing respawn"),Issues.ContainsByPredicate([](const FAetherLootRewardValidationIssue& I){return I.Code==TEXT("MissingRespawnReference");}));
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAetherLootRewardDuplicateTest,"AgeOfAether.Data.LootReward.Duplicate",EAutomationTestFlags::EditorContext|EAutomationTestFlags::EngineFilter)
bool FAetherLootRewardDuplicateTest::RunTest(const FString&)
{
    FAetherLootRewardRegistry R; FString E;
    FAetherLootTableDefinition L; L.DefinitionID=TEXT("Shared.ID"); L.DisplayName=TEXT("Loot"); L.Entries.Add(FAetherDataLootEntry());
    L.Entries[0].ItemID=TEXT("Item.X");
    TestTrue(TEXT("first"),R.RegisterLootTable(L,E));
    FAetherRewardDefinition Reward; Reward.DefinitionID=TEXT("Shared.ID"); Reward.DisplayName=TEXT("Reward"); Reward.Experience=1;
    TestFalse(TEXT("cross-type duplicate"),R.RegisterReward(Reward,E));
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAetherLootRewardDeterminismTest,"AgeOfAether.Data.LootReward.Determinism",EAutomationTestFlags::EditorContext|EAutomationTestFlags::EngineFilter)
bool FAetherLootRewardDeterminismTest::RunTest(const FString&)
{
    FAetherLootRewardRegistry R; FString E;
    FAetherLootTableDefinition L; L.DefinitionID=TEXT("Loot.Z"); L.DisplayName=TEXT("Z"); L.Entries.Add(FAetherDataLootEntry()); L.Entries[0].ItemID=TEXT("Item.X");
    R.RegisterLootTable(L,E);
    L.DefinitionID=TEXT("Loot.A"); R.RegisterLootTable(L,E);
    FAetherRewardDefinition Reward; Reward.DefinitionID=TEXT("Reward.M"); Reward.DisplayName=TEXT("M"); Reward.Experience=1; R.RegisterReward(Reward,E);
    TArray<FString> IDs; R.GetDefinitionIDs(IDs);
    TestEqual(TEXT("count"),IDs.Num(),3);
    TestEqual(TEXT("first"),IDs[0],FString(TEXT("Loot.A")));
    TestEqual(TEXT("second"),IDs[1],FString(TEXT("Loot.Z")));
    TestEqual(TEXT("third"),IDs[2],FString(TEXT("Reward.M")));
    return true;
}
