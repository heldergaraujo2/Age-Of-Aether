# PHASE 12 — ECONOMY & CRAFTING

## Status

Repository implementation: **COMPLETE**.
Repository/static validation: **PASSED**.
Unreal 5.8.1 compilation/runtime validation: **NOT VERIFIED** because Unreal is unavailable in this execution environment and no CI build exists.

## Scope

Phase 12 establishes the runtime economic foundation without introducing persistence:
- character-owned currency wallets;
- server-authoritative currency mutations;
- data-driven shops and fixed buy/sell prices;
- transactional shop buy/sell validation;
- data-driven crafting recipes;
- ingredient/output validation;
- crafting level and currency requirements;
- inventory capacity preflight before crafting output;
- economy transaction result contracts;
- Data Asset configuration boundary;
- server-authoritative wallet/shop/crafting RPCs;
- request ordering guards;
- automation coverage.

## Currency

The initial runtime currency is Gold, represented by int64.

The wallet is keyed by FAetherCharacterId, preserving the project rule that character-owned gameplay state is not implicitly account-owned.

Currency mutations are server-side. The client never supplies an authoritative balance.

Overflow is explicitly rejected on additions and transaction price calculations.

## Shops

A shop contains:
- stable ShopId;
- display name;
- item entries;
- buy price;
- sell price;
- per-transaction quantity limit;
- buy/sell enable flags.

Registration rejects:
- empty IDs;
- duplicate IDs;
- invalid item IDs;
- negative prices;
- invalid quantity limits;
- duplicate entries inside a shop.

Buy flow:
1. Server resolves the authenticated active character.
2. Server resolves the shop and item definition.
3. Server validates quantity and shop availability.
4. Server preflights inventory capacity.
5. Server validates sufficient Gold.
6. Server adds the item.
7. Server removes the Gold only after item creation succeeds.
8. Client receives an authoritative transaction result.

Sell flow:
1. Server resolves the authenticated active character.
2. Server resolves the item instance from that character's inventory.
3. Server derives the definition from the owned instance.
4. Server validates the shop's sell rule and quantity.
5. Server checks currency overflow before removing the item.
6. Server removes the item.
7. Server credits Gold.
8. Client receives the authoritative result.

## Crafting

A recipe contains:
- stable RecipeId;
- input ingredients;
- output items;
- currency type/cost;
- required character level;
- enabled state.

Registration rejects:
- empty IDs;
- duplicate IDs;
- empty ingredient/output sets;
- invalid item definitions;
- non-positive quantities;
- invalid level/currency cost.

Craft flow:
1. Server resolves the authenticated active character and authoritative level.
2. Server resolves and validates the recipe.
3. Server validates recipe enabled state and level requirement.
4. Server preflights every output against a simulated inventory.
5. Server validates all ingredient quantities.
6. Server validates currency cost.
7. Server consumes ingredients.
8. Server creates outputs.
9. Server consumes currency.
10. Client receives an authoritative transaction result.

The output preflight simulates all outputs together, including multiple outputs targeting the same stack, so crafting does not intentionally consume ingredients when the resulting inventory cannot hold the complete output.

## Security boundary

The client cannot:
- set its own wallet balance;
- select another character's wallet;
- choose another character's inventory;
- alter shop prices;
- alter recipe ingredients or outputs;
- bypass level requirements;
- bypass ingredient requirements;
- bypass inventory capacity;
- submit an arbitrary sell price;
- submit an arbitrary buy price;
- grant itself currency;
- authorize its own transaction result.

Account identity remains derived from the authenticated session and CharacterID from the active Character PlayerState.

## Data boundary

Added:
- FAetherShopDefinition
- FAetherShopEntry
- FAetherCraftRecipe
- FAetherCraftIngredient
- UAetherEconomyConfigDataAsset

The config asset registration path is atomic: all shop/recipe registrations are validated on a candidate service before replacing the live runtime registry.

No fake .uasset files were generated. Actual editor-authored Data Assets remain Unreal content work.

## Networking

AAetherNetworkPlayerController now exposes:
- RequestWallet
- BuyItem
- SellItem
- CraftItem

Each operation has:
- a dedicated request sequence;
- server RPC;
- replay/ordering guard;
- authoritative character resolution;
- client transaction delegate.

Blueprint presentation can consume OnEconomyTransaction without becoming the authority.

## Persistence boundary

Not implemented in Phase 12:
- database storage;
- crash recovery;
- durable transaction ledger;
- cross-server wallet synchronization;
- offline economic processing.

Those belong to the later persistence/backend phase.

## Tests

Added automation coverage for:
1. shop registration;
2. duplicate shop rejection;
3. recipe registration;
4. duplicate recipe rejection;
5. wallet set/add/remove behavior;
6. overspend rejection;
7. negative balance rejection;
8. shop buy;
9. shop sell;
10. oversell rejection;
11. crafting with ingredients and currency;
12. crafting output creation;
13. missing-ingredient rejection;
14. invalid recipe rejection;
15. currency validation;
16. overflow-safe economic mutations.

## Validation truth

Repository/static checks completed:
- source files inspected after implementation;
- balanced delimiters on Phase 12 source/tests;
- no literal escaped-newline artifacts in the audited Phase 12 and modified source files;
- Item Data Asset registration boundary repaired;
- Economy Data Asset registration made atomic;
- sell overflow is checked before inventory mutation;
- crafting output capacity is simulated across the complete output set;
- controller economy declarations and implementation names reconciled;
- no fake Unreal binary/content assets created.

Not executable here:
- Unreal Header Tool;
- Unreal Build Tool;
- C++ compilation against Unreal 5.8.1;
- Editor startup;
- PIE;
- multiplayer replication;
- Automation Framework execution;
- network emulation.

Therefore Phase 12 is **repository-complete and statically validated**, but Unreal runtime/build validation remains explicitly **NOT VERIFIED**.

## Next phase

**PHASE 13 — MULTIPLAYER & SERVER AUTHORITY**
