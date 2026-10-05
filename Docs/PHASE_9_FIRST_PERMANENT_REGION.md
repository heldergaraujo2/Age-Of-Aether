# AGE OF AETHER — PHASE 9: FIRST PERMANENT REGION

## Status

**SOURCE / REPOSITORY / ARCHITECTURE: COMPLETE**

**UNREAL MATERIALIZATION / RUNTIME VALIDATION: DEFERRED**

This phase defines the first permanent region as a production-ready content contract over the existing Age of Aether world architecture. It is not a disposable graybox and it does not fabricate a `.umap`, `.uasset` or any other Unreal binary.

The region is the first permanent piece of the single continuous world planned for approximately 40 major cities/jurisdictions.

## 1. Design decision

**Current first-region target (updated 2026-10-04):** a bright, stylized 3D isometric storybook diorama with a readable castle, river and bridge, village, cultivated fields, rolling terrain and layered forest. This owner-supplied reference supersedes the original Phase 9 assumption that the region environment would be premium 2D isometric. See `Docs/FIRST_REGION_ISOMETRIC_DIORAMA_RUNTIME_PROTOTYPE.md` for the current source implementation and its validation boundary.

The region must remain a navigable, decomposable world rather than one flattened background image. Build the environment from reusable 3D families for terrain, paths, buildings, vegetation and landmarks. Paper2D sprites/Flipbooks, depth layers, lighting, shadows, particles/VFX and atmospheric effects remain supported as optional character/effect or future content paths; they are not the primary environment representation for this region.

## 2. Permanent-region identity

Canonical content namespace:

- Map: `AOA.Region.FirstPermanent`
- Region tag: `Region.FirstPermanent`
- Settlement tag: `Region.FirstPermanent.Settlement`
- Wilderness tag: `Region.FirstPermanent.Wilderness`
- Dungeon tag: `Region.FirstPermanent.Dungeon`

These are stable content identifiers for the production specification. They are not Unreal package paths.

The final Unreal map/package name remains an editor/content concern and must be created legitimately in Unreal.

## 3. Spatial composition

The canonical experience is:

**arrival → settlement → plaza/services → outskirts → gate → main road → countryside → forest edge → exploration pockets → combat territory → ruins/landmark → dungeon approach → dungeon entrance**

The region is intentionally non-linear.

### 3.1 Settlement nucleus

The settlement is the safe social anchor.

Required composition:

- central plaza;
- civic/focal landmark;
- merchant/service frontage;
- forge/crafting location;
- tavern/social location;
- residential cluster;
- storage/work areas;
- gardens;
- carts and daily-life props;
- secondary alleys;
- courtyards;
- controlled transition toward the outskirts;
- readable settlement gate.

Buildings are arranged around spaces and routes. They are not placed as isolated decorative objects.

### 3.2 Outskirts

The settlement gradually loses urban density:

- smaller structures;
- fences;
- gardens and cultivated land;
- storage/work areas;
- secondary paths;
- vegetation beginning to dominate;
- visual transition toward countryside.

### 3.3 Main road

The main road is the primary exploration axis.

It must:

- remain readable from the isometric camera;
- connect the settlement to the frontier;
- support optional side paths;
- contain environmental storytelling;
- use landmarks to maintain orientation;
- transition naturally into less managed terrain.

### 3.4 Countryside

The first natural band contains:

- fields;
- meadow;
- low vegetation;
- rocks;
- small props;
- resource/readability pockets;
- minor discoveries;
- secondary traversal routes.

The transition from managed settlement to wilderness must be gradual.

### 3.5 Forest edge and wilderness

The wilderness is not an empty terrain expanse.

It must provide:

- multiple traversal routes;
- clearings;
- dense and sparse vegetation layers;
- rocks and natural obstacles;
- optional water features where the final regional composition requires them;
- small landmarks;
- resource/readability pockets;
- hidden or secondary discoveries;
- visual depth through background/midground/foreground layers.

### 3.6 Combat territory

Combat spaces are embedded into exploration rather than becoming artificial arenas.

Each encounter-capable area should provide:

- more than one approach;
- readable player movement space;
- obstacle/occlusion variation;
- sufficient room for melee and ranged gameplay;
- navigation-compatible space;
- visual escalation toward the dangerous frontier.

Existing combat, AI, damage, skills and loot systems remain authoritative.

