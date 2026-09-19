#include "Misc/AutomationTest.h"
#include "Data/AetherItemRegistry.h"
#include "Data/AetherContentRegistry.h"
#include "Data/AetherAssetRegistry.h"

namespace
{
FAetherItemDefinition MakeItem(const FString& ID, EAetherItemCategory Category = EAetherItemCategory::Material)
{
    FAetherItemDefinition Item;
    Item.DefinitionID = ID;
    Item.DisplayName = ID;
    Item.Category = Category;
    Item.MaxStack = 20;
    Item.MaxDurability = 100;
    Item.BaseDurability = 100;
    Item.MaxEnhancementLevel = 0;
    Item.Enhancements.AddDefaulted();
    return Item;
}
FAetherContentDefinition MakeContent(const FString& ID)
{
    FAetherContentDefinition Content;
    Content.Type = EAetherContentType::Item;
    Content.Metadata.DefinitionID = ID;
    Content.Metadata.DisplayName = FText::FromString(ID);
    return Content;
}
FAetherAssetDefinition MakeAsset(const FString& ID, EAetherAssetType Type = EAetherAssetType::Icon)
{
    FAetherAssetDefinition Asset;
    Asset.Asset.AssetID = ID;
    Asset.Asset.AssetType = Type;
    Asset.Asset.UnrealAssetPath = TEXT("/Game/Test/") + ID;
    return Asset;
}
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAetherItemRegistryBasicTest, "AgeOfAether.Data.ItemRegistry.Basic", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FAetherItemRegistryBasicTest::RunTest(const FString&)
{
    FAetherItemRegistry Registry;
    FString Error;
    TestTrue(TEXT("valid item registers"), Registry.RegisterItem(MakeItem(TEXT("Item.Material.Iron.001")), Error));
    TestEqual(TEXT("registry count"), Registry.Num(), 1);
    FAetherItemDefinition Resolved;
    TestTrue(TEXT("normalized resolve"), Registry.Resolve(TEXT(" Item.Material.Iron.001 "), Resolved));
    TestEqual(TEXT("resolved id"), Resolved.DefinitionID, FString(TEXT("Item.Material.Iron.001")));
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAetherItemRegistryValidationTest, "AgeOfAether.Data.ItemRegistry.Validation", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FAetherItemRegistryValidationTest::RunTest(const FString&)
{
    FAetherItemRegistry Registry;
    FString Error;
    FAetherItemDefinition Bad = MakeItem(TEXT("Item.Bad"));
    Bad.MaxStack = 0;
    TestFalse(TEXT("zero stack rejected"), Registry.RegisterItem(Bad, Error));
    Bad = MakeItem(TEXT("Item.BadDurability")); Bad.BaseDurability = 101; Bad.MaxDurability = 100;
    TestFalse(TEXT("invalid durability rejected"), Registry.RegisterItem(Bad, Error));
    Bad = MakeItem(TEXT("Item.BadWeight")); Bad.Weight = -1.0;
    TestFalse(TEXT("negative weight rejected"), Registry.RegisterItem(Bad, Error));
    Bad = MakeItem(TEXT("Item.BadEnhancement")); Bad.MaxEnhancementLevel = 1; Bad.Enhancements[0].SuccessChance = 1.5; Bad.Enhancements.AddDefaulted();
    TestFalse(TEXT("invalid enhancement chance rejected"), Registry.RegisterItem(Bad, Error));
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAetherItemRegistryEquipmentTest, "AgeOfAether.Data.ItemRegistry.Equipment", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FAetherItemRegistryEquipmentTest::RunTest(const FString&)
{
    FAetherItemRegistry Registry;
    FString Error;
    FAetherItemDefinition Weapon = MakeItem(TEXT("Item.Weapon.IronSword.001"), EAetherItemCategory::Equipment);
    Weapon.EquipmentSlot = EAetherEquipmentSlot::MainHand;
    Weapon.AllowedClasses = { TEXT("Warrior"), TEXT("Knight") };
    Weapon.RequiredCharacterLevel = 10;
    Weapon.Requirements.CharacterLevel = 10;
    Weapon.BaseStats.Add(TEXT("Attack"), 25.0);
    TestTrue(TEXT("equipment registers"), Registry.RegisterItem(Weapon, Error));
    FAetherItemDefinition Invalid = MakeItem(TEXT("Item.Weapon.Invalid"), EAetherItemCategory::Equipment);
    TestFalse(TEXT("equipment without slot rejected"), Registry.RegisterItem(Invalid, Error));
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAetherItemRegistryEnhancementTest, "AgeOfAether.Data.ItemRegistry.Enhancement", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FAetherItemRegistryEnhancementTest::RunTest(const FString&)
{
    FAetherItemRegistry Registry;
    FString Error;
    TestTrue(TEXT("material registers"), Registry.RegisterItem(MakeItem(TEXT("Item.Material.EnhanceStone.001")), Error));
    FAetherItemDefinition Sword = MakeItem(TEXT("Item.Weapon.DragonSword.001"), EAetherItemCategory::Equipment);
    Sword.EquipmentSlot = EAetherEquipmentSlot::MainHand;
    Sword.MaxEnhancementLevel = 2;
    Sword.Enhancements.SetNum(3);
    Sword.Enhancements[0].Level = 0; Sword.Enhancements[1].Level = 1; Sword.Enhancements[2].Level = 2;
    Sword.Enhancements[1].Materials.Add({ TEXT("Item.Material.EnhanceStone.001"), 2 });
    Sword.Enhancements[1].SuccessChance = 0.75;
    Sword.Enhancements[1].CurrencyAmount = 100;
    Sword.Enhancements[1].FailureBehavior = EAetherEnhancementFailureBehavior::Downgrade;
    Sword.Enhancements[1].StatScaling.Add(TEXT("Attack"), 1.15);
    TestTrue(TEXT("enhanced item registers"), Registry.RegisterItem(Sword, Error));
    TArray<FAetherItemValidationIssue> Issues;
    TestTrue(TEXT("enhancement graph validates"), Registry.Validate(Issues));
    TestEqual(TEXT("no validation issues"), Issues.Num(), 0);
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAetherItemRegistryCrossReferenceTest, "AgeOfAether.Data.ItemRegistry.CrossReferences", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FAetherItemRegistryCrossReferenceTest::RunTest(const FString&)
{
    FAetherItemRegistry Registry; FAetherContentRegistry Content; FAetherAssetRegistry Assets; FString Error;
    const FString ItemID = TEXT("Item.Weapon.AetherBlade.001");
    const FString IconID = TEXT("Item.Icon.AetherBlade.001");
    const FString MeshID = TEXT("Item.Mesh.AetherBlade.001");
    TestTrue(TEXT("content registers"), Content.RegisterDefinition(MakeContent(ItemID), Error));
    TestTrue(TEXT("icon registers"), Assets.RegisterAsset(MakeAsset(IconID), Error));
    TestTrue(TEXT("mesh registers"), Assets.RegisterAsset(MakeAsset(MeshID, EAetherAssetType::SkeletalMesh), Error));
    FAetherItemDefinition Item = MakeItem(ItemID, EAetherItemCategory::Equipment);
    Item.EquipmentSlot = EAetherEquipmentSlot::MainHand;
    Item.Visuals.IconAssetID = IconID; Item.Visuals.EquippedVisualAssetID = MeshID;
    TestTrue(TEXT("item registers"), Registry.RegisterItem(Item, Error));
    TArray<FAetherItemValidationIssue> Issues;
    TestTrue(TEXT("cross references validate"), Registry.Validate(Issues, &Content, &Assets));
    TestEqual(TEXT("cross reference issues"), Issues.Num(), 0);
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAetherItemRegistryDuplicateAndDeterminismTest, "AgeOfAether.Data.ItemRegistry.DuplicateAndDeterminism", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FAetherItemRegistryDuplicateAndDeterminismTest::RunTest(const FString&)
{
    FAetherItemRegistry Registry; FString Error;
    Registry.RegisterItem(MakeItem(TEXT("Item.Z")), Error);
    Registry.RegisterItem(MakeItem(TEXT("Item.A")), Error);
    TestFalse(TEXT("duplicate rejected"), Registry.RegisterItem(MakeItem(TEXT("Item.Z")), Error));
    TArray<FString> IDs; Registry.GetDefinitionIDs(IDs);
    TestEqual(TEXT("sorted first"), IDs[0], FString(TEXT("Item.A")));
    TestEqual(TEXT("sorted second"), IDs[1], FString(TEXT("Item.Z")));
    return true;
}
