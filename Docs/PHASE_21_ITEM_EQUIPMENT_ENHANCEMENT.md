# PHASE 21 — COMPLETE ITEM, EQUIPMENT & ENHANCEMENT DATA

## Status

**Repository implementation: COMPLETE.**

Phase 21 establishes the data-driven item contract on top of the Phase 19 content registry and Phase 20 visual asset registry.

## Implemented

- stable Item Definition IDs;
- item category and rarity;
- item level and character-level requirements;
- class restrictions;
- equipment slots for weapon/off-hand/shield/head/chest/legs/gloves/feet/rings/necklace/wings/mount/cosmetic;
- stack limits, weight and durability;
- binding/trading rules;
- drop/shop/craft eligibility;
- buy/sell economy values and currency ID;
- icon, world mesh and equipped visual asset IDs;
- base stats, options, effects and tags;
- separate enhancement-level records;
- enhancement materials and quantities;
- enhancement currency and cost;
- success chance;
- failure behavior: none/no-change/downgrade/destroy;
- protection item IDs;
- enhancement stat scaling;
- enhancement visual asset;
- ItemInstanceSnapshot separation from immutable ItemDefinition;
- duplicate detection, normalized lookup and deterministic enumeration;
- cross-reference validation against Phase 19 content and Phase 20 asset registries.

## Validation rules

The registry rejects empty IDs/names, negative levels, invalid stack limits, negative/non-finite weight, invalid durability ranges, invalid equipment-slot combinations, negative economy values, duplicate/out-of-range enhancement levels, success chances outside [0,1], negative enhancement currency, invalid material quantities, invalid stat scaling, empty stat/option keys and non-finite stat/option values.

When the Phase 19 and Phase 20 registries are supplied, the item registry also validates the item content definition and visual asset references.

## Architecture boundary

ItemDefinition is immutable design/content data. ItemInstanceSnapshot represents a concrete runtime item state.

Adding a new item is intended to require data registration rather than new gameplay code. The server remains authoritative for ownership, quantity, durability, enhancement outcome and economic mutation. The client resolves stable asset IDs through the visual asset pipeline.

## Tests

Automation coverage added for registration, normalized lookup, invalid stack/durability/weight, equipment slot/class requirements, enhancement chance/material/currency/failure behavior, duplicate IDs, deterministic ordering, Phase 19 content references and Phase 20 visual asset references.

## Validation truth

Repository/static inspection is performed for the Phase 21 source and contracts.

Unreal 5.8.1 UHT, UBT, Editor, Automation Framework and PIE were **NOT VERIFIED** because the Unreal toolchain is unavailable in this environment. No compilation success is claimed.

## Non-goals

Actual Unreal Data Assets/UAssets, runtime enhancement execution, inventory transactions, persistence/database storage, FBX import/rendering and client UI remain later/local-Unreal work.
