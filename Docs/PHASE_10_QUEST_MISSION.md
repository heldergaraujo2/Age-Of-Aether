# PHASE 10 — QUEST / MISSION

## Status

Repository implementation: **COMPLETE**.  
Repository/static validation: **PASSED**.  
Unreal 5.8.1 compilation/runtime validation: **NOT VERIFIED** because the Unreal environment is not available in this execution context and no CI build exists.

## Architecture

Quest definitions are immutable content data. Runtime quest state belongs to the character. Critical mutations are executed through the authoritative server-side quest service.

Unreal's Data Asset model is appropriate for rich per-quest definitions, while Data Registries remain an optional future content-discovery layer. Epic documents Data Assets as assets that store system-specific data and Data Registries as efficient read-oriented stores for USTRUCT data. citeturn0search3turn0search1

The existing Gameplay Framework remains the network boundary: PlayerController sends requests, GameMode/GameState define server rules/state, and PlayerState carries replicated player state. citeturn0search0turn0search6

## Implemented

### Quest contracts
- `FAetherQuestId`
- `FAetherQuestObjectiveDefinition`
- `FAetherQuestObjectiveProgress`
- `FAetherQuestReward`
- `FAetherQuestDefinition`
- `FAetherQuestState`
- `FAetherQuestConfig`
- `FAetherQuestOperation`

### Lifecycle
- Locked
- Available
- Active
- Completed
- Failed
- Abandoned

The current server workflow implements Available → Active → Abandoned/Completed. Failed/Cooldown-style rules remain extensible without changing the core state container.

### Objectives
- Kill
- Collect
- ReachZone
- Interact
- Talk
- ReachLevel
- UseItem

Progress can be advanced by objective ID or by objective type + authoritative target ID. The client has no RPC that directly grants objective progress.

Optional objectives do not block completion. Required objectives must be complete before completion is accepted.

### Requirements
- minimum character level;
- prerequisite quests;
- enabled/disabled definition state;
- active-quest capacity;
- active character lifecycle;
- authenticated ownership at the networking boundary.

### Rewards
Completion returns an authoritative reward contract containing:
- experience amount;
- item definition IDs;
- item quantities.

The quest state records `bRewardGranted` so the same quest cannot be completed a second time and granted twice.

Actual durable persistence is intentionally deferred to Phase 14. Economy/currency and broader reward transaction infrastructure remain later work.

## Networking

`AAetherNetworkPlayerController` now exposes:
- `RequestQuestList`;
- `AcceptQuest`;
- `AbandonQuest`;
- `CompleteQuest`.

All four use dedicated request IDs and reliable server RPCs.

The server derives:
- AccountID from authenticated session state;
- CharacterID from the active Character PlayerState.

The client only supplies a QuestID for mutation requests. It cannot select another character as the quest owner.

Quest list and operation responses are delivered through Blueprint-assignable delegates.

## Server-driven integration points

Future authoritative systems can call:
- `AdvanceObjective(Character, QuestID, ObjectiveID, Amount)`;
- `AdvanceObjectiveByTarget(Character, ObjectiveType, TargetID, Amount)`.

This is deliberate.

Examples:
- Combat can advance Kill objectives after an authoritative kill.
- Inventory can advance Collect/UseItem objectives after a successful server-side operation.
- World can advance ReachZone objectives after an accepted authoritative transition.
- Interaction/NPC systems can advance Interact/Talk objectives after server validation.
- Progression can satisfy ReachLevel objectives from authoritative character level.

No client-side event is trusted as proof.

## Files

Added:
- `Source/AgeOfAether/Public/Quests/AetherQuestTypes.h`
- `Source/AgeOfAether/Public/Quests/AetherQuestService.h`
- `Source/AgeOfAether/Private/Quests/AetherQuestService.cpp`
- `Source/AgeOfAether/Public/Quests/AetherQuestSubsystem.h`
- `Source/AgeOfAether/Private/Quests/AetherQuestSubsystem.cpp`
- `Source/AgeOfAether/Public/Quests/AetherQuestDefinitionDataAsset.h`
- `Source/AgeOfAether/Private/Tests/AetherQuestTests.cpp`

Modified:
- `Source/AgeOfAether/Public/Networking/AetherNetworkPlayerController.h`
- `Source/AgeOfAether/Private/Networking/AetherNetworkPlayerController.cpp`
- `ROADMAP.md`
- `PROJECT_MEMORY/00_CONTINUITY.md`

## Tests

Automation coverage added for:
1. definition validation;
2. duplicate definition rejection;
3. accept/active lifecycle;
4. duplicate active rejection;
5. abandon and reaccept;
6. objective progression and clamping;
7. completion;
8. duplicate completion rejection;
9. prerequisite enforcement;
10. invalid/offline character rejection;
11. negative progress rejection;
12. active quest capacity;
13. optional objective completion.

## Validation

Repository-side validation performed:
- all new quest source was reviewed for balanced C++ delimiters;
- declarations and implementations were reconciled;
- all referenced quest RPCs exist in both header and implementation;
- quest subsystem delegates to the service;
- controller includes the quest subsystem and quest types;
- no client-supplied CharacterID/AccountID is accepted by quest mutation RPCs;
- no fake `.umap`, World Partition, or other editor-generated asset was created;
- roadmap and continuity were updated.

### Unreal gate

Not executable here:
- UHT;
- UBT/C++ compilation against Unreal 5.8.1;
- Editor startup;
- PIE;
- multiplayer replication runtime;
- Automation Framework execution;
- network emulation.

Therefore this phase is **repository-complete**, not falsely marked as Unreal-runtime-complete.

## Next phase

**Phase 11 — Social**

Expected scope:
- party;
- friends;
- guild foundation;
- chat;
- social permissions;
- server-authoritative membership and invitation flows;
- automation/security tests.
