# First Playable Isometric Diorama — Production Asset Kit

## Purpose

This document defines the minimum real art/content kit required to materialize `AetherWorld_FirstRegion` as the first premium playable isometric 2.5D region.

It does not fabricate or embed Unreal binary assets. It defines exactly what real assets must be acquired/created, validated, imported and connected to the existing project architecture.

## Current evidence

The official repository currently contains the real `AetherWorld_FirstRegion.umap` only as a locally materialized Unreal map. The `Content/Aether` tree in the official clone contains README placeholders for most art categories and does not yet contain the production environment asset library.

Therefore the next production step is **asset acquisition/creation and pipeline validation**, not manual graybox decoration.

## Visual target

**Stylized Painterly Isometric / 2.5D Hand-Painted Diorama / 2.5D Isometric Cutaway**

The final region must be a real 3D Unreal environment viewed through the gameplay camera.

Assets must be evaluated for:

- strong silhouette at isometric gameplay distance;
- painterly material response;
- readable value grouping;
- controlled stylized PBR response;
- coherent scale;
- coherent regional art language;
- believable contact with terrain;
- readable shadows;
- controlled repetition;
- multiplayer readability;
- performance suitability for a continuous world.

Technically valid but visually inconsistent assets are not accepted merely because they import successfully.

## First-region composition

The kit must support this permanent route:

1. arrival / lower approach;
2. settlement;
3. central plaza;
4. service/social buildings;
5. settlement gate;
6. main road;
7. fields/countryside;
8. forest edge;
9. exploration pockets;
10. combat territory;
11. ruins/landmark;
12. dungeon approach;
13. dungeon entrance.

The settlement is the visual anchor, the plaza is the primary focal point, and the road is the main visual axis toward the wilderness.

## Asset families

### A. Settlement architecture

Minimum first slice:

- 1 central civic/plaza landmark;
- 1 market/service building;
- 1 forge/crafting building;
- 1 tavern/social building;
- 3-5 residential building variants;
- 1 gate assembly;
- modular wall/fence pieces;
- roof variants;
- doorway/window variants;
- stairs/platform pieces;
- small architectural trims.

The kit must support modular composition instead of one-off buildings.

### B. Streets and public-space kit

Required:

- primary road material/mesh system;
- secondary path;
- plaza surface;
- curb/edge treatment where appropriate;
- steps;
- bridge or crossing piece if terrain requires it;
- road transition from maintained settlement surface to countryside;
- road props and edge dressing.

Roads must remain readable from the isometric camera and support traversal.

### C. Settlement props

Required categories:

- market stalls;
- crates;
- barrels;
- sacks;
- carts;
- benches;
- signs;
- lanterns;
- wells/fountains;
- tools;
- fire/forge props;
- fences/gates;
- small vegetation;
- decorative clutter.

Props should communicate daily life and provide scale without visually overwhelming gameplay.

### D. Countryside

Required:

- grass/meadow surface language;
- field/crop variation;
- fences;
- dirt paths;
- rocks;
- shrubs;
- small trees;
- environmental storytelling props;
- minor structures where useful.

The countryside must provide visual breathing room between settlement and forest.

### E. Forest and wilderness

Required:

- at least 2-3 tree silhouettes;
- smaller tree variants;
- shrubs;
- grass/ground-cover variants;
- rocks;
- fallen logs/branches;
- roots/forest debris;
- clearings;
- traversal-pocket dressing;
- one or more exploration landmarks.

Vegetation must be layered rather than a uniform wall.

### F. Combat territory

Required:

- rougher terrain language;
- combat-space dressing;
- ruins or broken structures;
- rocks/obstacles that frame encounters without blocking movement;
- distinctive vegetation;
- environmental danger cues;
- creature staging spaces.

Combat territory must remain part of the continuous world rather than a disconnected arena.

### G. Ruin / landmark / dungeon approach

Required:

- distant landmark silhouette;
- ruin kit;
- entrance architecture;
- approach dressing;
- broken stone/debris;
- lighting/atmospheric cues;
- readable dungeon entrance silhouette.

The entrance must be recognizable from the surrounding region and become progressively more imposing along the approach.

### H. Character presentation proof

The first accepted character presentation should provide:

- one playable skeletal character;
- skeleton;
- Physics Asset;
- material;
- basic locomotion;
- basic attack animation;
- visual profile;
- stable AssetID;
- existing playable-character visual component integration.

This proves that the environment is being built around the actual game character scale and camera, not an abstract environment-only benchmark.

