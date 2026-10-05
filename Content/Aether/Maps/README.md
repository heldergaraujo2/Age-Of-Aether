# Maps

Unreal maps and world assets.

## First permanent world region

The canonical source/repository specification is:

- `Docs/PHASE_9_FIRST_PERMANENT_REGION.md`
- `Docs/FIRST_PLAYABLE_WORLD_REGION_PRODUCTION_SPECIFICATION.md`
- `Docs/FIRST_PLAYABLE_ISOMETRIC_DIORAMA_LAYOUT.md`
- `Docs/FIRST_PLAYABLE_ISOMETRIC_DIORAMA_ASSET_KIT.md`

The current first-region visual target is a **bright, stylized 3D isometric diorama** based on the project owner's supplied reference: a readable castle landmark, river and bridge, clustered village, farms, paths, rolling green terrain and layered forest. Paper2D remains supported for character/effect content, but is not the primary environment representation. The earlier 2D direction document is retained as historical pipeline guidance.

Canonical experience:

settlement -> plaza/services -> outskirts -> gate -> road -> countryside -> forest edge -> exploration -> combat frontier -> ruins/landmark -> dungeon approach -> dungeon entrance

The region is a permanent piece of the future single continuous world and is not a disposable test map.

## Source/content boundary

Existing `FAetherMapDefinition`, zone, streaming-cell, point, connection, actor-placement and interaction contracts define the repository-side content structure.

The actual Unreal map must be legitimately materialized in Unreal Engine. Repository source must not fabricate `.umap`, `.uasset` or other Unreal binaries.

## Runtime gate

Final map materialization, real visual asset assignment, collision, navigation, exploration, combat, multiplayer, PIE, Dedicated Server and visual-quality acceptance remain deferred until the dedicated Unreal validation window.
