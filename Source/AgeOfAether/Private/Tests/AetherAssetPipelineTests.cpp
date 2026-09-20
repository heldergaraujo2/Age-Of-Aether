#include "Data/AetherAssetPipelineRegistry.h"

#include "Misc/AutomationTest.h"

namespace
{
    FAetherAssetPipelineRecord MakeRecord(const FString& ID)
    {
        FAetherAssetPipelineRecord Record;
        Record.AssetID = ID;
        Record.AssetType = EAetherAssetType::SkeletalMesh;
        Record.Status = EAetherAssetValidationStatus::Draft;
        Record.UnrealAssetPath = TEXT("/Game/Aether/Characters/Test/SK_Test.SK_Test");
        Record.ContentVersion = 1;
        Record.Source.SourceReference = TEXT("test/Test.fbx");
        Record.Source.SourceFormat = TEXT("FBX");
        Record.Source.SourceVersion = TEXT("2020.2");
        Record.Source.Origin = TEXT("internal-test");
        Record.Source.License = TEXT("test-license");
        Record.Source.Author = TEXT("test-author");
        Record.Source.SourceRevision = 1;
        return Record;
    }
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAetherAssetPipelineBasicTest,
    "AgeOfAether.AssetPipeline.Basic",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FAetherAssetPipelineBasicTest::RunTest(const FString& Parameters)
{
    FAetherAssetPipelineRegistry Registry;
    FString Error;

    TestTrue(TEXT("Valid record registers"), Registry.Register(MakeRecord(TEXT("Hero.Base")), Error));
    TestEqual(TEXT("One record registered"), Registry.Num(), 1);

    FAetherAssetPipelineRecord Resolved;
    TestTrue(TEXT("Normalized resolve works"), Registry.Resolve(TEXT(" HERO.BASE "), Resolved));
    TestEqual(TEXT("Resolved ID normalized"), Resolved.AssetID, FString(TEXT("hero.base")));
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAetherAssetPipelineValidationTest,
    "AgeOfAether.AssetPipeline.Validation",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FAetherAssetPipelineValidationTest::RunTest(const FString& Parameters)
{
    FAetherAssetPipelineRegistry Registry;
    FAetherAssetPipelineRecord Record = MakeRecord(TEXT("hero"));
    Record.DependencyAssetIDs.Add(TEXT("missing"));
    Record.FallbackAssetID = TEXT("missingFallback");

    FString Error;
    TestTrue(TEXT("Base registration succeeds"), Registry.Register(Record, Error));

    TArray<FAetherAssetPipelineIssue> Issues;
    TestFalse(TEXT("Missing references are rejected"), Registry.Validate(Issues));
    TestTrue(TEXT("Dependency issue exists"), Issues.ContainsByPredicate([](const FAetherAssetPipelineIssue& Issue)
    {
        return Issue.Code == TEXT("MissingDependency");
    }));
    TestTrue(TEXT("Fallback issue exists"), Issues.ContainsByPredicate([](const FAetherAssetPipelineIssue& Issue)
    {
        return Issue.Code == TEXT("MissingFallback");
    }));
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAetherAssetPipelineCycleTest,
    "AgeOfAether.AssetPipeline.DependencyCycle",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FAetherAssetPipelineCycleTest::RunTest(const FString& Parameters)
{
    FAetherAssetPipelineRegistry Registry;
    FAetherAssetPipelineRecord A = MakeRecord(TEXT("a"));
    FAetherAssetPipelineRecord B = MakeRecord(TEXT("b"));
    A.DependencyAssetIDs.Add(TEXT("b"));
    B.DependencyAssetIDs.Add(TEXT("a"));

    FString Error;
    TestTrue(TEXT("A registers"), Registry.Register(A, Error));
    TestTrue(TEXT("B registers"), Registry.Register(B, Error));

    TArray<FAetherAssetPipelineIssue> Issues;
    TestFalse(TEXT("Cycle is detected"), Registry.Validate(Issues));
    TestTrue(TEXT("Cycle issue exists"), Issues.ContainsByPredicate([](const FAetherAssetPipelineIssue& Issue)
    {
        return Issue.Code == TEXT("DependencyCycle");
    }));
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAetherAssetPipelineImportProfileTest,
    "AgeOfAether.AssetPipeline.ImportProfile",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FAetherAssetPipelineImportProfileTest::RunTest(const FString& Parameters)
{
    FAetherAssetImportProfile Profile;
    FString Error;
    TestTrue(TEXT("Default FBX profile is valid"), Profile.IsValid(Error));

    Profile.ImportScale = 0.0f;
    TestFalse(TEXT("Invalid scale is rejected"), Profile.IsValid(Error));
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAetherAssetPipelineDuplicateTest,
    "AgeOfAether.AssetPipeline.Duplicate",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FAetherAssetPipelineDuplicateTest::RunTest(const FString& Parameters)
{
    FAetherAssetPipelineRegistry Registry;
    FString Error;
    TestTrue(TEXT("First registration succeeds"), Registry.Register(MakeRecord(TEXT("hero")), Error));
    TestFalse(TEXT("Duplicate registration is rejected"), Registry.Register(MakeRecord(TEXT(" HERO ")), Error));
    return true;
}
