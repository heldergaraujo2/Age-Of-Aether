# AGE OF AETHER — PHASE 3 — 2D ART PIPELINE

## Objective

Establish the canonical, repeatable production pipeline that turns approved 2D visual source material into deterministic Unreal-ready presentation assets while preserving the existing Age of Aether gameplay architecture.

This phase is intentionally separated from the real Unreal materialization gate. Repository/source and production-contract work can be completed now; Unreal import, asset creation, runtime presentation and visual acceptance remain deferred until the dedicated Unreal validation window.

## Non-negotiable visual quality

The project is a premium 2D isometric RPG.

Every asset produced through this pipeline must target:

- the best practical source-image quality;
- high resolution and sufficient pixel density for the intended camera distance;
- clean silhouettes and readable forms;
- consistent perspective/isometric language;
- coherent proportions and scale;
- consistent lighting direction unless an asset explicitly requires a different lighting model;
- clean edges and controlled transparency;
- no accidental halos, matte contamination or compression artifacts;
- consistent palette, materials and rendering response;
- animation that is extremely fluid, natural and responsive whenever the asset is animated.

Convenience must not silently lower quality. Any intentional reduction must be documented with a technical reason and the affected asset class.

## Architecture rule

The pipeline is a presentation layer over the existing game root.

It must not recreate or replace:

- progression and levels;
- classes/evolutions;
- skills/effects;
- combat;
- inventory;
- items/equipment;
- loot;
- quests/events;
- creatures/AI;
- economy/crafting;
- persistence;
- networking/multiplayer;
- server authority;
- world/map/streaming contracts;
- existing UI and registry architecture.

The required sequence is:

**preserve → prepare → adapt → integrate → validate → expand**

## Canonical asset lifecycle

Every production visual follows this lifecycle:

1. Concept / source
   - Original generated art, commissioned art, licensed source, or approved project-owned source.
   - Provenance must be known.
   - No unknown copyrighted or unlicensed source may silently enter the production set.

2. Ingestion
   - Assign a stable logical AssetID before integration.
   - Record category, subject, intended use, source type and revision.
   - Preserve the original source separately from derived working files.

3. Quality preparation
   - Remove accidental backgrounds or transparency defects.
   - Normalize canvas and framing.
   - Normalize scale and anchor conventions.
   - Correct obvious compression, color and edge defects.
   - Preserve intentional artistic details.
   - Never upscale a low-quality source and call the result final without documenting that limitation.

4. 2D derivation
   - Export the appropriate texture representation.
   - Create sprites or sprite sheets when multiple frames are required.
   - Create Flipbook-ready frame sequences for animated content.
   - Produce masks/material inputs only when the presentation requires them.
   - Keep source and derived assets traceable through the AssetID.

5. Presentation metadata
   - Define visual role.
   - Define anchor/origin convention.
   - Define intended world-space scale.
   - Define animation state set when applicable.
   - Define depth/layering requirements.
   - Define optional shadow, lighting, VFX and parallax requirements.

6. Registry integration
   - Connect the visual representation to the existing Asset Manager / visual registry architecture.
   - Do not introduce an isolated parallel registry merely for 2D content.
   - Missing content must resolve through an explicit fallback path.

7. Unreal materialization
   - Import/create the real Unreal asset through the legitimate Unreal Editor/toolchain.
   - Save the resulting Unreal binaries only through Unreal.
   - Never fabricate Unreal binary content in GitHub.

8. Runtime validation
   - Verify appearance, scale, anchoring, depth ordering, animation, lighting/shadow response, VFX interaction and performance in the real project.
   - Record evidence separately from source preparation.

## Asset classes

The pipeline must support at minimum:

- playable characters;
- class/evolution presentations;
- creatures;
- NPCs;
- bosses;
- architecture;
- roads and terrain dressing;
- vegetation;
- rocks and natural props;
- water;
- ruins;
- dungeon elements;
- interactable props;
- icons;
- magic and combat effects;
- environmental effects;
- weather elements;
- UI visual content;
- map/region composition layers.

## Naming and identity contract

Every production asset must have:

- stable logical AssetID;
- human-readable display name;
- category;
- visual role;
- revision/version;
- source provenance;
- intended resolution or scale target;
- animation-state metadata when applicable;
- integration target;
- fallback identifier.

Suggested logical form:

AOA.<Domain>.<Subject>.<Role>.<Variant>

Examples:

- AOA.Character.Mage.Idle
- AOA.Character.Mage.Walk
- AOA.Creature.Wolf.Attack
- AOA.Environment.MedievalHouse.Exterior
- AOA.VFX.Fireball.Impact

These are logical identifiers, not claims that corresponding Unreal assets already exist.

## Animation contract

Animated 2D content must be authored around motion quality, not merely frame availability.

