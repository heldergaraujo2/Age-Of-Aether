# FIRST PLAYABLE WORLD REGION — PRODUCTION SPECIFICATION

## Status
PRODUCTION SPECIFICATION: COMPLETE
REAL UNREAL MAP (.umap): PENDING LOCAL UNREAL EDITOR MATERIALIZATION

This is the first permanent production region of Age of Aether, not a disposable test map. It must remain expandable into the final single continuous world.

No fabricated .umap, .uasset, FBX, texture package, or other Unreal binary may be introduced into source control.

## 1. Required player experience

Primary route:

settlement nucleus -> plaza/services -> outskirts -> gate/road -> countryside -> forest/field -> exploration pockets -> combat territory -> dungeon approach -> dungeon entrance

The region must be genuinely explorable, with optional side routes and discoveries. It must not be a corridor disguised as an open area.

## 2. Spatial composition

### A — Settlement nucleus
Central plaza, main streets, residential structures, merchant/service frontage, forge/workshop, tavern/social building, storage, carts, gardens, focal landmark, controlled wilderness transition, secondary alleys and courtyards.

### B — Outskirts
Smaller structures, fences, fields/gardens, storage, work areas, secondary paths, transitional vegetation and progressively less formal architecture.

### C — Main road
A broad, readable route connecting settlement and frontier. It needs terrain wear, secondary paths, believable terrain following, landmarks and environmental storytelling.

### D — Natural transition
Managed land -> meadow/field -> scrub -> woodland. No hard artificial biome boundary.

### E — Wilderness
Multiple traversal routes, clearings, elevation changes, rocks, vegetation layers, water features where appropriate, minor discoveries, resource/readability pockets and natural landmarks.

### F — Combat territory
Natural encounter spaces, line-of-sight variation, obstacles/cover, multiple approaches, room for melee/ranged gameplay, creature navigation and visual escalation toward the dungeon.

### G — Dungeon approach
Increasing isolation, danger, geological/ecological change and environmental traces, ending in a major recognizable dungeon landmark.

## 3. Premium visual target

Current direction (updated after the owner supplied the target reference):

**Stylized 3D Isometric Storybook Diorama** — vivid, carefully composed 3D forms in Unreal, with a castle landmark, winding water and bridges, a lived-in village, cultivated fields, low hills, and layered forest. The screenshot is a visual target, not a source to copy.

Required qualities:
- readable, coherent silhouettes at gameplay zoom;
- rich color grouping and deliberate regional composition;
- clear material separation for stone, timber, roofs, soil, crops and foliage;
- soft contact shadows and consistent key lighting;
- visibly shaped terrain and natural river/road transitions;
- dense but disciplined environmental storytelling;
- characters and routes remain readable against scenery;
- a crafted miniature/storybook feeling without blocking traversal.

The current runtime mesh blockout is a composition/scale prototype only. Production quality still requires authored meshes/materials, lighting review and UE runtime acceptance; flat colors and engine primitives are not the final visual target.

## 4. Isometric composition

Compose for the actual gameplay camera, not a free-flying cinematic camera.

Player, NPCs, creatures, entrances, interactables, roads and landmarks must remain readable at gameplay zoom.

Use architecture, foliage and terrain to create depth without hiding gameplay-critical information.

Each major view should follow:

foreground -> gameplay space -> midground -> landmark -> atmospheric background.

## 5. Architecture

Use a reusable modular kit: walls, foundations, roofs, doors, windows, beams, stairs, balconies, fences, market stalls, carts, signs, barrels, crates and workshops.

Avoid repetition through silhouette, proportions, material aging, accents, attachments and purposeful props. Do not create dozens of unique meshes where a strong modular kit is sufficient.

## 6. Terrain and vegetation

Terrain must have believable elevation, broad traversable areas, readable slopes, road logic, drainage logic and expansion-friendly borders.

Vegetation must have ecological layers:
ground cover -> grass -> small plants -> shrubs -> medium vegetation -> trees -> deadwood/rocks -> focal vegetation.

Density must communicate:
settled -> managed -> semi-wild -> wilderness.

Foliage must remain readable under the isometric camera.

## 7. Lighting

Establish the final visual lighting language:
- physically coherent key light;
- soft but readable shadows;
- strong grounding/contact;
- controlled exposure;
- atmospheric depth;
- readable interior/exterior contrast;
- cinematic composition without obscuring gameplay.

Do not use aggressive post-processing to hide weak assets.

## 8. Material language

Stone: broad painterly variation, worn/chipped edges, controlled roughness.
Wood: directional grain, authored variation, contact wear.
Metal: readable edge response, controlled highlights, oxidation/wear where appropriate.
Fabric: softer response, readable folds.
Soil: broad tonal variation and path wear.
Foliage: readable masses, painterly value grouping and controlled translucency where useful.

## 9. Environmental storytelling

Every important prop cluster must have a purpose: usage, navigation, local economy, history, danger or world connection.

Examples include tools near workshops, goods near merchants, carts on roads, storage near services, worn paths near gates, guards near entrances and environmental traces near the dungeon route.

Avoid decorative clutter without purpose.

## 10. Landmark hierarchy

Primary landmarks orient the player across the region.
Secondary landmarks orient local traversal.
Tertiary landmarks reward exploration.

The player should be able to build a mental map from visual cues.

## 11. Navigation and multiplayer

Required route:
spawn -> plaza -> services -> gate -> road -> wilderness -> combat -> dungeon entrance.

Required optional loops:
side street, courtyard, secondary road, wilderness detour, discovery pocket and alternate combat approach.

Reserve readable spaces for multiple players, NPCs, social activity and combat. Avoid multiplayer bottlenecks.

## 12. Performance by design

Use modular assets, sensible material reuse, controlled foliage density, appropriate collision, asset-class-specific LOD/Nanite decisions, HLOD compatibility, streaming-friendly organization and disciplined actor density.

No performance PASS may be claimed without real Unreal profiling.

## 13. Asset acceptance

First establish the visual language through:
1. settlement architecture;
2. terrain/environment;
3. ground/road materials;
4. vegetation;
5. player visual;
6. NPC visual;
7. creature visual;
8. props;
9. lighting;
10. VFX/UI presentation.

Every real asset must follow the existing Phase 3 pipeline and existing registries/consumers.

## 14. Unreal materialization

The repository cannot manufacture the binary map. The actual .umap must be created and saved by Unreal Engine 5.8.

Materialization:
1. pull official main;
2. open AgeOfAether.uproject in UE 5.8;
3. create the production world using this specification;
4. establish terrain and world composition;
5. assemble real assets through the existing pipeline;
6. connect existing world/map registries and GameMode;
7. save the real .umap;
8. reopen it from a clean editor session;
9. run PIE;
10. walk the complete route;
11. validate camera, movement, collision and occlusion;
12. validate 2-client PIE;
13. validate Dedicated Server;
14. profile;
15. record evidence.

## 15. Definition of done

The first region is runtime-complete only when:
- the real map opens;
- settlement and wilderness are coherent;
- traversal works;
- combat territory works;
- dungeon entrance exists;
- camera/movement/collision/occlusion work;
- materials and lighting are validated;
- NPCs/creatures present correctly;
- multiplayer traversal works;
- Dedicated Server smoke test works;
- no critical Output Log errors remain;
- profiling evidence exists;
- the region remains expandable into the continuous world.

Until then the status is RUNTIME PENDING.
