# AGE OF AETHER — PHASE 1 — REAL UNREAL FOUNDATION

## Objective

Close the real Unreal Engine foundation gate for the existing Age of Aether architecture before advancing to visual transformation.

This phase is a **runtime gate**. Repository/source preparation can be completed in GitHub, but final acceptance requires execution in the real local Unreal Engine 5.8 environment.

## Non-negotiable acceptance

All of the following must be evidenced in the real project:

- Unreal Editor starts the project without critical errors.
- UHT completes successfully.
- UBT builds Editor/Development successfully.
- Game target builds successfully.
- Dedicated Server target builds successfully.
- A real development map exists and opens.
- The project has an explicit default GameMode.
- PIE launches.
- 2-client PIE launches.
- Dedicated Server starts.
- A real player character spawns.
- Character movement works.
- Basic replication is observable between clients.
- Automation executes without critical infrastructure failure.
- Output Log contains no unresolved critical startup/runtime errors.
- The result is reproducible after a clean Git pull.

## Existing foundation audited

The repository already contains the relevant source foundation:

- `AgeOfAether.uproject` targets Unreal 5.8 and enables Enhanced Input.
- `AgeOfAether.Build.cs` declares Core, CoreUObject, Engine, UMG, InputCore and EnhancedInput.
- Editor and Server target files exist.
- `AAetherCharacter` already contains movement/input/camera/network-facing character foundations.
- `AetherNetworkGameMode` is configured as the global default GameMode in `Config/DefaultEngine.ini`.
- Asset Manager and Aether visual registry configuration already exist.
- World runtime/map registries and streaming abstractions already exist.
- The project already has server-authoritative networking architecture and dedicated-server target support.

## Important correction

No fake `.umap`, `.uasset`, FBX, animation or other binary Unreal content is created by this phase.

The development map must be created and saved by the real Unreal Editor. Repository source/configuration must not pretend that such a map exists before that happens.

## Runtime execution checklist

### A. Synchronize

```powershell
git pull --ff-only origin main
```

### B. Generate/refresh project files

Use the installed Unreal 5.8 editor/toolchain appropriate to the local installation.

### C. Build

Build:

1. AgeOfAetherEditor Win64 Development
2. AgeOfAether Win64 Development
3. AgeOfAetherServer Win64 Development

Record exit code and first critical error if any.

### D. Editor startup

Open the real `AgeOfAether.uproject`.

Record:

- startup success;
- module load result;
- map load result;
- critical Output Log errors.

### E. Development map

In the real Unreal Editor:

1. Create or open the development map.
2. Set a valid PlayerStart.
3. Confirm the configured GameMode is used.
4. Save the map in the real project.
5. Set it as Game Default Map only after it is real and saved.

### F. PIE

Run single-client PIE.

Verify:

- character exists;
- possession occurs;
- movement input works;
- camera is functional;
- no critical errors.

### G. 2-client PIE

Run two clients.

Verify:

- both clients connect;
- both characters spawn;
- movement is replicated;
- server remains authoritative;
- no critical networking errors occur.

### H. Dedicated Server

Launch the real dedicated server.

Connect a client.

Verify:

- server starts;
- client connects;
- player spawns;
- replicated movement works;
- no critical server/client errors occur.

### I. Automation

Run the project's relevant Automation tests after the project is open and after the server/client smoke tests.

Do not mark unrelated historical test failures as resolved without evidence.

## Current known blockers

The latest recorded repository checkpoint reports:

- 280 automation tests total;
- 266 PASS;
- 14 FAIL;
- 0 WARN;
- ClassBalance is the primary known blocker;
- additional failures include BalanceSimulation.Neutral, ClassCombat.PvPSwitch, QuestDialogueEvent.CrossReferences, Multiplayer heartbeat/rate-limit, Quests.Security and Security.Replay.

These numbers are historical evidence and must be refreshed during the real run.

## Phase gate

### SOURCE PREPARATION

**READY**

The repository already contains the source/configuration foundation required to execute this gate.

### REAL UNREAL RUNTIME

**PENDING LOCAL EXECUTION**

This cannot be truthfully marked PASS from GitHub source inspection alone.

## Exit criteria

Phase 1 becomes **COMPLETE** only when the real Unreal evidence is recorded in this document and the continuity file.

After Phase 1 passes, the next transformation phase is:

**PHASE 2 — ISOMETRIC VISUAL DIRECTION**

The first visual work must continue to preserve the existing MMORPG architecture and must ultimately feed the single continuous world / ~40-city jurisdiction model.

## Evidence record

| Gate | Status | Evidence |
|---|---|---|
| UHT | PENDING | Local Unreal run required |
| UBT Editor | PENDING | Local build required |
| UBT Game | PENDING | Local build required |
| UBT Dedicated Server | PENDING | Local build required |
| Editor startup | PENDING | Local Unreal run required |
| Real development map | PENDING | Must be created/saved in Unreal |
| PIE | PENDING | Local Unreal run required |
| 2-client PIE | PENDING | Local Unreal run required |
| Dedicated Server | PENDING | Local Unreal run required |
| Replication | PENDING | Local Unreal run required |
| Automation | PENDING | Local Unreal run required |
| Critical log errors | PENDING | Local Output Log required |

**Current phase state: RUNTIME GATE PENDING.**
