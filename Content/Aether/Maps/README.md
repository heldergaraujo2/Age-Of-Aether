# Maps

Unreal maps and world assets.

## First playable world region

The canonical production specification is:
Docs/FIRST_PLAYABLE_WORLD_REGION_PRODUCTION_SPECIFICATION.md

Composition:
settlement nucleus -> plaza/services -> outskirts -> road -> natural transition -> wilderness -> exploration pockets -> combat territory -> dungeon approach/entrance

This is a permanent slice of the future single continuous world, not a disposable test map.

## Binary asset rule

Real Unreal binary assets such as .umap and .uasset must not be fabricated in source control. The actual map must be materialized and saved by Unreal Engine 5.8 using the existing project architecture and Phase 3 real-art pipeline.

Runtime acceptance remains pending until the map has been opened, explored and validated in the real Unreal environment.
