#include "Misc/AutomationTest.h"
#include "Data/AetherAssetRegistry.h"

namespace
{
FAetherAssetDefinition MakeAsset(const TCHAR* ID, EAetherAssetType Type)
{
    FAetherAssetDefinition Definition;
    Definition.Asset.AssetID = ID;
    Definition.Asset.AssetType = Type;
    Definition.SourceFormat = TEXT("FBX");
    return Definition;
}
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAetherAssetRegistryRegistrationTest,
    "AgeOfAether.AssetRegistry.Registration",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::EngineFilter)

bool FAetherAssetRegistryRegistrationTest::RunTest(const FString&)
{
    FAetherAssetRegistry Registry;
    FString Error;

    const FAetherAssetDefinition Mesh = MakeAsset(TEXT("Monster.Goblin.Mesh.001"), EAetherAssetType::SkeletalMesh);
    TestTrue(TEXT("Mesh registers"), Registry.RegisterAsset(Mesh, Error));

    FAetherAssetDefinition Resolved;
    TestTrue(TEXT("Asset resolves with normalized ID"), Registry.Resolve(TEXT(" Monster.Goblin.Mesh.001 "), Resolved));
    TestEqual(TEXT("Resolved ID"), Resolved.Asset.AssetID, FString(TEXT("Monster.Goblin.Mesh.001")));

    TestFalse(TEXT("Duplicate asset rejected"), Registry.RegisterAsset(Mesh, Error));
    TestTrue(TEXT("Duplicate has error"), !Error.IsEmpty());
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAetherAssetRegistryFallbackTest,
    "AgeOfAether.AssetRegistry.Fallback",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::EngineFilter)

bool FAetherAssetRegistryFallbackTest::RunTest(const FString&)
{
    FAetherAssetRegistry Registry;
    FString Error;

    FAetherAssetDefinition Broken = MakeAsset(TEXT("Monster.Goblin.Mesh.001"), EAetherAssetType::SkeletalMesh);
    Broken.bHasFallback = true;
    Broken.Asset.FallbackAssetID = TEXT("Missing.Fallback");
    TestTrue(TEXT("Definition registers before cross-reference validation"), Registry.RegisterAsset(Broken, Error));

    TArray<FAetherAssetValidationIssue> Issues;
    TestFalse(TEXT("Missing fallback fails validation"), Registry.Validate(Issues));
    TestTrue(TEXT("Fallback issue exists"), Issues.ContainsByPredicate(
        [](const FAetherAssetValidationIssue& Issue)
        {
            return Issue.Code == TEXT("MissingFallback");
        }));
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAetherAssetRegistrySkeletonTest,
    "AgeOfAether.AssetRegistry.Skeleton",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::EngineFilter)

bool FAetherAssetRegistrySkeletonTest::RunTest(const FString&)
{
    FAetherAssetRegistry Registry;
    FString Error;

    FAetherAssetDefinition Character = MakeAsset(TEXT("Character.Hero.Mesh.001"), EAetherAssetType::SkeletalMesh);
    Character.bRequiresSkeleton = true;
    TestFalse(TEXT("Missing required skeleton rejected"), Registry.RegisterAsset(Character, Error));
    TestTrue(TEXT("Skeleton validation has error"), !Error.IsEmpty());

    Character.Asset.SkeletonAssetID = TEXT("Character.Hero.Skeleton.001");
    TestTrue(TEXT("Definition registers once skeleton is declared"), Registry.RegisterAsset(Character, Error));

    TArray<FAetherAssetValidationIssue> Issues;
    TestFalse(TEXT("Missing declared skeleton fails cross-reference validation"), Registry.Validate(Issues));
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAetherAssetRegistryNumericTest,
    "AgeOfAether.AssetRegistry.NumericValidation",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::EngineFilter)

bool FAetherAssetRegistryNumericTest::RunTest(const FString&)
{
    FAetherAssetRegistry Registry;
    FString Error;
    FAetherAssetDefinition Asset = MakeAsset(TEXT("Texture.BadScale.001"), EAetherAssetType::Texture);
    Asset.Asset.Scale.X = 0.0f;
    TestFalse(TEXT("Zero scale rejected"), Registry.RegisterAsset(Asset, Error));

    Asset.Asset.Scale = FVector(1.0, 1.0, 1.0);
    TestTrue(TEXT("Positive scale accepted"), Registry.RegisterAsset(Asset, Error));
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAetherAssetRegistryDependencyTest,
    "AgeOfAether.AssetRegistry.Dependencies",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::EngineFilter)

bool FAetherAssetRegistryDependencyTest::RunTest(const FString&)
{
    FAetherAssetRegistry Registry;
    FString Error;

    FAetherAssetDefinition Mesh = MakeAsset(TEXT("Character.Hero.Mesh.001"), EAetherAssetType::SkeletalMesh);
    Mesh.Asset.SkeletonAssetID = TEXT("Character.Hero.Skeleton.001");
    Mesh.bRequiresSkeleton = true;

    FAetherAssetDefinition Skeleton = MakeAsset(TEXT("Character.Hero.Skeleton.001"), EAetherAssetType::SkeletalMesh);

    TestTrue(TEXT("Mesh registers"), Registry.RegisterAsset(Mesh, Error));
    TestTrue(TEXT("Skeleton registers"), Registry.RegisterAsset(Skeleton, Error));

    TArray<FAetherAssetValidationIssue> Issues;
    TestTrue(TEXT("Valid dependency graph passes"), Registry.Validate(Issues));
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAetherAssetRegistryDeterministicOrderTest,
    "AgeOfAether.AssetRegistry.DeterministicOrder",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::EngineFilter)

bool FAetherAssetRegistryDeterministicOrderTest::RunTest(const FString&)
{
    FAetherAssetRegistry Registry;
    FString Error;

    Registry.RegisterAsset(MakeAsset(TEXT("Asset.Z.001"), EAetherAssetType::Icon), Error);
    Registry.RegisterAsset(MakeAsset(TEXT("Asset.A.001"), EAetherAssetType::Icon), Error);

    TArray<FString> IDs;
    Registry.GetAssetIDs(IDs);

    TestEqual(TEXT("Two assets"), IDs.Num(), 2);
    TestEqual(TEXT("Sorted first"), IDs[0], FString(TEXT("Asset.A.001")));
    TestEqual(TEXT("Sorted second"), IDs[1], FString(TEXT("Asset.Z.001")));
    return true;
}
