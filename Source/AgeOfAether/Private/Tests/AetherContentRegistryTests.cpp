#include "Misc/AutomationTest.h"
#include "Data/AetherContentRegistry.h"

namespace
{
FAetherContentDefinition MakeDefinition(const TCHAR* ID, EAetherContentType Type = EAetherContentType::Generic)
{
    FAetherContentDefinition Definition;
    Definition.Type = Type;
    Definition.Metadata.DefinitionID = ID;
    Definition.Metadata.SchemaVersion = 1;
    Definition.Metadata.ContentVersion = 1;
    return Definition;
}
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAetherContentRegistryRegistrationTest,
    "AgeOfAether.ContentRegistry.Registration",
    EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::EngineFilter)

bool FAetherContentRegistryRegistrationTest::RunTest(const FString&)
{
    FAetherContentRegistry Registry;
    FAetherContentDefinition Item = MakeDefinition(TEXT("Item.Sword.Iron.001"), EAetherContentType::Item);

    FString Error;
    TestTrue(TEXT("First definition registers"), Registry.RegisterDefinition(Item, Error));
    TestTrue(TEXT("Definition resolves"), Registry.Contains(TEXT("Item.Sword.Iron.001")));

    FAetherContentDefinition Resolved;
    TestTrue(TEXT("Definition can be resolved"), Registry.Resolve(TEXT(" Item.Sword.Iron.001 "), Resolved));
    TestEqual(TEXT("Resolved ID normalized"), Resolved.Metadata.DefinitionID, FString(TEXT("Item.Sword.Iron.001")));

    FAetherContentDefinition Duplicate = Item;
    TestFalse(TEXT("Duplicate ID rejected"), Registry.RegisterDefinition(Duplicate, Error));
    TestTrue(TEXT("Duplicate reports an error"), !Error.IsEmpty());
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAetherContentRegistryReferenceTest,
    "AgeOfAether.ContentRegistry.References",
    EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::EngineFilter)

bool FAetherContentRegistryReferenceTest::RunTest(const FString&)
{
    FAetherContentRegistry Registry;
    FAetherContentDefinition Recipe = MakeDefinition(TEXT("Recipe.Weapon.DragonSword.001"), EAetherContentType::Recipe);
    Recipe.References.Add(TEXT("Item.Fragment.Dragon.001"));

    FString Error;
    TestTrue(TEXT("Recipe registers"), Registry.RegisterDefinition(Recipe, Error));

    TArray<FAetherContentValidationIssue> Issues;
    TestFalse(TEXT("Missing reference is rejected"), Registry.Validate(Issues));
    TestTrue(TEXT("Missing reference issue exists"), Issues.ContainsByPredicate(
        [](const FAetherContentValidationIssue& Issue)
        {
            return Issue.Code == TEXT("MissingReference");
        }));

    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAetherContentRegistryCycleTest,
    "AgeOfAether.ContentRegistry.Cycles",
    EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::EngineFilter)

bool FAetherContentRegistryCycleTest::RunTest(const FString&)
{
    FAetherContentRegistry Registry;
    FAetherContentDefinition A = MakeDefinition(TEXT("Quest.A.001"), EAetherContentType::Quest);
    FAetherContentDefinition B = MakeDefinition(TEXT("Quest.B.001"), EAetherContentType::Quest);
    A.References.Add(B.Metadata.DefinitionID);
    B.References.Add(A.Metadata.DefinitionID);

    FString Error;
    TestTrue(TEXT("A registers"), Registry.RegisterDefinition(A, Error));
    TestTrue(TEXT("B registers"), Registry.RegisterDefinition(B, Error));

    TArray<FAetherContentValidationIssue> Issues;
    TestFalse(TEXT("Cycle rejected"), Registry.Validate(Issues));
    TestTrue(TEXT("Cycle issue exists"), Issues.ContainsByPredicate(
        [](const FAetherContentValidationIssue& Issue)
        {
            return Issue.Code == TEXT("CircularDependency");
        }));

    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAetherContentRegistryNumericValidationTest,
    "AgeOfAether.ContentRegistry.NumericValidation",
    EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::EngineFilter)

bool FAetherContentRegistryNumericValidationTest::RunTest(const FString&)
{
    FAetherContentRegistry Registry;
    FAetherContentDefinition Recipe = MakeDefinition(TEXT("Recipe.Test.001"), EAetherContentType::Recipe);
    Recipe.NumericFields.Add(TEXT("SuccessChance"), 1.2);
    Recipe.ProbabilityFields.Add(TEXT("SuccessChance"));

    FString Error;
    TestFalse(TEXT("Invalid probability rejected at registration"),
        Registry.RegisterDefinition(Recipe, Error));
    TestTrue(TEXT("Numeric validation reports error"), !Error.IsEmpty());

    FAetherContentDefinition Valid = MakeDefinition(TEXT("Recipe.Valid.001"), EAetherContentType::Recipe);
    Valid.NumericFields.Add(TEXT("SuccessChance"), 0.75);
    Valid.ProbabilityFields.Add(TEXT("SuccessChance"));
    TestTrue(TEXT("Valid probability accepted"), Registry.RegisterDefinition(Valid, Error));
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAetherContentRegistryDeterministicOrderTest,
    "AgeOfAether.ContentRegistry.DeterministicOrder",
    EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::EngineFilter)

bool FAetherContentRegistryDeterministicOrderTest::RunTest(const FString&)
{
    FAetherContentRegistry Registry;
    FString Error;

    Registry.RegisterDefinition(MakeDefinition(TEXT("Item.Z.001")), Error);
    Registry.RegisterDefinition(MakeDefinition(TEXT("Item.A.001")), Error);
    Registry.RegisterDefinition(MakeDefinition(TEXT("Item.M.001")), Error);

    TArray<FString> IDs;
    Registry.GetDefinitionIDs(IDs);

    TestEqual(TEXT("Three definitions"), IDs.Num(), 3);
    TestEqual(TEXT("Sorted first"), IDs[0], FString(TEXT("Item.A.001")));
    TestEqual(TEXT("Sorted second"), IDs[1], FString(TEXT("Item.M.001")));
    TestEqual(TEXT("Sorted third"), IDs[2], FString(TEXT("Item.Z.001")));
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAetherContentRegistryNonNegativeTest,
    "AgeOfAether.ContentRegistry.NonNegative",
    EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::EngineFilter)

bool FAetherContentRegistryNonNegativeTest::RunTest(const FString&)
{
    FAetherContentRegistry Registry;
    FAetherContentDefinition Item = MakeDefinition(TEXT("Item.Stack.Test.001"), EAetherContentType::Item);
    Item.NumericFields.Add(TEXT("StackSize"), -1.0);
    Item.NonNegativeFields.Add(TEXT("StackSize"));

    FString Error;
    TestFalse(TEXT("Negative quantity rejected"), Registry.RegisterDefinition(Item, Error));
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAetherContentRegistrySelfCycleTest,
    "AgeOfAether.ContentRegistry.SelfCycle",
    EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::EngineFilter)

bool FAetherContentRegistrySelfCycleTest::RunTest(const FString&)
{
    FAetherContentRegistry Registry;
    FAetherContentDefinition Definition = MakeDefinition(TEXT("Quest.Self.001"), EAetherContentType::Quest);
    Definition.References.Add(Definition.Metadata.DefinitionID);

    FString Error;
    TestTrue(TEXT("Definition registers"), Registry.RegisterDefinition(Definition, Error));

    TArray<FAetherContentValidationIssue> Issues;
    TestFalse(TEXT("Self cycle rejected"), Registry.Validate(Issues));
    return true;
}
