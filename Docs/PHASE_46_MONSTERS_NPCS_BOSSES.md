# Phase 46 — Monsters, NPCs & Bosses

## Status
Complete at repository/source level. Unreal 5.8.1 runtime acceptance remains a local gate.

## Implemented
- Stable data-driven creature definitions for Monster, NPC and Boss roles.
- Generic AI profile: Passive, Guard, Patrol, Aggressive and Boss.
- Bounded health, attack, defense, movement speed and level values.
- Loot, quest, dialogue, shop and interaction IDs for content integration.
- Optional skeletal mesh, Animation Blueprint, attack montage, VFX and SFX references.
- Stable spawn definitions with zone, transform, max-alive and bounded respawn timing.
- Creature registry with normalized IDs, duplicate protection and reference validation.
- World subsystem capable of spawning/despawning AAetherCreatureActor from a stable spawn ID.
- Per-creature runtime health/state presentation record.
- Data Asset catalog UAetherCreatureCatalog so designers can author creature/spawn content without C++.
- Dedicated Server avoids presentation asset loading.
- No fake Unreal binary assets.

## Low-bureaucracy Unreal workflow
1. Import one real creature skeletal FBX.
2. Create one UAetherCreatureCatalog Data Asset.
3. Add one Monster, one NPC and one Boss definition.
4. Reuse the same AAetherCreatureActor.
5. Assign mesh/animation only where needed.
6. Add spawn rows by data; no new C++ class per creature.
7. Duplicate rows for additional creatures and swap IDs/assets.

### Visual minimum
Only one real creature asset is required to prove the pipeline. The same actor and presentation path can be reused for every monster/NPC/boss.

## Authority boundary
This phase establishes creature definition, spawn and presentation foundations. It intentionally does not invent a second combat authority. Existing player combat remains authoritative; full creature combat/AI behavior can consume these stable creature records in subsequent integration.

## Unreal acceptance gate
- Compile Editor/Game/Dedicated Server.
- Create one catalog Data Asset.
- Assign one skeletal FBX and one Animation Blueprint.
- Register one monster/NPC/boss.
- Spawn from the World Subsystem.
- Verify max-alive enforcement.
- Verify Dedicated Server does not load presentation assets.
- Verify 2-client PIE visibility/replication.
- Validate registry and catalog errors.