### 3.7 Ruins / landmark

A recognizable landmark must establish a strong visual memory point before the dungeon.

It should:

- be visible or discoverable from multiple approaches;
- explain environmental history;
- provide an orientation reference;
- create a visual transition from wilderness to dungeon territory;
- remain compatible with future quest/event hooks.

### 3.8 Dungeon approach and entrance

The dungeon is introduced through a progressive change in atmosphere:

**wilderness → isolation → environmental traces → danger → landmark → entrance**

The entrance is a major visual focal point, not a generic doorway.

The actual dungeon gameplay is a later Phase 12 production gate; Phase 9 establishes the permanent overworld approach and its integration point.

## 4. Region zones

The existing `FAetherWorldZoneDefinition` / `FAetherMapDefinition` contract is the authoritative source for zone metadata.

Canonical logical zones:

| Zone ID | Role | Combat |
|---|---|---|
| `AOA.Region.FirstPermanent.Settlement` | settlement and services | Peaceful |
| `AOA.Region.FirstPermanent.Outskirts` | settlement transition | Peaceful |
| `AOA.Region.FirstPermanent.Countryside` | fields/meadow | Peaceful or controlled PvE |
| `AOA.Region.FirstPermanent.ForestEdge` | first wilderness band | PvE |
| `AOA.Region.FirstPermanent.Wilderness` | exploration | PvE |
| `AOA.Region.FirstPermanent.CombatFrontier` | concentrated encounters | PvE |
| `AOA.Region.FirstPermanent.Ruins` | landmark/discovery | PvE or controlled |
| `AOA.Region.FirstPermanent.DungeonApproach` | dungeon transition | PvE |
| `AOA.Region.FirstPermanent.DungeonEntrance` | dungeon integration point | Dungeon |

Exact world-space bounds are deliberately left to the legitimate Unreal authoring pass. The source contract must describe the topology and relationships without pretending that a repository document is a real map package.

## 5. Streaming-cell grammar

The region must be decomposable into streaming cells using the existing `FAetherStreamingCellDefinition` contract.

Logical cell roles:

1. Arrival
2. Settlement-West
3. Settlement-Center
4. Settlement-East
5. Outskirts
6. Main-Road
7. Countryside
8. Forest-Edge
9. Wilderness-North
10. Wilderness-South
11. Combat-Frontier
12. Ruins-Dungeon-Approach

Cells may be subdivided further during legitimate Unreal authoring.

No gameplay rule may depend on a particular cell being loaded. Streaming is an implementation detail over the continuous-world requirement.

## 6. World points

Use the existing `FAetherWorldPointDefinition` contract.

Required logical points:

- `ArrivalSpawn`
- `SettlementEntry`
- `SettlementPlaza`
- `Forge`
- `Market`
- `Tavern`
- `QuestHub`
- `SettlementGate`
- `RoadFrontier`
- `ForestEntry`
- `WildernessNorth`
- `WildernessSouth`
- `CombatFrontier`
- `RuinsLandmark`
- `DungeonApproach`
- `DungeonEntrance`

These points are content anchors. Their final transforms are authored in Unreal and must not be fabricated in source control.

## 7. Existing-system integration hooks

Phase 9 does not recreate gameplay systems.

The region exposes content hooks for existing systems:

### Quests/events
- settlement quest giver;
- arrival/intro quest hook;
- exploration discovery hook;
- ruins discovery hook;
- dungeon approach/entrance hook.

### Interaction
Use the existing interaction types for:

- Talk;
- Use;
- Harvest;
- Enter;
- Quest;
- Forge;
- Craft;
- Container;
- Custom.

### Creatures/NPCs
Use existing creature/NPC registries and spawn-group contracts.

The region defines **placement slots and tags**, not duplicate creature/AI systems.

### Economy
Settlement service locations consume existing shop/crafting/economy contracts.

### Combat
Combat-frontier zones consume the existing combat/AI/damage/loot systems.

### Persistence
World tags, quest state, actor state and progression remain under the existing persistence architecture.

### Multiplayer
Server authority, replication and existing multiplayer rules remain unchanged.

## 8. Visual production grammar

The region must establish reusable visual families for the whole world.

### Settlement families

- civic landmark;
- market/service buildings;
- forge;
- tavern;
- residential buildings;
- walls/fences;
- doors/windows;
- roofs;
- stairs/platforms;
- carts/crates/barrels;
- lamps;
- signs;
- gardens.

