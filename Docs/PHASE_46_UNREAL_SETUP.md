# Phase 46 — Minimal Unreal Setup

## One catalog, one actor

Create a single UAetherCreatureCatalog Data Asset.

Add:
- one Monster definition;
- one NPC definition;
- one Boss definition;
- one spawn row for each.

All three use the same AAetherCreatureActor class.

## Visuals

For the first vertical slice:
- import one skeletal FBX;
- assign its mesh;
- assign one reusable Animation Blueprint;
- optionally assign one attack montage;
- optionally assign spawn/idle VFX and SFX.

Do not create separate C++ classes or character Blueprints for each creature.

## Spawn

Use the UAetherCreatureSubsystem World Subsystem and call SpawnCreature(SpawnID) from a level Blueprint, test actor, or future world-content system.

The subsystem enforces the spawn MaxAlive value.

## Content scaling

After the first creature works, content expansion is data-only:
- duplicate a creature definition;
- change IDs/stats/loot/dialogue/shop references;
- assign another mesh if available;
- add a spawn row.

## Runtime boundary

The repository implementation is complete, but Unreal 5.8.1 runtime cannot be marked verified until it is executed locally.
