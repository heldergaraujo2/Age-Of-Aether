#include "Misc/AutomationTest.h"
#include "Quests/AetherQuestService.h"

namespace
{
    FAetherCharacterRecord MakeQuestCharacter()
    {
        FAetherCharacterRecord Character;
        Character.CharacterId = FAetherCharacterId::NewId();
        Character.AccountId = FAetherAccountId::NewId();
        Character.Name = TEXT("QuestTester");
        Character.Status = EAetherCharacterStatus::Active;
        Character.Level = 10;
        return Character;
    }

    FAetherQuestDefinition MakeQuest(const FString& QuestId)
    {
        FAetherQuestDefinition Definition;
        Definition.QuestId = FAetherQuestId::FromString(QuestId);
        Definition.DisplayName = TEXT("Test Quest");
        Definition.Description = TEXT("Test");
        Definition.MinimumLevel = 5;

        FAetherQuestObjectiveDefinition Objective;
        Objective.ObjectiveId = TEXT("kill_01");
        Objective.Type = EAetherQuestObjectiveType::Kill;
        Objective.TargetId = TEXT("wolf");
        Objective.RequiredCount = 3;
        Definition.Objectives.Add(Objective);
        return Definition;
    }
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAetherQuestDefinitionValidationTest, "AgeOfAether.Quests.DefinitionValidation", EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter)
bool FAetherQuestDefinitionValidationTest::RunTest(const FString&)
{
    FAetherQuestDefinition Definition = MakeQuest(TEXT("quest.test"));
    TestTrue(TEXT("valid definition"), Definition.IsValid());

    Definition.Objectives[0].RequiredCount = 0;
    TestFalse(TEXT("zero objective count rejected"), Definition.IsValid());
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAetherQuestRegistrationTest, "AgeOfAether.Quests.Registration", EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter)
bool FAetherQuestRegistrationTest::RunTest(const FString&)
{
    FAetherQuestService Service;
    const FAetherQuestDefinition Definition = MakeQuest(TEXT("quest.register"));
    TestTrue(TEXT("register valid definition"), Service.RegisterDefinition(Definition));
    TestFalse(TEXT("duplicate rejected"), Service.RegisterDefinition(Definition));
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAetherQuestLifecycleTest, "AgeOfAether.Quests.Lifecycle", EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter)
bool FAetherQuestLifecycleTest::RunTest(const FString&)
{
    FAetherQuestService Service;
    const FAetherCharacterRecord Character = MakeQuestCharacter();
    const FAetherQuestDefinition Definition = MakeQuest(TEXT("quest.lifecycle"));
    TestTrue(TEXT("register"), Service.RegisterDefinition(Definition));

    FAetherQuestOperation Operation;
    TestTrue(TEXT("accept"), Service.AcceptQuest(Character, Definition.QuestId, Operation));
    TestEqual(TEXT("active"), Operation.State.Status, EAetherQuestStatus::Active);

    TestFalse(TEXT("duplicate active rejected"), Service.AcceptQuest(Character, Definition.QuestId, Operation));
    TestEqual(TEXT("already active"), Operation.Result, EAetherQuestOperationResult::AlreadyActive);

    TestTrue(TEXT("abandon"), Service.AbandonQuest(Character, Definition.QuestId, Operation));
    TestEqual(TEXT("abandoned"), Operation.State.Status, EAetherQuestStatus::Abandoned);

    TestTrue(TEXT("reaccept abandoned"), Service.AcceptQuest(Character, Definition.QuestId, Operation));
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAetherQuestProgressionTest, "AgeOfAether.Quests.ObjectiveProgression", EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter)
bool FAetherQuestProgressionTest::RunTest(const FString&)
{
    FAetherQuestService Service;
    const FAetherCharacterRecord Character = MakeQuestCharacter();
    const FAetherQuestDefinition Definition = MakeQuest(TEXT("quest.progress"));
    TestTrue(TEXT("register"), Service.RegisterDefinition(Definition));

    FAetherQuestOperation Operation;
    TestTrue(TEXT("accept"), Service.AcceptQuest(Character, Definition.QuestId, Operation));
    TestTrue(TEXT("advance one"), Service.AdvanceObjective(Character, Definition.QuestId, TEXT("kill_01"), 1, Operation));
    TestEqual(TEXT("one progress"), Operation.State.Objectives[0].CurrentCount, 1);
    TestTrue(TEXT("advance two"), Service.AdvanceObjective(Character, Definition.QuestId, TEXT("kill_01"), 2, Operation));
    TestTrue(TEXT("completed objective"), Operation.State.Objectives[0].bCompleted);
    TestTrue(TEXT("complete quest"), Service.CompleteQuest(Character, Definition.QuestId, Operation));
    TestTrue(TEXT("reward marked"), Operation.State.bRewardGranted);
    TestFalse(TEXT("second completion rejected"), Service.CompleteQuest(Character, Definition.QuestId, Operation));
    TestEqual(TEXT("already completed"), Operation.Result, EAetherQuestOperationResult::AlreadyCompleted);
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAetherQuestPrerequisiteTest, "AgeOfAether.Quests.Prerequisites", EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter)
bool FAetherQuestPrerequisiteTest::RunTest(const FString&)
{
    FAetherQuestService Service;
    const FAetherCharacterRecord Character = MakeQuestCharacter();

    FAetherQuestDefinition First = MakeQuest(TEXT("quest.first"));
    FAetherQuestDefinition Second = MakeQuest(TEXT("quest.second"));
    Second.PrerequisiteQuestIds.Add(First.QuestId);
    TestTrue(TEXT("register first"), Service.RegisterDefinition(First));
    TestTrue(TEXT("register second"), Service.RegisterDefinition(Second));

    FAetherQuestOperation Operation;
    TestFalse(TEXT("prerequisite blocks"), Service.AcceptQuest(Character, Second.QuestId, Operation));
    TestEqual(TEXT("requirements result"), Operation.Result, EAetherQuestOperationResult::RequirementsNotMet);

    TestTrue(TEXT("accept first"), Service.AcceptQuest(Character, First.QuestId, Operation));
    TestTrue(TEXT("finish first objective"), Service.AdvanceObjective(Character, First.QuestId, TEXT("kill_01"), 3, Operation));
    TestTrue(TEXT("complete first"), Service.CompleteQuest(Character, First.QuestId, Operation));
    TestTrue(TEXT("prerequisite now satisfied"), Service.AcceptQuest(Character, Second.QuestId, Operation));
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAetherQuestSecurityTest, "AgeOfAether.Quests.Security", EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter)
bool FAetherQuestSecurityTest::RunTest(const FString&)
{
    FAetherQuestService Service;
    const FAetherCharacterRecord Character = MakeQuestCharacter();
    FAetherQuestDefinition Definition = MakeQuest(TEXT("quest.security"));
    TestTrue(TEXT("register"), Service.RegisterDefinition(Definition));

    FAetherQuestOperation Operation;
    TestFalse(TEXT("inactive objective cannot progress"), Service.AdvanceObjective(Character, Definition.QuestId, TEXT("kill_01"), 1, Operation));
    TestEqual(TEXT("not active"), Operation.Result, EAetherQuestOperationResult::NotActive);

    FAetherCharacterRecord Offline = Character;
    Offline.Status = EAetherCharacterStatus::Offline;
    TestFalse(TEXT("offline character rejected"), Service.AcceptQuest(Offline, Definition.QuestId, Operation));
    TestEqual(TEXT("invalid character"), Operation.Result, EAetherQuestOperationResult::CharacterInvalid);

    TestTrue(TEXT("accept"), Service.AcceptQuest(Character, Definition.QuestId, Operation));
    TestFalse(TEXT("negative progress rejected"), Service.AdvanceObjective(Character, Definition.QuestId, TEXT("kill_01"), -1, Operation));
    TestEqual(TEXT("invalid request"), Operation.Result, EAetherQuestOperationResult::InvalidRequest);
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAetherQuestCapacityTest, "AgeOfAether.Quests.Capacity", EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter)
bool FAetherQuestCapacityTest::RunTest(const FString&)
{
    FAetherQuestService Service;
    FAetherQuestConfig Config = Service.GetConfig();
    Config.MaxActiveQuestsPerCharacter = 1;
    TestTrue(TEXT("configure capacity"), Service.SetConfig(Config));

    const FAetherCharacterRecord Character = MakeQuestCharacter();
    FAetherQuestDefinition First = MakeQuest(TEXT("quest.capacity.a"));
    FAetherQuestDefinition Second = MakeQuest(TEXT("quest.capacity.b"));
    TestTrue(TEXT("register first"), Service.RegisterDefinition(First));
    TestTrue(TEXT("register second"), Service.RegisterDefinition(Second));

    FAetherQuestOperation Operation;
    TestTrue(TEXT("accept first"), Service.AcceptQuest(Character, First.QuestId, Operation));
    TestFalse(TEXT("second blocked by capacity"), Service.AcceptQuest(Character, Second.QuestId, Operation));
    TestEqual(TEXT("capacity result"), Operation.Result, EAetherQuestOperationResult::CapacityReached);
    return true;
}


IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAetherQuestOptionalObjectiveTest, "AgeOfAether.Quests.OptionalObjective", EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter)
bool FAetherQuestOptionalObjectiveTest::RunTest(const FString&)
{
    FAetherQuestService Service;
    const FAetherCharacterRecord Character = MakeQuestCharacter();
    FAetherQuestDefinition Definition = MakeQuest(TEXT("quest.optional"));
    Definition.Objectives[0].bOptional = true;
    TestTrue(TEXT("register"), Service.RegisterDefinition(Definition));

    FAetherQuestOperation Operation;
    TestTrue(TEXT("accept"), Service.AcceptQuest(Character, Definition.QuestId, Operation));
    TestTrue(TEXT("optional objective can remain incomplete"), Service.CompleteQuest(Character, Definition.QuestId, Operation));
    TestTrue(TEXT("reward marked"), Operation.State.bRewardGranted);
    return true;
}
