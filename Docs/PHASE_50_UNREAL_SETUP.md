# Phase 50 — Minimal Unreal Setup

1. Create one UAetherEconomyConfigDataAsset.
2. Add one merchant shop with one item.
3. Add one forge recipe with two material inputs and one output.
4. Assign CraftingStationID = forge.basic.
5. Reuse the generic shop/crafting UI when the UI phase wires it.
6. Test buy, sell and craft through the authenticated server path.

For visuals, use the existing NPC presentation and existing item/equipment visual profiles. No special Blueprint is required for each merchant or recipe.

Acceptance checks: insufficient Gold, missing ingredients, inventory full, invalid quantity, duplicate registration, two-client PIE and Dedicated Server.

The repository phase is source-complete; actual Unreal 5.8.1 compilation and runtime remain local acceptance work.
