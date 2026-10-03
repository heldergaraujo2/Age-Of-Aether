# PHASE 5 — PRIMEIRO DIORAMA JOGÁVEL

## STATUS

**SOURCE/DOCUMENTATION: COMPLETE.**  
**UNREAL RUNTIME: PENDING LOCAL EXECUTION.**

Phase 5 defines the first real playable visual/gameplay benchmark for AGE OF AETHER. It is not a disposable prototype map and it must be authored as a real region of the future **single continuous open world**.

No runtime acceptance is claimed from source inspection alone. No fake `.umap`, `.uasset`, FBX, animation or other binary Unreal asset is introduced by this phase.

---

## 1. OBJECTIVE

Create the production contract for the first small but complete playable diorama:

**village/city nucleus → plaza → houses/services/NPCs → road → natural transition → forest/field → combat area → dungeon entrance**

The slice must prove that the project's final presentation can combine:

- Stylized Painterly Isometric;
- 2.5D Hand-Painted Diorama;
- real 3D Unreal gameplay;
- readable player/NPC silhouettes;
- real exploration;
- real interaction;
- environmental storytelling;
- continuous-world geography;
- the existing server-authoritative gameplay architecture.

The goal is not to build the whole MMORPG at this stage. The goal is to prove the **first production-quality piece of the final game**.

---

## 2. NON-NEGOTIABLE WORLD RULE

The diorama is a region of the eventual world, not a standalone test map.

Its geography must be designed so that it can conceptually and technically continue into:

**safezone/village → outskirts → road → fields → forest → wilderness → sub-region → discovery area → frontier → next jurisdiction**

Do not use arbitrary map boundaries that would contradict the final world.

World Partition, streaming, HLOD and other technical partitioning are allowed. The player-facing world identity must remain continuous.

The first slice must leave clear extension points for:

- additional roads;
- terrain continuation;
- future settlements;
- resource areas;
- creature habitats;
- landmarks;
- dungeon expansion;
- neighboring jurisdictions.

---

## 3. SLICE COMPOSITION

### A. Village/city nucleus

The first settlement should contain enough structure to feel like a real inhabited place:

- central plaza;
- main street;
- secondary streets;
- houses;
- shop;
- tavern or equivalent social building;
- forge/workshop;
- public/service building;
- small gardens;
- storage areas;
- carts;
- barrels;
- crates;
- benches;
- signs;
- lamps/posts;
- fences;
- wells/fountains where appropriate.

The settlement does not need to be a complete major city. It must instead establish the production language that can scale toward the game's approximately 40 major cities.

### B. Plaza

The plaza is the primary visual and gameplay anchor.

It should support:

- player gathering;
- NPC activity;
- navigation;
- interaction;
- quest presentation;
- visual landmarking;
- clear isometric readability.

### C. Road and transition

A visibly intentional road must leave the settlement.

The transition should demonstrate that the city does not end abruptly:

**buildings → outskirts → farms/fields → vegetation → wilderness**

### D. Natural area

Include a forest/field region with:

- trees;
- bushes;
- grass;
- rocks;
- elevation changes;
- natural obstacles;
- paths;
- resource/readability points;
- small discoveries;
- creatures when the runtime slice reaches the appropriate phase.

### E. Combat area

Reserve a readable outdoor gameplay area with:

- enough navigable space;
- clear silhouettes;
- controlled obstruction;
- combat-readable ground;
- surrounding environmental composition;
- room for future creature/AI integration.

Do not hard-code balance assumptions into the diorama.

### F. Dungeon entrance

Include a visually recognizable dungeon entrance or access landmark.

It must communicate:

- transition from overworld to dungeon;
- navigation purpose;
- visual importance;
- future expansion capability.

The full dungeon belongs to Phase 8. Phase 5 only establishes the overworld-to-dungeon relationship.

---

## 4. VISUAL BENCHMARK

The diorama must establish the project's final visual target:

**STYLIZED PAINTERLY ISOMETRIC / 2.5D HAND-PAINTED DIORAMA / 2.5D ISOMETRIC CUTAWAY**

Required qualities:

- real 3D geometry;
- elevated gameplay isometric camera;
- painterly materials;
- premium stylized architecture;
- readable characters;
- controlled stylized PBR;
- cinematic but gameplay-compatible lighting;
- contact shadows;
- depth separation;
- vegetation with strong silhouettes;
- environmental storytelling;
- visually coherent props;
- clear foreground/gameplay/midground/landmark/background hierarchy.

