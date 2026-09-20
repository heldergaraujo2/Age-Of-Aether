# Phase 50 — Crafting, Forge, Economy & Shops

## Status

**Complete at repository/source level. Unreal 5.8.1 runtime acceptance remains a local gate.**

## Implemented

- FAetherWallet and authoritative Gold balance handling.
- Overflow-safe currency add/remove.
- FAetherShopDefinition and shop entries with buy/sell permissions and transaction quantity limits.
- FAetherCraftRecipe with ingredients, outputs, currency cost, required level and enable flag.
- Forge/crafting station metadata through CraftingStationID.
- FAetherEconomyService for shop registration, recipe registration, wallet operations, buy, sell, craft, inventory capacity checks, ingredient consumption and output grants.
- UAetherEconomySubsystem as the Unreal GameInstance subsystem bridge.
- UAetherEconomyConfigDataAsset for data-only authoring.
- Transaction result contract with stable transaction IDs.
- Automation coverage for registration, wallets, overflow, buy/sell, crafting, missing ingredients and inventory-capacity rejection.
- Existing item/inventory services are reused; no second inventory system is created.

## Safety and authority

Economy mutation APIs are domain/service APIs intended to be invoked by the authenticated server path. UI and Blueprint should request operations; they must not author balances, item quantities or transaction results.

The client must never be trusted for price, balance, item ownership, ingredient quantity, output quantity or transaction result.

## Low-bureaucracy Unreal workflow

1. Create one UAetherEconomyConfigDataAsset.
2. Add shop rows and recipe rows.
3. Set CraftingStationID for forge/smith/alchemy/etc. when needed.
4. Reuse one generic shop UI and one generic crafting/forge UI.
5. Assign item IDs from the existing item registry.
6. Duplicate data rows for additional shops/recipes.

No new C++ class is required for each recipe, forge, merchant or item.

## First visual vertical slice

Use only one merchant NPC, one shop UI, one forge UI, two material item definitions, one crafted item and one shop transaction. This is enough to validate the entire content pipeline.

## Runtime acceptance gate

In Unreal 5.8.1: compile Editor/Game/Dedicated Server; create one economy config Data Asset; open the shop UI; buy an item; sell an owned item; craft one recipe at the forge; verify insufficient Gold; verify missing ingredients; verify inventory full; run two clients and Dedicated Server; verify only the server changes economy/inventory state.

No fake .uasset or .umap files are committed by this phase.
