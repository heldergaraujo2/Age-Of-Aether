#include "Misc/AutomationTest.h"
#include "Data/AetherLootRewardRegistry.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAetherLootRewardBasicTest,"AgeOfAether.Data.LootReward.Basic",EAutomationTestFlags::EditorContext|EAutomationTestFlags::EngineFilter)
bool FAetherLootRewardBasicTest::RunTest(const FString&)
{
    FAetherLootRewardRegistry R; FString E;
    FAetherLootTableDefinition L; L.DefinitionID=TEXT("Loot.Goblin"); L.DisplayName=TEXT("Goblin Loot");
    FAetherLootEntry Entry; Entry.ItemID=TEXT("Item.Coin"); Entry.Weight=1.0; Entry.Chance=1.0; L.Entries.Add(Entry);
    FAetherRewardDefinition Reward; Reward.DefinitionID=TEXT("Reward.Goblin"); Reward.DisplayName=TEXT("Goblin Reward"); Reward.Experience=100.0;
    FAetherRespawnDefinition Respawn; Respawn.DefinitionID=TEXT("Respawn.Goblin"); Respawn.WorldActorID=TEXT("Monster.Goblin.001"); Respawn.RespawnSeconds=30.0;
    TestTrue(TEXT("loot registration"),R.RegisterLootTable(L,E));
    TestTrue(TEXT("reward registration"),R.RegisterReward(Reward,E));
    TestTrue(TEXT("respawn registration"),R.RegisterRespawn(Respawn,E));
    TestEqual(TEXT("counts"),R.NumLootTables()+R.NumRewards()+R.NumRespawns(),3);
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAetherLootRewardValidationTest,"AgeOfAether.Data.LootReward.Validation",EAutomationTestFlags::EditorContext|EAutomationTestFlags::EngineFilter)
bool FAetherLootRewardValidationTest::RunTest(const FString&)
{
    FAetherLootRewardRegistry R; FString E;
    FAetherLootTableDefinition L; L.DefinitionID=TEXT("Loot.Bad"); L.DisplayName=TEXT("Bad");
    FAetherLootEntry Entry; Entry.ItemID=TEXT("Item.Bad"); Entry.MinQuantity=0; L.Entries.Add(Entry);
    TestFalse(TEXT("invalid quantity"),R.RegisterLootTable(L,E));
    FAetherLootTableDefinition W; W.DefinitionID=TEXT("Loot.BadWeight"); W.DisplayName=TEXT("Bad Weight");
    FAetherLootEntry WEntry; WEntry.ItemID=TEXT("Item.X"); WEntry.Weight=0.0; W.Entries.Add(WEntry);
    TestFalse(TEXT("zero total weight"),R.RegisterLootTable(W,E));
    FAetherRewardDefinition Empty; Empty.DefinitionID=TEXT("Reward.Empty"); Empty.DisplayName=TEXT("Empty");
    TestFalse(TEXT("empty reward"),R.RegisterReward(Empty,E));
    FAetherRespawnDefinition BadR; BadR.DefinitionID=TEXT("Respawn.Bad"); BadR.WorldActorID=TEXT("Monster.X"); BadR.RespawnSeconds=5; BadR.RespawnJitterSeconds=6;
    TestFalse(TEXT("bad jitter"),R.RegisterRespawn(BadR,E));
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAetherLootRewardCrossReferenceTest,"AgeOfAether.Data.LootReward.CrossReferences",EAutomationTestFlags::EditorContext|EAutomationTestFlags::EngineFilter)
bool FAetherLootRewardCrossReferenceTest::RunTest(const FString&)
{
    FAetherLootRewardRegistry R; FString E;
    FAetherLootTableDefinition L; L.DefinitionID=TEXT("Loot.Missing"); L.DisplayName=TEXT("Missing");
    FAetherLootEntry Entry; Entry.ItemID=TEXT("Item.Nope"); L.Entries.Add(Entry); R.RegisterLootTable(L,E);
    FAetherRespawnDefinition Respawn; Respawn.DefinitionID=TEXT("Respawn.Missing"); Respawn.WorldActorID=TEXT("Monster.Nope"); Respawn.RespawnSeconds=10; R.RegisterRespawn(Respawn,E);
    TArray<FAetherLootRewardValidationIssue> Issues;
    TestFalse(TEXT("missing refs"),R.Validate(Issues));
    TestTrue(TEXT("missing item"),Issues.ContainsByPredicate([](const FAetherLootRewardValidationIssue& I){return I.Code==TEXT("MissingItemReference");}));
    TestTrue(TEXT("missing world actor"),Issues.ContainsByPredicate([](const FAetherLootRewardValidationIssue& I){return I.Code==TEXT("MissingWorldActorReference");}));
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAetherLootRewardDuplicateTest,"AgeOfAether.Data.LootReward.Duplicate",EAutomationTestFlags::EditorContext|EAutomationTestFlags::EngineFilter)
bool FAetherLootRewardDuplicateTest::RunTest(const FString&)
{
    FAetherLootRewardRegistry R; FString E;
    FAetherLootTableDefinition L; L.DefinitionID=TEXT("Shared.ID"); L.DisplayName=TEXT("Loot"); L.Entries.Add(FAetherLootEntry());
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
    FAetherLootTableDefinition L; L.DefinitionID=TEXT("Loot.Z"); L.DisplayName=TEXT("Z"); L.Entries.Add(FAetherLootEntry()); L.Entries[0].ItemID=TEXT("Item.X");
    R.RegisterLootTable(L,E);
    L.DefinitionID=TEXT("Loot.A"); R.RegisterLootTable(L,E);
    FAetherRewardDefinition Reward; Reward.DefinitionID=TEXT("Reward.M"); Reward.DisplayName=TEXT("M"); Reward.Experience=1; R.RegisterReward(Reward,E);
    TArray<FString> IDs; R.GetDefinitionIDs(IDs);
    TestEqual(TEXT("first"),IDs[0],FString(TEXT("Loot.A")));
    TestEqual(TEXT("second"),IDs[1],FString(TEXT("Loot.Z")));
    TestEqual(TEXT("third"),IDs[2],FString(TEXT("Reward.M")));
    return true;
}