It must look plausibly like an actual Unreal Engine game scene, not a 2D painting or generic fantasy concept.

### Image/visual benchmark reference

A conceptual benchmark may be rendered at **4096 × 2304 (4K, 16:9)** when the chosen image-generation pipeline supports it.

This resolution is a presentation benchmark, not a runtime texture requirement.

The runtime project must not assume 4K textures or 4K rendering everywhere. Asset resolution, mip levels, LOD/Nanite, streaming and scalability must be determined by asset class and measured performance.

The benchmark should prioritize:

- high perceived detail;
- clean anti-aliasing;
- crisp silhouettes;
- preserved microdetail;
- controlled sharpness;
- no artificial oversharpening;
- readable characters/equipment;
- readable architecture/material separation.

---

## 5. GAMEPLAY CAMERA

Reuse the Phase 4 camera contract.

The slice must be authored for:

- elevated isometric view;
- approximately 3/4 presentation;
- orthographic or strongly controlled perspective;
- bounded zoom;
- stable player framing;
- readable traversal;
- camera collision/occlusion handling;
- movement relative to camera;
- selection/targeting/interaction compatibility.

The camera must not become a detached cinematic camera.

The benchmark question is:

> “É assim que eu realmente enxergaria o mundo enquanto estivesse jogando?”

---

## 6. PLAYER AND NPC READABILITY

The first diorama must provide visual space for:

- player character;
- NPCs;
- merchants/services;
- guards/residents;
- future creatures;
- interactables;
- quest markers/presentation;
- equipment silhouettes.

NPCs must not all occupy the same visual position or use identical presentation.

Activities should communicate a living settlement:

- walking;
- talking;
- working;
- trading;
- entering/exiting buildings;
- standing at service stations;
- moving along roads.

Runtime animation validation remains a local Unreal gate.

---

## 7. ENVIRONMENTAL STORYTELLING

Props must have a reason to exist.

Examples:

- wood near a workshop;
- goods near a merchant;
- tools near a forge;
- carts near roads;
- storage near buildings;
- farming elements near fields;
- worn paths indicating traffic;
- guards near entrances;
- signs guiding navigation;
- environmental traces near the dungeon route.

Avoid decorative clutter with no gameplay, navigation or storytelling purpose.

---

## 8. ART PIPELINE INTEGRATION

All real assets used by the diorama must pass through the Phase 3 asset pipeline:

**Concept/source asset → cleanup/validation → import profile → Unreal asset → materials/skeleton/collision as applicable → Asset Registry → Stable AssetID → presentation consumer → runtime validation**

The diorama must not bypass:

- provenance/licensing;
- deterministic import settings;
- dependency validation;
- collision policy;
- sockets where applicable;
- LOD/Nanite policy;
- fallback policy;
- stable asset identity.

No fake binary assets may be committed to represent completed runtime content.

---

## 9. EXISTING ARCHITECTURE INTEGRATION

Reuse the existing systems rather than creating parallel systems.

The diorama should consume the existing architecture for:

- world/map definitions;
- streaming;
- character;
- movement/camera;
- creatures/NPCs;
- interactions;
- quests/events;
- inventory/equipment where needed;
- visual profiles;
- audio;
- UI;
- performance/scale;
- content packages;
- server authority.

Phase 5 is an integration slice, not a reason to create duplicate gameplay frameworks.

---

## 10. DATA-DRIVEN AUTHORING

Where the existing data-driven registries support the content, the diorama should use stable definitions for:

- map/region identity;
- assets;
- NPCs;
- interactables;
- services;
- points of interest;
- presentation profiles;
- content package membership.

Hard-coded one-off runtime logic should not become the foundation of future city production.

The slice should be reproducible and expandable.

---

## 11. NAVIGATION AND TRAVERSAL

The player must have an intuitive route through the slice:

**spawn/start → plaza → services → road → natural area → combat space → dungeon entrance**

The environment should also support optional exploration loops:

- side streets;
- small courtyards;
- minor discoveries;
- resource/readability pockets;
- alternate routes.

Do not create a single narrow corridor disguised as an open area.

