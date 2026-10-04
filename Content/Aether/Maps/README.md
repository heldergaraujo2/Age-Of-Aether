# Maps

Unreal maps and world assets.

## First permanent world region

The canonical source/repository specification is:

- `Docs/PHASE_9_FIRST_PERMANENT_REGION.md`
- `Docs/FIRST_PLAYABLE_WORLD_REGION_PRODUCTION_SPECIFICATION.md`
- `Docs/FIRST_PLAYABLE_ISOMETRIC_DIORAMA_LAYOUT.md`
- `Docs/FIRST_PLAYABLE_ISOMETRIC_DIORAMA_ASSET_KIT.md`

The current official visual interpretation is **premium 2D isometric**. The historical first-region documents remain useful for permanent-world composition, spatial hierarchy and asset-family planning, while Phase 9 reconciles their older 3D/2.5D assumptions with the current 2D direction.

Canonical experience:

settlement -> plaza/services -> outskirts -> gate -> road -> countryside -> forest edge -> exploration -> combat frontier -> ruins/landmark -> dungeon approach -> dungeon entrance

The region is a permanent piece of the future single continuous world and is not a disposable test map.

## Source/content boundary

Existing `FAetherMapDefinition`, zone, streaming-cell, point, connection, actor-placement and interaction contracts define the repository-side content structure.

The actual Unreal map must be legitimately materialized in Unreal Engine. Repository source must not fabricate `.umap`, `.uasset` or other Unreal binaries.

## Runtime gate

Final map materialization, real visual asset assignment, collision, navigation, exploration, combat, multiplayer, PIE, Dedicated Server and visual-quality acceptance remain deferred until the dedicated Unreal validation window.
