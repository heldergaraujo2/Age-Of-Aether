#include "Misc/AutomationTest.h"
#include "Data/AetherRecipeRegistry.h"
#include "Data/AetherSkillEffectRegistry.h"
#include "Data/AetherQuestDialogueEventRegistry.h"
#include "Data/AetherWorldActorRegistry.h"

namespace
{
FAetherRecipeDefinition MakeRecipe(const FString& ID)
{
    FAetherRecipeDefinition R;R.DefinitionID=ID;R.DisplayName=ID;
    FAetherRecipeIngredient I;I.ItemID=TEXT("Item.Iron");I.Quantity=2;R.Ingredients.Add(I);
    FAetherRecipeOutput O;O.ItemID=TEXT("Item.Sword");O.MinQuantity=1;O.MaxQuantity=1;R.Outputs.Add(O);
    return R;
}
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAetherRecipeBasicTest,"AgeOfAether.Data.Recipe.Basic",EAutomationTestFlags::EditorContext|EAutomationTestFlags::EngineFilter)
bool FAetherRecipeBasicTest::RunTest(const FString&)
{
    FAetherRecipeRegistry R;FString E;TestTrue(TEXT("register"),R.RegisterRecipe(MakeRecipe(TEXT("Recipe.Basic")),E));TestEqual(TEXT("count"),R.Num(),1);
    FAetherRecipeDefinition Out;TestTrue(TEXT("resolve"),R.Resolve(TEXT(" Recipe.Basic "),Out));TestEqual(TEXT("resolved id"),Out.DefinitionID,TEXT("Recipe.Basic"));return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAetherRecipeInvalidTest,"AgeOfAether.Data.Recipe.Invalid",EAutomationTestFlags::EditorContext|EAutomationTestFlags::EngineFilter)
bool FAetherRecipeInvalidTest::RunTest(const FString&)
{
    FAetherRecipeRegistry R;FString E;auto A=MakeRecipe(TEXT("Recipe.Bad"));A.Ingredients[0].Quantity=0;TestFalse(TEXT("zero ingredient"),R.RegisterRecipe(A,E));
    auto B=MakeRecipe(TEXT("Recipe.BadOutput"));B.Outputs[0].MaxQuantity=0;TestFalse(TEXT("invalid output"),R.RegisterRecipe(B,E));
    auto C=MakeRecipe(TEXT("Recipe.BadChance"));C.SuccessChance=1.1;TestFalse(TEXT("chance range"),R.RegisterRecipe(C,E));
    auto D=MakeRecipe(TEXT("Recipe.BadGuarantee"));D.bGuaranteedResult=true;TestFalse(TEXT("guarantee threshold"),R.RegisterRecipe(D,E));
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAetherRecipeFailureTest,"AgeOfAether.Data.Recipe.FailureModes",EAutomationTestFlags::EditorContext|EAutomationTestFlags::EngineFilter)
bool FAetherRecipeFailureTest::RunTest(const FString&)
{
    FAetherRecipeRegistry R;FString E;auto A=MakeRecipe(TEXT("Recipe.Consume"));A.FailureBehavior=EAetherRecipeFailureBehavior::ConsumeInputs;TestTrue(TEXT("consume failure mode"),R.RegisterRecipe(A,E));
    auto B=MakeRecipe(TEXT("Recipe.ProduceFail"));B.FailureBehavior=EAetherRecipeFailureBehavior::ProduceFailureOutputs;TestFalse(TEXT("requires failure output"),R.RegisterRecipe(B,E));
    FAetherRecipeOutput F;F.ItemID=TEXT("Item.Scrap");F.MinQuantity=1;F.MaxQuantity=2;B.FailureOutputs.Add(F);TestTrue(TEXT("failure output"),R.RegisterRecipe(B,E));return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAetherRecipeLimitTest,"AgeOfAether.Data.Recipe.Limits",EAutomationTestFlags::EditorContext|EAutomationTestFlags::EngineFilter)
