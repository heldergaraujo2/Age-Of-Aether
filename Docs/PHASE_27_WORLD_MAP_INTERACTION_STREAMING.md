# PHASE 27 — WORLD, MAP, INTERACTION & STREAMING CONTENT

## STATUS
Repository implementation complete at source/data-contract level.

## OBJECTIVE
Provide a data-driven authoring contract for maps, regions/zones, streaming cells, world points, map connections, actor placements and interactions without requiring C++ changes for ordinary content additions.

## IMPLEMENTADO
- `EAetherMapType`: open world, city, dungeon, instance and event maps.
- `EAetherCombatRule`: peaceful, PvP, PvP with penalty and no-combat rules.
- `FAetherWorldRequirement` and `FAetherWorldOutcome` for reusable interaction/portal requirements and outcomes.
- `FAetherStreamingCellDefinition` with stable IDs, grid coordinates, bounds, load radius, tags and asset references.
- `FAetherWorldZoneDefinition` with bounds, level limits, combat policy, safe-zone state and tags.
- `FAetherWorldPointDefinition` for spawn, portal, teleport, checkpoint, waypoint and interaction points.
- `FAetherMapConnectionDefinition` for directed/bidirectional map graph edges.
- `FAetherWorldActorPlacementDefinition` for data-driven placement of monsters/NPCs/bosses and spawn groups.
- `FAetherInteractionDefinition` for actor/point interactions, requirements and outcomes.
- `FAetherMapDefinition` as the complete map authoring contract.
- `FAetherWorldContentRegistry` with normalized IDs, duplicate detection and deterministic map enumeration.
- Map-bound validation, zone/point/cell containment checks and streaming-cell coordinate uniqueness.
- Target-map/target-point validation for connections and teleports.
- Graph cycles are intentionally allowed; dangling graph references are rejected.
- Cross-registry validation for assets, items, world actors, loot/spawn groups, quests/events/dialogues, recipes, generic content IDs and skills.
- Safe-zone and combat-policy metadata remains data-driven and server-authoritative at the content boundary.
- No fake `.umap`, World Partition or editor-generated Unreal assets were fabricated.

## DATA/ENGINE BOUNDARY
C++ owns validation and authoritative interpretation. Map definitions are content data. Unreal Editor remains responsible for real map packages, World Partition/Data Layers, terrain, collision, NavMesh, HLOD and imported visual assets.

Stable content IDs are used instead of exposing Unreal package paths to gameplay/network protocols.

## TESTS
Automation coverage added in:
`Source/AgeOfAether/Private/Tests/AetherWorldContentRegistryTests.cpp`

Coverage includes:
1. registration, normalized resolution and counts;
2. invalid map bounds/levels/streaming configuration;
3. duplicate map and nested content IDs;
4. oversized streaming cells;
5. points outside map bounds;
6. bidirectional/cyclic map connections;
7. interaction target/radius and requirement/outcome structure;
8. missing actor/item/spawn/quest/recipe/point references;
9. valid and invalid teleport targets;
10. deterministic sorted map IDs.

## VALIDATION TRUTH
- Repository/static source validation: performed after implementation.
- Unreal 5.8.1 UHT: NOT VERIFIED in this environment.
- Unreal 5.8.1 UBT compilation: NOT VERIFIED in this environment.
- Unreal Editor/PIE: NOT VERIFIED.
- Unreal Automation Framework execution: NOT VERIFIED.
- Dedicated-server runtime/network integration: NOT VERIFIED.
- Real World Partition/Data Layer authoring: NOT VERIFIED because it requires Unreal Editor/content assets.

No Unreal compilation or runtime success is claimed from repository-only checks.

## KNOWN SCOPE LIMIT
This phase creates the authoritative content schema and registry. Actual `.umap` packages, World Partition cells, Data Layers, terrain, NavMesh, HLOD and final visual placement are intentionally deferred to the Unreal client/content phases.

## NEXT
**Phase 28 — Client Core Architecture.**
