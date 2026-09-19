#include "Misc/AutomationTest.h"
#include "Data/AetherQuestDialogueEventRegistry.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAetherQuestDialogueEventBasicTest,"AgeOfAether.Data.QuestDialogueEvent.Basic",EAutomationTestFlags::EditorContext|EAutomationTestFlags::EngineFilter)
bool FAetherQuestDialogueEventBasicTest::RunTest(const FString&)
{
 FAetherQuestDialogueEventRegistry R; FString E;
 FAetherQuestDefinition Q;Q.DefinitionID=TEXT("Quest.Intro");Q.DisplayName=TEXT("Intro");FAetherQuestObjectiveDefinition O;O.ObjectiveID=TEXT("Talk");O.Type=EAetherQuestObjectiveType::Talk;O.TargetID=TEXT("NPC.Guide");Q.Objectives.Add(O);
 FAetherDialogueDefinition D;D.DefinitionID=TEXT("Dialogue.Intro");D.DisplayName=TEXT("Guide");D.StartNodeID=TEXT("N1");FAetherDialogueNodeDefinition N;N.NodeID=TEXT("N1");N.Type=EAetherDialogueNodeType::End;D.Nodes.Add(N);
 FAetherWorldEventDefinition Ev;Ev.DefinitionID=TEXT("Event.Invasion");Ev.DisplayName=TEXT("Invasion");Ev.DurationSeconds=60.0;
 FAetherWorldContentDefinition W;W.DefinitionID=TEXT("World.Village");W.DisplayName=TEXT("Village");W.ZoneTag=TEXT("Village");
 TestTrue(TEXT("quest"),R.RegisterQuest(Q,E));TestTrue(TEXT("dialogue"),R.RegisterDialogue(D,E));TestTrue(TEXT("event"),R.RegisterWorldEvent(Ev,E));TestTrue(TEXT("world"),R.RegisterWorldContent(W,E));TestEqual(TEXT("counts"),R.NumQuests()+R.NumDialogues()+R.NumWorldEvents()+R.NumWorldContents(),4);return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAetherQuestDialogueEventInvalidTest,"AgeOfAether.Data.QuestDialogueEvent.Invalid",EAutomationTestFlags::EditorContext|EAutomationTestFlags::EngineFilter)
bool FAetherQuestDialogueEventInvalidTest::RunTest(const FString&)
{
 FAetherQuestDialogueEventRegistry R;FString E;
 FAetherQuestDefinition Q;Q.DefinitionID=TEXT("Quest.Bad");Q.DisplayName=TEXT("Bad");TestFalse(TEXT("quest requires objective"),R.RegisterQuest(Q,E));
 FAetherDialogueDefinition D;D.DefinitionID=TEXT("Dialogue.Bad");D.DisplayName=TEXT("Bad");D.StartNodeID=TEXT("Missing");FAetherDialogueNodeDefinition N;N.NodeID=TEXT("N1");N.Type=EAetherDialogueNodeType::End;D.Nodes.Add(N);TestFalse(TEXT("dialogue missing start"),R.RegisterDialogue(D,E));
 FAetherWorldEventDefinition Ev;Ev.DefinitionID=TEXT("Event.Bad");Ev.DisplayName=TEXT("Bad");Ev.DurationSeconds=-1;TestFalse(TEXT("negative duration"),R.RegisterWorldEvent(Ev,E));
 return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAetherQuestDialogueEventGraphTest,"AgeOfAether.Data.QuestDialogueEvent.DialogueGraph",EAutomationTestFlags::EditorContext|EAutomationTestFlags::EngineFilter)
bool FAetherQuestDialogueEventGraphTest::RunTest(const FString&)
{
 FAetherQuestDialogueEventRegistry R;FString E;FAetherDialogueDefinition D;D.DefinitionID=TEXT("Dialogue.Graph");D.DisplayName=TEXT("Graph");D.StartNodeID=TEXT("N1");
 FAetherDialogueNodeDefinition N1;N1.NodeID=TEXT("N1");N1.Type=EAetherDialogueNodeType::Line;N1.Text=FText::FromString(TEXT("Hello"));N1.NextNodeID=TEXT("N2");
 FAetherDialogueNodeDefinition N2;N2.NodeID=TEXT("N2");N2.Type=EAetherDialogueNodeType::End;D.Nodes={N1,N2};TestTrue(TEXT("valid graph"),R.RegisterDialogue(D,E));
 FAetherDialogueDefinition Bad=D;Bad.DefinitionID=TEXT("Dialogue.Dangling");Bad.Nodes[0].NextNodeID=TEXT("Missing");TestFalse(TEXT("dangling edge"),R.RegisterDialogue(Bad,E));return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAetherQuestDialogueEventDuplicateTest,"AgeOfAether.Data.QuestDialogueEvent.Duplicate",EAutomationTestFlags::EditorContext|EAutomationTestFlags::EngineFilter)
bool FAetherQuestDialogueEventDuplicateTest::RunTest(const FString&)
{
 FAetherQuestDialogueEventRegistry R;FString E;FAetherWorldEventDefinition A;A.DefinitionID=TEXT("Shared");A.DisplayName=TEXT("A");A.DurationSeconds=1;TestTrue(TEXT("first"),R.RegisterWorldEvent(A,E));FAetherQuestDefinition Q;Q.DefinitionID=TEXT("Shared");Q.DisplayName=TEXT("Q");FAetherQuestObjectiveDefinition O;O.ObjectiveID=TEXT("O");Q.Objectives.Add(O);TestFalse(TEXT("cross type duplicate"),R.RegisterQuest(Q,E));return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAetherQuestDialogueEventDeterminismTest,"AgeOfAether.Data.QuestDialogueEvent.Determinism",EAutomationTestFlags::EditorContext|EAutomationTestFlags::EngineFilter)
bool FAetherQuestDialogueEventDeterminismTest::RunTest(const FString&)
{
 FAetherQuestDialogueEventRegistry R;FString E;for(const FString&ID:{TEXT("Z"),TEXT("A"),TEXT("M")}){FAetherWorldEventDefinition X;X.DefinitionID=ID;X.DisplayName=ID;X.DurationSeconds=1;R.RegisterWorldEvent(X,E);}TArray<FString>IDs;R.GetDefinitionIDs(IDs);TestEqual(TEXT("count"),IDs.Num(),3);TestTrue(TEXT("sorted"),IDs[0]==TEXT("A")&&IDs[1]==TEXT("M")&&IDs[2]==TEXT("Z"));return true;
}
