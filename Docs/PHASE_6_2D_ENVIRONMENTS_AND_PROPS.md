# AGE OF AETHER — PHASE 6: 2D ENVIRONMENTS AND PROPS

## Objective

Create the reusable repository/source foundation for the premium 2D isometric world library without turning each region into a disposable image and without rebuilding world gameplay.

## Architecture

`UAether2DEnvironmentVisualProfile` is the data contract for reusable environment assets. It provides:
- stable `VisualProfileID`;
- `AssetKind`: Architecture, Road, Vegetation, Rock, Water, Ruin, Dungeon, Prop or Interactive;
- `FamilyID` for reusable visual families and variants;
- Paper2D Sprite reference;
- scale and world offset;
- render-layer priority;
- optional shadow casting.

`UAether2DEnvironmentVisualComponent` is the reusable presentation component. It validates the profile, creates a Paper2D Sprite component, applies the sprite, scale, offset, render ordering and shadow configuration, and does not run on Dedicated Server.

## Reuse contract

The library is intended to support modular combinations of:
- houses, walls, doors, towers and temples;
- bridges, roads and paths;
- trees, vegetation and rocks;
- water and shoreline elements;
- ruins and dungeon pieces;
- fires, furniture and decorative props;
- interactable visual props.

A region should compose these reusable families rather than becoming one irreversible background image.

## Gameplay preservation

This phase does not create or replace collision, navigation, interaction logic, destruction, economy, quests, combat, spawning, persistence, networking or world streaming. Existing gameplay actors remain authoritative; the component is presentation-only.

## Asset quality

Production source art must target maximum practical resolution and detail, clean silhouettes, consistent isometric perspective, coherent lighting direction, modular edges/pivots and visual consistency across families. Visual quality must not be reduced for convenience without technical justification.

## Materialization boundary

No `.uasset`, `.umap` or other Unreal binary was fabricated. Source profiles/components define how legitimate Paper2D assets can be materialized later.

Still deferred: production texture import, Sprite creation, real asset assignment, collision/navigation authoring, runtime composition, PIE, multiplayer, Dedicated Server and visual quality validation.

## Completion status

🟩 Repository/source/architecture complete for the declared Phase 6 scope.

🟥 Unreal materialization and runtime validation deferred by project decision.

Next: Phase 7 — 2D depth, lighting and shadows.