### Natural families

- grass/ground layers;
- shrubs;
- trees;
- rocks;
- roots;
- flowers;
- water/shoreline when applicable;
- fallen logs;
- terrain transition pieces.

### Frontier families

- ruins;
- broken walls;
- abandoned structures;
- ancient markers;
- dungeon approach pieces;
- entrance landmark.

### Presentation families

- background layers;
- far layers;
- world layer;
- midground;
- foreground;
- overlay/atmosphere;
- shadows;
- particles;
- weather/VFX.

All visual families use the existing Phase 3–8 presentation contracts rather than introducing a parallel visual architecture.

## 9. Exploration design

The region must support three exploration scales.

### Primary route

Settlement → road → wilderness → ruins → dungeon.

### Secondary routes

Paths branching from:

- settlement outskirts;
- countryside;
- forest edge;
- wilderness clearings;
- combat frontier.

### Discovery routes

Short optional paths leading to:

- environmental storytelling;
- resource pockets;
- hidden props;
- landmarks;
- optional encounter spaces;
- future quest/event locations.

The player should be able to understand the world spatially without a forced corridor.

## 10. Isometric readability

The final composition must preserve:

- clear silhouettes;
- readable walkable space;
- controlled foreground occlusion;
- strong focal landmarks;
- coherent sprite scale;
- consistent light direction;
- useful depth layering;
- readable NPC/creature separation;
- readable combat spaces;
- sufficient camera clearance.

The Phase 8 camera contract is the presentation authority.

## 11. Content authoring boundary

Repository/source can define:

- stable IDs;
- topology;
- zones;
- logical cells;
- points;
- connections;
- interaction hooks;
- placement contracts;
- asset-family requirements;
- quest/event integration points;
- validation rules.

Unreal Editor must author and validate:

- actual `.umap`;
- imported textures/sprites/Flipbooks;
- real asset references;
- final transforms;
- collision;
- navigation;
- visual layers;
- material instances;
- actual lighting;
- particles/VFX;
- editor-only data such as World Partition/Data Layers when applicable;
- runtime composition.

No binary asset is considered complete merely because this specification exists.

## 12. Acceptance for the source phase

Phase 9 source/repository scope is complete when:

- the first region has a permanent identity;
- its composition is fully specified;
- settlement, wilderness, combat frontier, landmark and dungeon approach are all represented;
- the region is explicitly non-linear;
- the region is compatible with the existing map/content/streaming contracts;
- integration hooks for quests, interaction, creatures/NPCs, economy, combat and persistence are defined;
- visual families are reusable;
- the current first-region environment target is the owner-supplied stylized 3D isometric diorama (Paper2D remains optional);
- no gameplay system has been rebuilt;
- no Unreal binary has been fabricated;
- the Unreal materialization/runtime gate remains explicitly separate.

## 13. Runtime gate deferred by project decision

The following are intentionally **not** claimed as complete:

- final `.umap` materialization;
- real Paper2D asset import;
- real sprite/Flipbook assignment;
- final collision;
- NavMesh/navigation validation;
- real exploration;
- combat runtime;
- multiplayer runtime;
- PIE;
- Dedicated Server;
- visual quality acceptance;
- profiling.

These belong to the later Unreal validation window.

## 14. Historical reconciliation

This phase supersedes the older first-region production documents as the current visual-direction interpretation.

Historical evidence retained:

- `3bc8f6ba6a01964cc6eaec6f07bc9578972bb152` — first-region production specification;
- `055eacf6d4f6460b666769588590376a47689d4e` — first-region spatial layout;
- `a19791d22003e65db50fd2e4f31fe25693077f2b` — first-region asset kit.

Those documents established the permanent-region composition, modular production kit and world continuity. The later owner-supplied 3D diorama target supersedes Phase 9's original 2D environment assumption; the historical 2D presentation and Paper2D integration remain optional rather than being removed from the project.

## Completion

**🟩 PHASE 9 — SOURCE / REPOSITORY / ARCHITECTURE COMPLETE**

**🟥 PHASE 9 — UNREAL MATERIALIZATION / RUNTIME VALIDATION DEFERRED**

Next: **Phase 10 — Integration of the existing gameplay systems with presentation**, preserving the gameplay root; its original Paper2D path remains supported while the first-region environment target is 3D.
