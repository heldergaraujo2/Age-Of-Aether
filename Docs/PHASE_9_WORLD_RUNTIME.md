# AGE OF AETHER — PHASE 9: WORLD RUNTIME

## Status

**Repository implementation:** COMPLETE  
**Repository/static validation:** PENDING FINAL AUDIT  
**Unreal 5.8.1 compile/runtime validation:** NOT VERIFIED in this environment  
**Persistence:** intentionally deferred

Phase 9 establishes the authoritative world model that later systems consume for movement, PvP, events, quests, NPCs, AI and dedicated-server world management.

## Architecture

The runtime boundary is:

Client request → Server authentication/character authority → World validation → Portal/zone rules → Spawn resolution → CharacterService commit → PlayerState replication → Pawn teleport → World revision

The world is represented by data contracts rather than hard-coded level logic:

- FAetherWorldZone
- FAetherWorldSpawnPoint
- FAetherWorldPortal
- FAetherWorldConfig
- UAetherWorldConfigDataAsset

The runtime service is:

- FAetherWorldService
- UAetherWorldSubsystem

Unreal's World Partition is the intended large-world technology for the eventual Unreal content layer. Epic documents it as a distance-based streaming/data-management system using grid cells, streaming sources and Data Layers. Level Streaming remains available for specific cases, but World Composition is legacy and World Partition is recommended for UE5 projects. citeturn0search0turn0search2

## World rules

### Zone types

Implemented:

- Safe
- PvP
- Event
- Dungeon
- City

Zone data contains:

- stable ZoneID;
- display name;
- enabled/disabled state;
- minimum level;
- combat permission;
- recommended level.

The current runtime intentionally does not make bAllowCombat directly mutate the combat service. It is exposed as authoritative world policy so Phase 8 combat and later combat-mode rules can consume it without coupling the world service to damage calculation.

### Spawn points

Spawn points contain:

- stable SpawnID;
- owning ZoneID;
- location;
- rotation;
- enabled state;
- minimum level.

Selection is deterministic by SpawnID when multiple eligible spawns exist.

### Portals

Portals contain:

- stable PortalID;
- source ZoneID;
- target ZoneID;
- enabled state;
- minimum level.

A transition is only accepted when:

1. source zone exists and is enabled;
2. target zone exists;
3. target zone is enabled;
4. character level satisfies target zone requirement;
5. a matching enabled portal exists;
6. character level satisfies portal requirement;
7. an eligible target spawn exists.

The client cannot teleport by submitting a location or rotation. It submits only the target ZoneID; the server resolves the portal and spawn.

## Character integration

FAetherCharacterRecord now carries:

- WorldLocation
- WorldRotation
- CurrentZoneId
- CurrentZoneType

FAetherCharacterService::UpdateCharacterWorldState is the authoritative commit boundary.

World transitions therefore do not write directly to arbitrary client-controlled state.

## Replication

AAetherCharacterPlayerState now replicates:

- CurrentZoneId
- CurrentZoneType

On accepted transition:

1. WorldService changes the authoritative character record.
2. CharacterService commits the world state.
3. PlayerState receives the authoritative record.
4. Pawn is moved server-side using teleport physics.
5. GameState authoritative revision advances.

GameMode also attempts authoritative spawn initialization when a selected character is spawned.

## Data-driven configuration

UAetherWorldConfigDataAsset contains:

- runtime limits;
- zone definitions;
- spawn definitions;
- portal definitions.

UAetherWorldSubsystem::ConfigureFromDataAsset loads the configuration and definitions as one operation. Invalid definitions abort the load rather than leaving a partially accepted configuration.

The intended production workflow is:

World Data Asset → World Subsystem → Server Authority → Character State → PlayerState/Pawn

No map content is faked in C++. Unreal map/World Partition assets belong to the later Unreal handoff/content stage.

## Unreal large-world integration

The project should use World Partition rather than introducing a new World Composition architecture. Epic documents World Partition as a persistent level divided into runtime grid cells that stream based on streaming sources; Player Controllers can act as streaming sources. citeturn0search0

The World Runtime C++ layer intentionally does not attempt to manufacture .umap assets without an Unreal editor environment.

Navigation is deliberately not hard-coded into this phase. Unreal's Navigation System generates navigable space from collision geometry through Navigation Mesh data, and Navigation Mesh Bounds Volumes define the area to generate. citeturn0search1turn0search8

AI navigation will consume this world layer in a later phase.

## Automated tests

Added Unreal Automation Framework coverage for:

1. invalid/valid world configuration;
2. zone registration;
3. spawn registration;
4. portal registration;
5. duplicate portal rejection;
6. deterministic spawn selection;
7. target level requirement;
8. missing portal rejection;
9. valid portal transition;
10. disabled-zone rejection;
11. authoritative initial character spawn;
12. authoritative character transition;
13. invalid/offline character rejection.

These tests were statically audited but cannot be executed in this environment without Unreal Engine 5.8.1.

## Local acceptance procedure

When Unreal 5.8.1 is available:

1. Generate/update Visual Studio project files.
2. Run UHT.
3. Build Development Editor.
4. Open the project.
5. Create a World Config Data Asset.
6. Define zones.
7. Define spawn points.
8. Define directed portals.
9. Configure the World Subsystem from the Data Asset.
10. Authenticate an account.
11. Create/select a character.
12. Verify authoritative initial spawn.
13. Verify CurrentZoneId/CurrentZoneType replication.
14. Request a valid portal transition.
15. Verify server-side location/rotation change.
16. Verify GameState revision advances.
17. Test insufficient level.
18. Test disabled zone.
19. Test missing portal.
20. Test invalid spawn.
21. Enable network latency/loss/jitter emulation.
22. Verify the client cannot submit arbitrary coordinates to teleport.
23. Run AgeOfAether.World.* automation tests.
24. Run the full Automation Framework suite.

## Security boundary

The client cannot authoritatively set:

- ZoneID;
- world location;
- world rotation;
- portal;
- spawn point;
- minimum-level bypass;
- zone enabled state;
- portal enabled state.

The client may request a target zone. The server decides whether that transition is legal and resolves the resulting spawn.

## Explicitly deferred

- actual .umap creation;
- World Partition grid authoring;
- Data Layers;
- HLOD;
- terrain/landscape;
- collision/NavMesh authoring;
- movement prediction/reconciliation;
- anti-teleport movement validation;
- dynamic streaming source management;
- NPC spawn runtime;
- monster spawn/runtime;
- AI;
- quests;
- event scheduling;
- zone population;
- respawn;
- world persistence;
- cross-server transfer.

These are later consumers of the Phase 9 contracts.

## Documentation references

- Unreal Engine 5.8 World Partition documentation.
- Unreal Engine 5.8 Level Streaming documentation.
- Unreal Engine 5.8 Navigation documentation.
- Unreal Engine 5.8 GameMode/GameState documentation.