### I. NPC / creature proof

After the character proof:

- one NPC presentation;
- one creature presentation;
- one combat-capable encounter space;
- one VFX proof;
- one interaction point.

Reuse the existing gameplay, creature, interaction and presentation architecture.

## Material language

The environment should use a controlled material vocabulary:

- painted stone;
- warm/dark timber;
- plaster/clay surfaces;
- aged metal;
- painted/signage materials;
- dirt/soil;
- grass;
- foliage;
- water where composition requires it.

Material variation should be achieved through reusable materials/instances and texture variation rather than dozens of bespoke shader implementations.

## Scale rules

All assets must be checked against the actual Unreal scale and playable character.

Before mass production, validate:

- doorway/player relationship;
- stairs;
- furniture;
- road width;
- plaza scale;
- tree scale;
- gate scale;
- dungeon entrance scale.

A visually attractive asset that is incorrectly scaled is rejected before mass placement.

## Pipeline acceptance order

Do not acquire or import the entire library before proving the pipeline.

### Gate 1 — Character

1. source asset;
2. provenance/license;
3. import profile;
4. skeletal mesh;
5. skeleton;
6. Physics Asset;
7. material;
8. animation;
9. stable AssetID;
10. visual profile;
11. runtime presentation.

### Gate 2 — Environment prop

1. source mesh;
2. material;
3. collision;
4. LOD/Nanite decision;
5. AssetID;
6. registry;
7. real placement in `AetherWorld_FirstRegion`;
8. gameplay-camera inspection.

### Gate 3 — Architecture module

1. modular building/structure;
2. materials;
3. collision;
4. repeatability test;
5. AssetID;
6. placement in settlement;
7. isometric composition inspection.

### Gate 4 — Vegetation

1. tree/vegetation source;
2. material;
3. foliage/placement strategy;
4. collision policy;
5. performance class;
6. repeated placement test;
7. camera/readability review.

### Gate 5 — Complete visual route

Only after Gates 1-4 pass:

settlement → plaza → gate → road → countryside → forest → exploration → combat → ruin → dungeon.

## Source and licensing

Every production asset must have recorded provenance:

- source;
- creator/vendor;
- license;
- permitted modification;
- version;
- dependencies;
- attribution requirements;
- source format.

Do not introduce an asset whose redistribution or commercial-use rights are unclear.

## Unreal import policy

Use the existing Phase 3 pipeline.

For UE 5.8, real external assets may be imported through the supported content pipeline. Interchange provides a customizable import framework; FBX remains supported through the Unreal content pipeline, and glTF/GLB are supported for open-standard interchange.

The project must retain deterministic import settings and inspect the generated Unreal assets before runtime acceptance.

## Repository organization target

The intended production organization is:

`Content/Aether/Characters/`
`Content/Aether/Architecture/`
`Content/Aether/Environment/`
`Content/Aether/Props/`
`Content/Aether/Vegetation/`
`Content/Aether/Materials/`
`Content/Aether/Animations/`
`Content/Aether/VFX/`
`Content/Aether/Maps/`

Do not create these directories merely to make empty placeholders. Create them when the corresponding real assets enter the project.

## No-graybox rule

Temporary Unreal primitives are permitted only to validate:

- scale;
- traversal;
- camera framing;
- collision;
- rough spatial composition.

They are not the visual deliverable and must be replaced by real assets before the region can be called visually complete.

## No-fabrication rule

Never create fake:

- `.uasset`;
- `.umap`;
- `.fbx`;
- `.glb`;
- texture binaries;
- skeletal assets;
- animation assets.

Real Unreal binaries must be produced by Unreal Editor or a legitimate external content pipeline and then verified locally.

## Definition of done

The asset kit is production-ready when:

- every required region category has identified real assets;
- provenance/license is known;
- import profiles are validated;
- AssetIDs and registries resolve;
- materials match the painterly target;
- collision is correct;
- character scale is correct;
- architecture is modular;
- vegetation can be repeated without destroying performance;
- the entire route can be composed without placeholder geometry;
- the actual isometric camera preserves readability;
- PIE, two-client PIE and applicable dedicated-server checks can be performed;
- profiling evidence exists for the populated region.

## Immediate next action

Do not continue manual landscape sculpting.

First obtain/produce the **Gate 1 character proof and Gate 2 environment-prop proof**, then use those real assets to validate the complete pipeline before scaling the environment library.

The existing gameplay/presentation architecture must be reused. No parallel visual framework should be created for the first region.
