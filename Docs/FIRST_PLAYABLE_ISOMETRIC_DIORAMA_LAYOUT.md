# First Playable Isometric 2.5D Diorama Layout

## Purpose

This document defines the spatial composition for `AetherWorld_FirstRegion`, the first permanent playable region of Age of Aether.

It is a production layout specification, not a fabricated Unreal binary asset. The Unreal `.umap` must be materialized and saved by Unreal Editor.

## Visual target

The region is a real 3D environment presented as a stylized painterly isometric 2.5D diorama.

The camera, terrain, architecture, vegetation, materials, lighting, silhouettes, and landmark placement must work together as one composed scene. The target is not a flat background image and not a disposable graybox.

## Composition

The playable route is organized as:

1. Arrival / lower approach
2. Permanent settlement
3. Central plaza
4. Service buildings and social spaces
5. Settlement gate
6. Main road
7. Cultivated fields / countryside
8. Forest edge
9. Exploration pockets
10. Combat territory
11. Ruins / environmental landmark
12. Dungeon approach
13. Dungeon entrance

The route must remain traversable and readable from the isometric camera.

## Spatial hierarchy

The settlement is the visual and gameplay anchor. The plaza is the primary composition focal point. The road establishes the main visual axis from the settlement toward the wilderness.

The outer terrain progressively becomes less domesticated:

settlement -> road -> fields -> forest edge -> wilderness -> combat territory -> ruins -> dungeon.

Terrain elevation should reinforce this hierarchy without creating artificial cliffs or obstructing the camera.

## Diorama composition

The camera should be able to frame the region with:

- foreground: arrival path, vegetation, props and readable player-scale elements;
- gameplay layer: settlement, roads, NPCs, activities and traversal;
- midground: fields, forest edge, exploration landmarks and combat space;
- background: elevated terrain, distant ruins and dungeon landmark;
- atmosphere: controlled depth, fog and lighting separation.

Major silhouettes must remain readable at the intended gameplay camera distance.

## Settlement

The settlement must contain a coherent nucleus rather than isolated buildings.

Required functional composition:

- central plaza;
- recognizable civic landmark;
- market/service area;
- crafting/forge area;
- tavern/social building;
- residential cluster;
- clear pedestrian routes;
- settlement entrance and gate;
- small environmental props that communicate daily life.

Buildings should be arranged around spaces and routes, not placed as a random collection of meshes.

## Road

The main road must visually and physically connect the settlement to the wilderness.

It should:

- leave the settlement through the gate;
- provide the primary exploration route;
- contain readable bends rather than an artificial straight corridor;
- transition naturally from maintained road to countryside path;
- support traversal, NPC navigation and future expansion.

## Countryside

The countryside provides visual breathing room between settlement and forest.

Use:

- gently varied terrain;
- fields/meadows;
- sparse vegetation;
- fences or minor infrastructure where appropriate;
- small points of environmental storytelling;
- secondary paths that invite exploration.

## Forest and exploration

The forest edge should become progressively denser.

Use layered vegetation rather than a uniform wall:

- grass and ground cover;
- shrubs;
- medium vegetation;
- mature trees;
- rocks and fallen material;
- small clearings;
- hidden traversal pockets;
- optional landmarks.

Exploration pockets should reward leaving the main road without forcing every player through a single corridor.

## Combat territory

The combat space must be visually distinguishable from peaceful countryside without relying solely on UI.

Use:

- stronger terrain framing;
- ruins or other environmental storytelling;
- denser/rougher vegetation;
- readable encounter spaces;
- enough open ground for combat movement;
- routes for creatures and NPCs.

The combat territory must not be a tiny arena disconnected from the world.

## Dungeon approach

The dungeon entrance is a major distant landmark.

It should be visually readable from the surrounding region while remaining integrated into the terrain.

The approach should progressively communicate:

wilderness -> danger -> ancient/forgotten landmark -> dungeon entrance.

## Terrain requirements

The existing Landscape in `AetherWorld_FirstRegion` is the physical base for the region.

The terrain should eventually be shaped to provide:

- a relatively stable settlement area;
- natural drainage;
- believable slopes;
- readable road grades;
- elevation variation around the wilderness;
- terrain framing for the dungeon/landmark;
- no arbitrary walls around the playable area.

Do not replace the Landscape with a disposable graybox.

## Isometric camera requirements

The final composition must be validated from the actual gameplay camera.

Validation must include:

- player readability;
- route readability;
- building silhouette readability;
- no excessive occlusion;
- controlled depth;
- clear foreground/midground/background separation;
- usable movement through the scene.

The map must not be judged only from the editor's free camera.

## Asset policy

No fabricated `.uasset`, `.umap`, FBX or other Unreal binary is accepted as a substitute for real editor materialization.

Assets must enter through the project's existing visual/art pipeline and registries where applicable.

Temporary primitives may be used only as controlled development scaffolding and must not be mistaken for the finished visual target.

## Runtime definition of done

The region is not complete when the layout exists in documentation.

Completion requires real Unreal evidence that:

- `AetherWorld_FirstRegion` opens successfully;
- the player spawns;
- the isometric camera works;
- the player can traverse the complete route;
- terrain collision works;
- settlement spaces are readable;
- NPCs/creatures can inhabit the intended spaces;
- combat territory is reachable;
- dungeon entrance is reachable;
- PIE works;
- two-client PIE is evaluated;
- dedicated-server behavior is evaluated where applicable;
- logs are reviewed;
- performance is profiled;
- the final composition is reviewed from the actual gameplay camera.

## Current status

The permanent map `AetherWorld_FirstRegion.umap` has been created locally in Unreal Editor by the project user.

The repository specification does not claim that binary map content is present in GitHub.

The next implementation step is to materialize this layout in Unreal using real project assets and the existing gameplay/presentation architecture, without rebuilding systems that already exist.
