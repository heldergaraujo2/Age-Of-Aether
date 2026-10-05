# PHASE 4 — CÂMERA E EXPLORAÇÃO ISOMÉTRICA

## STATUS

**SOURCE/DOCUMENTATION: COMPLETE.**  
**UNREAL RUNTIME: PENDING LOCAL EXECUTION.**

This phase defines the camera and exploration contract for the final **Stylized Painterly Isometric / 2.5D Hand-Painted Diorama** experience.

It reuses the existing Phase 40 movement/camera source foundation rather than creating a second movement system. No fake Unreal binaries are introduced and no runtime acceptance is claimed.

---

## 1. OBJECTIVE

Provide a comfortable, readable and multiplayer-consistent exploration model:

**Camera → cursor-to-world destination → click-to-move → character orientation → selection/targeting → interaction → authoritative gameplay → presentation**

The camera must make the world readable while preserving real 3D traversal and the scale of the continuous world.

---

## 2. EXISTING FOUNDATION TO PRESERVE

The repository already contains the Phase 40 source-level movement/camera foundation, including:

- configurable movement/camera profile;
- walk/sprint/jump/rotation values;
- camera zoom range/step;
- pitch limits;
- left-click-to-move;
- right-click basic attack;
- F7 toggles local 3D orbit-camera control while preserving the last user view;
- middle-mouse drag rotates/tilts the view; Page Up/Down provide a fine pitch adjustment toward overhead/front view;
- F6 resets to the original camera view;
- mouse-wheel zoom, including close character inspection in orbit mode;
- jump;
- sprint;
- server-authoritative sprint transition;
- camera pitch clamping;
- automation coverage.

Phase 4 therefore defines the **isometric exploration behavior and acceptance contract** around that foundation instead of replacing it.

---

## 3. CAMERA MODEL

Preferred baseline:

- elevated isometric presentation;
- orthographic camera or strongly controlled perspective;
- stable pitch range;
- bounded zoom;
- smooth but controlled camera motion;
- no uncontrolled camera drift;
- consistent framing of the player;
- sufficient view of nearby traversal hazards and interactables.

The implementation must preserve the ability to use real 3D terrain, slopes, verticality, buildings, foliage, dungeons and World Partition.

### Camera goals

At normal exploration distance the player should clearly read:

- character silhouette;
- movement direction;
- nearby enemies;
- NPCs;
- interactable objects;
- terrain obstacles;
- roads and paths;
- points of interest.

At zoomed-out distances the composition should communicate:

- route;
- local geography;
- landmark direction;
- jurisdiction identity;
- combat/exploration context.

---

## 4. ZOOM

Zoom is gameplay-adjacent presentation and must remain bounded.

Requirements:

- minimum and maximum zoom;
- deterministic step or smooth interpolation;
- no clipping into the character;
- no excessive clipping through terrain/buildings;
- character remains readable at normal gameplay distance;
- distant view does not make targeting/interactions ambiguous;
- multiplayer clients can use presentation-local zoom without changing authoritative world state.

Zoom must never modify gameplay authority such as movement speed, damage, targeting range or interaction distance.

---

## 5. CAMERA COLLISION / OCCLUSION

The camera must not become trapped inside world geometry.

The runtime design should support:

- camera collision traces;
- obstruction handling;
- controlled camera pull-in;
- recovery when obstruction ends;
- transparency/occlusion strategy only where appropriate;
- protection against camera clipping through terrain and large structures.

Occlusion handling must preserve gameplay readability.

Buildings and props should not permanently hide the character or important interaction targets simply because the camera is at the intended isometric angle.

---

## 6. MOVEMENT RELATIVE TO CAMERA

Exploration controls should be intuitive and preserve the mouse click-to-move first-playable contract:

- left-click selects a ground destination and the character moves there;
- movement direction is derived from the destination, not from camera-relative WASD axes;
- character orientation follows the intended movement/combat rules;
- camera rotation does not unexpectedly change the selected world destination;
- keyboard movement bindings remain absent in the current first-playable input setup.

The implementation must remain server-authoritative.

The client may request movement; the server remains the authority over resulting character state.

---

## 7. ROTATION POLICY

Rotation is optional presentation behavior and must not be required to make basic exploration usable.

If rotation is enabled:

- bounded input;
- predictable direction;
- no abrupt inversion;
- no loss of player readability;
- movement controls remain understandable after rotation.

If the final camera remains fixed-angle, the same movement-relative-to-camera contract must still hold.

---

## 8. PLAYER FRAMING

The player should occupy a stable visual region of the screen.

Avoid:

- excessive dead space;
- player touching screen edges during ordinary exploration;
- camera lag that makes movement feel disconnected;
- abrupt camera snaps;
- excessive camera smoothing that harms combat/interaction response.

The framing must support:

**exploration → encounter → combat → loot → continued traversal**

without requiring a camera mode change for ordinary play.

---

## 9. SELECTION AND TARGETING

Selection must be compatible with the existing authoritative gameplay architecture.

The presentation layer may identify:

- player;
- NPC;
- creature;
- interactable;
- loot;
- world point.

The server validates gameplay actions.

Targeting presentation should communicate:

- selected target;
- target type;
- distance/availability when relevant;
- target health/status when applicable;
- invalid/out-of-range state.