bool FAetherRecipeLimitTest::RunTest(const FString&)
{
    FAetherRecipeRegistry R;FString E;auto A=MakeRecipe(TEXT("Recipe.Limits"));A.DailyLimit=5;A.WeeklyLimit=10;A.CooldownSeconds=2.5;TestTrue(TEXT("valid limits"),R.RegisterRecipe(A,E));
    auto B=MakeRecipe(TEXT("Recipe.NegLimit"));B.DailyLimit=-1;TestFalse(TEXT("negative daily"),R.RegisterRecipe(B,E));return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAetherRecipeDuplicateTest,"AgeOfAether.Data.Recipe.Duplicate",EAutomationTestFlags::EditorContext|EAutomationTestFlags::EngineFilter)
bool FAetherRecipeDuplicateTest::RunTest(const FString&)
{
    FAetherRecipeRegistry R;FString E;auto A=MakeRecipe(TEXT("Recipe.Shared"));TestTrue(TEXT("first"),R.RegisterRecipe(A,E));TestFalse(TEXT("duplicate"),R.RegisterRecipe(A,E));return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAetherRecipeDeterminismTest,"AgeOfAether.Data.Recipe.Determinism",EAutomationTestFlags::EditorContext|EAutomationTestFlags::EngineFilter)
bool FAetherRecipeDeterminismTest::RunTest(const FString&)
{
    FAetherRecipeRegistry R;FString E;for(const FString& ID:{TEXT("Recipe.Z"),TEXT("Recipe.A"),TEXT("Recipe.M")})TestTrue(TEXT("register"),R.RegisterRecipe(MakeRecipe(ID),E));
    TArray<FString> IDs;R.GetDefinitionIDs(IDs);TestTrue(TEXT("sorted"),IDs.Num()==3&&IDs[0]==TEXT("Recipe.A")&&IDs[1]==TEXT("Recipe.M")&&IDs[2]==TEXT("Recipe.Z"));return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAetherRecipeReferenceTest,"AgeOfAether.Data.Recipe.References",EAutomationTestFlags::EditorContext|EAutomationTestFlags::EngineFilter)
bool FAetherRecipeReferenceTest::RunTest(const FString&)
{
    FAetherRecipeRegistry R;FString E;auto A=MakeRecipe(TEXT("Recipe.Ref"));A.RequiredSkillID=TEXT("Skill.Missing");A.RequiredSkillLevel=1;A.RequiredStationID=TEXT("Station.Missing");FAetherRecipeUnlockCondition Qc; Qc.Type=EAetherRecipeUnlockConditionType::QuestCompleted; Qc.ReferenceID=TEXT("Quest.Missing"); A.UnlockConditions.Add(Qc);TestTrue(TEXT("register structurally"),R.RegisterRecipe(A,E));
    FAetherSkillEffectRegistry SkillRegistry; FAetherQuestDialogueEventRegistry QuestRegistry; FAetherWorldActorRegistry WorldActorRegistry; TArray<FAetherRecipeValidationIssue> Issues; TestFalse(TEXT("validation detects missing refs"),R.Validate(Issues,nullptr,nullptr,nullptr,&SkillRegistry,&QuestRegistry,&WorldActorRegistry)); TestTrue(TEXT("issues"),Issues.Num()>=3); return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAetherRecipeUnlockConditionTest,"AgeOfAether.Data.Recipe.UnlockConditions",EAutomationTestFlags::EditorContext|EAutomationTestFlags::EngineFilter)
bool FAetherRecipeUnlockConditionTest::RunTest(const FString&)
{
    FAetherRecipeRegistry R;FString E;auto A=MakeRecipe(TEXT("Recipe.Unlock"));FAetherRecipeUnlockCondition C;C.Type=EAetherRecipeUnlockConditionType::WorldTag;C.RequiredTag=TEXT("Season.1");A.UnlockConditions.Add(C);TestTrue(TEXT("world tag condition"),R.RegisterRecipe(A,E));
    auto B=MakeRecipe(TEXT("Recipe.BadUnlock"));FAetherRecipeUnlockCondition Bc;Bc.Type=EAetherRecipeUnlockConditionType::WorldTag;A.UnlockConditions.Add(Bc);B.UnlockConditions.Add(Bc);TestFalse(TEXT("missing world tag"),R.RegisterRecipe(B,E));return true;
}