For each animation:

- define the intended action and timing;
- maintain consistent anchors across frames;
- avoid visible popping during frame transitions;
- preserve anticipation where useful;
- preserve follow-through and recovery where appropriate;
- use enough frames for the intended speed and camera distance;
- ensure attack, hit, cast and death transitions remain readable;
- avoid inconsistent scale or silhouette between frames;
- define loop/non-loop behavior;
- define transition expectations between states;
- preserve responsive timing for gameplay-critical actions.

Minimum conceptual state vocabulary for characters/creatures:

- Idle;
- Walk;
- Run when applicable;
- Attack;
- Hit;
- Death;
- Cast when applicable;
- Interaction when applicable.

The existing gameplay state machine remains authoritative; the 2D layer only presents those states.

## Isometric composition contract

The visual pipeline must preserve a coherent isometric language:

- consistent camera-facing orientation;
- consistent ground contact;
- predictable feet/base anchors;
- deliberate overlap and occlusion;
- readable foreground/background ordering;
- controlled scale changes;
- silhouettes that remain recognizable at gameplay distance;
- no accidental perspective mismatch between neighboring assets.

Depth may be created with:

- layer ordering;
- sprite offsets;
- occlusion;
- parallax;
- shadows;
- lighting;
- particles;
- fog;
- VFX;
- carefully authored background/foreground elements.

3D remains optional and may only be introduced when it provides a clear technical or visual benefit.

## Provenance and source-of-truth rules

For every asset, the project must be able to answer:

- where did this source come from?
- is it original, generated, commissioned or licensed?
- which revision was approved?
- which derived representation came from that source?
- where is it intended to be used?
- which existing gameplay/presentation consumer receives it?
- what fallback is used if the asset is unavailable?

Generated images are production sources, not automatically Unreal assets.

An image file alone does not prove:

- a valid Unreal Texture;
- a Sprite;
- a Flipbook;
- a material;
- a registry entry;
- a playable visual;
- runtime correctness.

## Quality gates

### Gate P0 — source integrity

PASS only when provenance and AssetID are known.

### Gate P1 — visual preparation

PASS only when framing, transparency, scale, edges and quality are acceptable.

### Gate P2 — 2D derivation

PASS only when the required texture/sprite/sprite-sheet/Flipbook representation is defined.

### Gate P3 — presentation contract

PASS only when scale, anchor, state, depth and fallback requirements are defined.

### Gate P4 — registry contract

PASS only when the intended existing registry/Asset Manager integration point is identified.

### Gate P5 — Unreal materialization

Requires the real Unreal Editor/toolchain and produces actual Unreal assets.

### Gate P6 — runtime visual acceptance

Requires real Unreal execution and evidence.

P0–P4 are repository/source production gates. P5–P6 are deliberately deferred runtime gates.

## Fallback policy

Every runtime-consumable visual must have an explicit fallback strategy.

Fallbacks must:

- be deterministic;
- preserve gameplay readability;
- never silently break gameplay logic;
- be distinguishable from final production content during development;
- be replaceable without changing the gameplay contract.

The fallback is not a substitute for production art.

## Automation direction

The pipeline is designed so future tooling can automate:

- asset manifest creation;
- provenance validation;
- naming validation;
- duplicate detection;
- source/derived relationship checks;
- animation frame consistency checks;
- missing-state detection;
- registry coverage checks;
- fallback coverage checks;
- quality metadata validation.

Automation may reject malformed or incomplete assets, but it must not invent visual content or fabricate Unreal binaries.

## Phase 3 completion boundary

Repository/source completion for this phase means:

- the canonical lifecycle is defined;
- supported asset classes are defined;
- stable identity/provenance rules are defined;
- 2D derivation rules are defined;
- animation-quality rules are defined;
- isometric/depth rules are defined;
- registry/fallback integration rules are defined;
- source/runtime separation is explicit;
- Unreal binary fabrication is prohibited;
- runtime materialization is explicitly queued for later validation.

This phase is therefore complete at the **repository/source contract level**.

It is not a claim that production Unreal assets or runtime-ready visual assets already exist.

## Deferred runtime work

The following remains intentionally deferred to the Unreal validation window:

- importing real production source images;
- creating real Unreal Textures/Sprites/Sprite Sheets/Flipbooks;
- assigning materials;
- registering real asset instances;
- viewing assets in the real isometric camera;
- validating animation fluency in runtime;
- validating depth, shadows, lighting, VFX and performance;
- validating the final visual result in PIE/multiplayer.

## Exit status

**PHASE 3 — SOURCE/PIPELINE CONTRACT: COMPLETE**

**PHASE 3 — REAL UNREAL MATERIALIZATION: DEFERRED**

This distinction is intentional and follows the project's rule of truth.