Targeting range must come from gameplay rules, not camera zoom.

---

## 10. INTERACTION

Interaction should use a clear world-space or HUD presentation:

**approach → identify → prompt → request → server validation → result → presentation**

Examples:

- NPC;
- quest;
- shop;
- crafting;
- loot;
- door/portal;
- world object;
- event interaction.

Camera positioning must not be required to manually align the character with every interactable unless the gameplay design explicitly requires it.

---

## 11. ISOMETRIC READABILITY

The camera is part of the art direction.

Acceptance must consider:

### Silhouette
Character, creature and equipment remain distinguishable.

### Layering
Foreground, gameplay space, midground and landmarks remain visually separated.

### Depth
Lighting, shadows, elevation and occlusion communicate spatial relationships.

### Navigation
Roads, terrain breaks, landmarks and entrances are readable.

### Combat
Attacks, hit reactions, VFX and target identity remain visible.

### Exploration
The player can understand where they are and where they can reasonably go.

---

## 12. CONTINUOUS-WORLD COMPATIBILITY

The camera must work across the complete world model:

**city/safezone → outskirts → wilderness → sub-regions → discoveries → natural frontier → next jurisdiction**

It must support:

- large open terrain;
- roads;
- forests;
- mountains;
- rivers/coasts;
- settlements;
- ruins;
- dungeons;
- landmarks;
- vertical terrain;
- World Partition;
- streaming/HLOD;
- long-distance exploration.

The camera system must not assume small isolated maps.

---

## 13. MULTIPLAYER CONSISTENCY

Camera presentation is client-local, but gameplay authority is server-side.

The system must preserve:

- server-authoritative movement;
- replicated player state;
- consistent character orientation/state where gameplay requires it;
- no camera-only manipulation of gameplay outcomes;
- target validation on the authoritative side;
- predictable presentation for other players.

Two clients should be able to explore the same world while using local camera presentation without corrupting shared state.

---

## 14. PERFORMANCE

Camera and exploration must be compatible with the future large-world budget.

Evaluate in real Unreal:

- camera trace cost;
- visibility/culling;
- foliage density;
- skeletal mesh count;
- VFX;
- World Partition streaming;
- HLOD;
- frame time;
- memory;
- network relevance.

No major performance decision should be declared PASS without profiling.

---

## 15. RUNTIME ACCEPTANCE MATRIX

| Area | Source/Repository | Unreal Runtime |
|---|---|---|
| Camera profile | Implemented | Tune/verify |
| Isometric angle | Contract | Visual verification |
| Zoom bounds | Implemented | Real input verification |
| Camera collision | Contract | Real geometry test |
| Occlusion | Contract | Real buildings/terrain test |
| Relative movement | Foundation exists | PIE verification |
| Character framing | Contract | Visual verification |
| Selection | Architecture/contract | Real interaction |
| Targeting | Existing gameplay architecture | Real target test |
| Interaction | Existing interaction system | Real world interaction |
| Multiplayer | Architecture | 2-client PIE |
| Dedicated Server | Architecture | Runtime smoke test |
| Performance | Policy | Real profiling |
| Output Log | Source only | Real log inspection |

---

## 16. SOURCE VALIDATION

Phase 4 source-level closure is based on:

- existing Phase 40 movement/camera implementation;
- existing authoritative movement foundation;
- existing interaction/creature/world systems;
- existing Phase 2 visual direction;
- existing Phase 47 world/streaming contracts;
- explicit camera/exploration requirements documented here.

No duplicate movement/camera architecture is introduced.

---

## 17. RUNTIME GATE

The following remain pending in the user's real Unreal installation:

- actual camera setup;
- real development map;
- real isometric angle;
- zoom;
- camera collision;
- occlusion;
- movement-relative-to-camera;
- character framing;
- selection;
- targeting;
- interaction;
- 2-client PIE;
- Dedicated Server smoke test;
- Output Log;
- profiling.

The existing **Phase 1 — Real Unreal Foundation** audit remains the mandatory unresolved runtime gate.

Therefore:

**PHASE 4 = SOURCE/DOCUMENTATION COMPLETE / UNREAL RUNTIME PENDING.**

No runtime PASS is inferred from source correctness.

---

## 18. DEFINITION OF DONE — SOURCE/DOCUMENTATION

Phase 4 is complete at source/documentation level when:

- camera model is defined;
- zoom contract is defined;
- collision/occlusion policy is defined;
- movement-relative-to-camera is defined;
- rotation policy is defined;
- framing/readability requirements are defined;
- selection/targeting/interaction flow is defined;
- multiplayer authority boundaries are explicit;
- continuous-world compatibility is explicit;
- performance concerns are explicit;
- runtime acceptance matrix exists;
- roadmap is updated;
- PROJECT_MEMORY continuity is updated;
- no fake Unreal binaries are introduced.

All source/documentation requirements above are satisfied.

---

## 19. NEXT PHASE

**PHASE 5 — PRIMEIRO DIORAMA JOGÁVEL**

The next phase should define the first real visual/gameplay benchmark:

**village/city → plaza → houses/shop/NPC → road → forest → combat area → dungeon entrance**

It must be designed as part of the eventual continuous world, not as a disposable isolated map.

The real Unreal audit remains open throughout.
