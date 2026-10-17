# Phase 60 — Overworld, farming, equipment and VFX web slice

**Date:** 2026-10-17
**Scope:** playable Canvas/browser preview in `Preview/` only. This follow-up advances the presentation/gameplay prototype associated with Roadmap Phases 45, 47, 49 and 51; it does not close those Unreal acceptance gates.

## Delivered

- Added `AOA.Overworld.Aetherfall100`, a kilometer-based **100 km × 100 km** world map with a 10 km grid, terrain/biome regions, topography, a connected route network and a live player marker.
- Added ten teleport destinations: the two expanded safe cities **Aetherfall** and **Lunária**, plus eight hunting areas. The map has separate world, regional-detail and logical-cell views. Selecting a world destination and pressing **TELEPORTAR** changes the active destination, local spawn, world coordinates and active hunting camp; map clicks can also mark free waypoints.
- Every hunting destination declares **10 active mobs** and a **3-second individual respawn**. Mobs roam, aggro, pursue and attack; they are selectable/clickable and reuse animated creature atlas rows. Hunting kills grant XP and coins and can leave a visible, collectible item drop. Pet attacks can contribute damage.
- Reframed the two existing Elara/Oca quests as **optional journey content**. Quest kills and turn-ins award coins and/or items only. Farming mobs is the sole `grantXp` call path in this browser slice.
- Replaced the placeholder inventory list with catalog-backed items, equipment slots and equip/unequip actions. Weapon, armor, boots, cloak and ring stats affect combat/health/mana/movement. The equipped ember fox follows and attacks; equipped wings render in the scene and modify movement. Mob loot and quest rewards enter the same inventory.
- Added visible arcane-bolt/strike/healing VFX and an animated level-up burst/banner. The level-up effect is triggered by farming XP.
- Expanded both city scenes with a dozen varied buildings, gate/towers, a market stall, a fountain, guild/market signs and animated townsfolk, retaining the existing mage/Elara and creature sprite animation.
- Bumped the preview resource query version to `0.5.0` so the already-running preview server on port `4174` requests the updated JS/CSS.

## Data and files

- `Preview/world-map-data.js`: preserves the canonical first-region map contract and adds world destinations, farming metadata, terrain regions and the item catalog.
- `Preview/game.js`: farming/respawn/XP and loot, teleport handling, global map layers, inventory/equipment, pet/wings, city and VFX presentation.
- `Preview/index.html`, `Preview/styles.css`: responsive presentation and cache-busted `0.5.0` resources.
- `Tools/Map/test_first_region_map.js`: retains the nine-zone/12-cell regional contract and validates the 100 km overworld, two cities, eight ten-mob farms, three-second respawn values and equipment items.

## Validation performed

- `node --check Preview/game.js`
- `node --check Preview/world-map-data.js`
- `node Tools/Map/test_first_region_map.js`
- `git diff --check`
- Temporary JSDOM + Canvas-stub smoke: map/destination selection, teleport, ten active mobs, no respawn before three seconds and respawn afterwards, farming-only XP, no XP from quest kills/turn-in, a real quest item reward, equipment stat changes, spell VFX, level-up animation and preserved regional/logical-cell layers.
- The preview server on port `4174` continued to return the updated cache-busted entry point. A Canvas stub is not a real graphical browser and cannot establish final visual quality.

## Explicit acceptance boundary

No Unreal Editor, Unreal build, PIE, Paper2D import, multiplayer or Unreal Automation was executed for this slice. The 100 km coordinates are browser-preview coordinates, not Unreal transforms; regional destinations currently materialize the existing local Canvas scene at destination-specific spawn coordinates. Real `.umap`/`.uasset` creation, World Partition/Data Layers/streaming, full 100 km terrain traversal, actual Unreal inventory/equipment/pet/wing assets, server-authoritative XP/loot/teleport, multiplayer and runtime acceptance remain pending. No Unreal binary asset or Unreal PASS is claimed.
