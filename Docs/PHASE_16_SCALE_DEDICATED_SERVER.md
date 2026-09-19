# PHASE 16 — MMORPG SCALE & DEDICATED SERVER

## Status

**Repository implementation:** COMPLETE
**Static/source validation:** PASS
**Unreal 5.8.1 UHT/UBT/Editor/Automation/dedicated-server runtime:** NOT VERIFIED in this environment

## Objective

Prepare AGE OF AETHER for large-scale authoritative multiplayer and dedicated-server deployment without inventing cloud infrastructure or claiming runtime validation that was not performed.

Epic documents dedicated servers as headless server targets with no local player and identifies them as appropriate for large-scale multiplayer. Unreal Build Tool exposes a dedicated Server target through a Server.Target.cs file. Epic also documents Replication Graph as a replication system designed to scale to large numbers of players and replicated Actors. World Partition remains the intended large-world content system.

## Implemented

### Dedicated-server foundation

Added or restored the Unreal project foundation required by this phase:

- AgeOfAether.uproject;
- AgeOfAether.Build.cs;
- Game and Editor targets;
- AgeOfAetherServer.Target.cs using TargetType.Server;
- module bootstrap files;
- DefaultEngine.ini network baseline.

The Server target is a real UBT target. It does not fabricate an executable; Unreal Build Tool must build it locally.

### Scale runtime boundary

Added:

- FAetherScaleConfig;
- FAetherServerNodeId;
- FAetherServerNodeDescriptor;
- FAetherServerLoadSnapshot;
- FAetherServerTransferRequest;
- FAetherScaleService;
- UAetherScaleSubsystem;
- UAetherScaleConfigDataAsset.

The scale service provides:

- server lifecycle;
- accepting/draining states;
- player admission capacity;
- node health heartbeat;
- load snapshots;
- connection/tick/replication load score;
- automatic drain at hard load;
- transfer contracts with expiration;
- server-node identity;
- data-driven scale limits.

### Admission hierarchy

Phase 13 remains the connection-level rate/capacity authority.

Phase 16 adds the node-level layer above it:

Connection -> Multiplayer admission -> Scale/node admission -> Gameplay authority

A node can enter Draining so new connections are rejected while existing players can be migrated or disconnected gracefully.

### Replication strategy

The repository intentionally does not hard-wire a custom Replication Graph implementation before Unreal validation. Epic documents Replication Graph as a scalable replication system and supports a replication-driver configuration boundary. The project therefore keeps the scale architecture independent from an unverified engine plugin API.

When the local Unreal toolchain is available, the concrete replication driver should be validated against the installed 5.8.1 engine before enabling it for production.

### World-scale strategy

World Partition remains the intended large-world streaming system. Its grid cells, streaming sources, Data Layers and HLOD integration are content/editor responsibilities and are not fabricated as text .umap or .uasset files.

### Cross-server transfer boundary

The phase defines a server-to-server transfer contract but does not pretend that a gateway or distributed transport exists.

The intended production sequence is:

1. source creates transfer intent;
2. persistent state is saved and locked;
3. destination validates the intent;
4. destination restores authoritative state;
5. source releases the session;
6. expired transfers are rejected safely.

No client is allowed to author a transfer destination state.

## Tests

Added AetherScaleTests.cpp covering:

1. configuration validation;
2. player admission/capacity;
3. drain mode;
4. heartbeat health;
5. hard-load drain;
6. transfer creation and expiration;
7. invalid same-node transfer.

Static audits performed:

- delimiter balance on new C++ files;
- no literal escaped-newline artifacts;
- no fake Unreal content assets;
- target file/class naming consistency;
- Server target uses TargetType.Server;
- transfer authority remains server-side;
- tests cover every public scale behavior.

## Local Unreal acceptance gate

This environment has no Unreal Engine 5.8.1 executable/toolchain, so these cannot truthfully be marked PASS:

- UHT;
- UBT Development Game/Editor/Server;
- Automation Framework execution;
- server cook;
- dedicated-server launch;
- multi-client connection;
- network saturation/load test;
- Replication Graph runtime;
- World Partition runtime.

Epic's documented dedicated-server workflow requires local build, cook, launch and client-connection testing.

## Acceptance

### Repository

- dedicated-server target: PASS by source inspection
- Unreal module/project foundation: restored and PASS by source inspection
- server-node scale contract: PASS
- admission/drain lifecycle: PASS
- health heartbeat: PASS
- load accounting: PASS
- transfer contract: PASS
- automation tests added: PASS by source inspection
- no fake production infrastructure: PASS

### Unreal

- UHT: NOT VERIFIED
- UBT Game/Editor/Server: NOT VERIFIED
- Automation Framework: NOT VERIFIED
- dedicated-server cook/launch: NOT VERIFIED
- Replication Graph: NOT VERIFIED
- multi-client load: NOT VERIFIED
- World Partition: NOT VERIFIED

## Next phase

**PHASE 17 — AI/GPT INTEGRATION**