---

## 12. PERFORMANCE DESIGN

Performance must be considered from the beginning, but not guessed.

The slice must be compatible with future:

- World Partition;
- HLOD;
- foliage systems;
- skeletal mesh scalability;
- VFX scalability;
- visibility/culling;
- streaming;
- memory budgets;
- network relevance.

The following are **runtime measurements**, not source-level assumptions:

- FPS/frame time;
- Game Thread;
- Render Thread;
- GPU;
- memory;
- draw calls;
- actor counts;
- skeletal mesh cost;
- foliage cost;
- VFX cost;
- streaming behavior.

No performance PASS is declared until measured in Unreal.

---

## 13. RUNTIME ACCEPTANCE MATRIX

| Area | Source/Documentation | Unreal Runtime |
|---|---|---|
| Slice layout | Defined | Pending |
| Real map/world identity | Contract defined | Pending |
| Isometric camera | Phase 4 foundation | Pending |
| Zoom/collision/occlusion | Contract defined | Pending |
| Village geometry | Production contract | Pending real assets |
| Materials | Phase 3 pipeline | Pending import/runtime |
| Lighting | Visual direction defined | Pending |
| Player | Existing source foundation | Pending real spawn/PIE |
| NPC | Existing source architecture | Pending real runtime |
| Interaction | Existing architecture | Pending |
| Road/traversal | Defined | Pending |
| Forest/field | Defined | Pending real environment |
| Combat area | Defined | Pending integration |
| Dungeon entrance | Defined | Pending |
| Environmental storytelling | Defined | Pending |
| World continuation | Defined | Pending real map |
| Multiplayer | Architecture exists | Pending 2-client PIE |
| Dedicated Server | Architecture exists | Pending smoke test |
| Performance | Policy defined | Pending profiling |
| Output Log | Source only | Pending real inspection |

---

## 14. DEFINITION OF DONE — SOURCE/DOCUMENTATION

Phase 5 source/documentation is complete when:

- the first playable diorama is explicitly defined;
- its layout and gameplay flow are documented;
- it is explicitly part of the future continuous world;
- the visual benchmark is fixed;
- camera requirements reuse Phase 4;
- asset production reuses Phase 3;
- existing architecture is preserved;
- NPC/player readability is defined;
- environmental storytelling is defined;
- navigation/traversal is defined;
- performance requirements are defined;
- runtime acceptance matrix exists;
- no fake Unreal binary content is introduced;
- roadmap is updated;
- canonical continuity is updated.

All source/documentation requirements above are satisfied by this phase.

---

## 15. RUNTIME GATE

The real Unreal gate remains open.

Before declaring the diorama runtime-complete, the local project must provide actual evidence for:

1. Editor startup;
2. map loading;
3. real player spawn;
4. isometric camera;
5. movement;
6. zoom;
7. camera collision/occlusion;
8. village traversal;
9. real NPC presentation;
10. interaction;
11. road/forest traversal;
12. combat-area traversal;
13. dungeon entrance interaction/presentation;
14. PIE;
15. 2-client PIE;
16. Dedicated Server smoke test;
17. Output Log inspection;
18. relevant Automation;
19. performance profiling.

No source-level completion may be promoted to runtime PASS.

The unresolved **Phase 1 — Real Unreal Foundation / real Unreal environment audit** remains a mandatory runtime dependency.

---

## 16. PRODUCTION EXPANSION RULE

Once this slice is runtime-validated, its production patterns become the template for expansion:

**first diorama → first exceptional region → city production pipeline → jurisdiction population → neighboring jurisdictions → world scale**

Do not copy the same village mechanically.

Reuse:

- production pipeline;
- technical contracts;
- registries;
- visual language;
- composition rules;
- authoring workflow.

Create new:

- architecture;
- biome;
- landmarks;
- props;
- NPC identities;
- creatures;
- environmental stories;
- regional materials;
- cultural identity.

---

## 17. NEXT PHASE

**PHASE 6 — NÚCLEO RPG JOGÁVEL**

After the diorama source contract is established, the next phase defines the playable RPG loop that inhabits it:

**character → level/XP → attributes → HP/resource → basic attack → damage → death/respawn → equipment → inventory → loot**

The runtime gates of Phases 1–5 remain explicitly pending until executed in the real Unreal environment.
