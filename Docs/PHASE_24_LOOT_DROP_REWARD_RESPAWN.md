# PHASE 24 — LOOT, DROP, REWARD & RESPAWN

## Status
**Repository implementation: COMPLETE.**

## Scope
This phase establishes data-driven definitions and registry validation for loot tables, item drops, rewards and world-actor respawn rules. Runtime random selection, inventory mutation and spawn scheduling remain server-runtime responsibilities.

## Implemented
- LootTableDefinition with weighted, guaranteed, random-count and chance-each selection modes.
- Loot entries with item IDs, quantity ranges, weight, chance and tag requirements.
- RewardDefinition with experience, currencies, direct item rewards and nested loot-table references.
- RespawnDefinition with world actor ID, base respawn time, jitter, maximum alive count, enable state and spawn/world tags.
- Stable global IDs and normalized deterministic registry lookup.
- Cross-validation against Phase 19 Content Registry, Phase 20 Asset Registry boundary, Phase 21 Item Registry and Phase 22 World Actor Registry.
- Structural validation for quantities, probability ranges, level ranges, weighted tables, reward non-emptiness and respawn timing.
- Automation coverage for registration, invalid data, cross references, duplicate IDs and deterministic ordering.

## Authority boundary
The server will own:
- RNG and loot selection;
- duplicate/drop protection;
- reward grant transactions;
- item ownership;
- respawn timers and spawn counts;
- persistence and audit.

The client receives authoritative results and never chooses the reward outcome.

## Determinism
Definitions are enumerated by sorted stable ID. Runtime randomization must use server-authoritative RNG; this phase does not introduce a gameplay RNG implementation.

## Cross-reference model
Loot entries reference Phase 21 item definition IDs.
Respawn definitions reference Phase 22 monster/NPC/boss IDs.
Reward definitions can reference loot tables.
Optional Phase 19 content validation can require the definition ID itself to exist in the generic content registry.

## Unreal validation truth
Repository/static validation and CI are the available validation layers. Unreal 5.8.1 UHT/UBT/Editor/Automation/PIE remain **NOT VERIFIED** because the Unreal toolchain is unavailable in this environment. No compilation success is claimed.

## Non-goals
Live loot rolling, drop spawning, inventory insertion, reward transactions, respawn scheduling, map spawn-point resolution and UI are later runtime integration.
