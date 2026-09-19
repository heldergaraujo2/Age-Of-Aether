# AGE OF AETHER — PHASE 6: ITEM AND INVENTORY SYSTEM

## Status

**Repository implementation:** COMPLETE  
**Repository/static validation:** PASSED by source inspection  
**Unreal 5.8.1 compilation/runtime:** NOT VERIFIED in this environment  
**Persistence:** INTENTIONALLY NOT IMPLEMENTED; belongs to later persistence/backend phases

## Objective

Establish the first authoritative item pipeline:

- immutable item definitions;
- concrete item instances;
- CharacterID ownership;
- fixed inventory capacity;
- stacking;
- add/remove;
- move;
- split;
- merge;
- transactional item grants;
- data-driven Primary Data Asset contract;
- server-authoritative inventory RPC boundary;
- automation coverage.

## Implemented

### Item definitions

FAetherItemDefinition contains:

- stable ItemDefinitionId;
- internal/display identity;
- category;
- equipment slot;
- dimensions;
- maximum stack;
- base level;
- base durability;
- binding;
- class mask.

UAetherItemDefinitionDataAsset provides an Unreal UDataAsset representation so item content can be authored as data rather than hard-coded gameplay rules.

### Item instances

FAetherItemInstance contains:

- unique ItemInstanceId;
- ItemDefinitionId;
- OwnerCharacterId;
- quantity;
- item level;
- durability;
- binding.

An instance is separate from its definition.

### Inventory

FAetherInventorySlot represents a fixed 64-slot inventory.

The runtime service supports:

- deterministic slot indexing;
- stack accumulation;
- multi-stack grants;
- removal;
- movement into empty slots;
- stack splitting;
- stack merging;
- occupied-slot inspection.

Ownership is keyed by CharacterID, not AccountID.

### Transaction safety

AddItem operates on a working inventory copy and only commits the result after the entire grant can be completed.

This prevents a failed grant from partially mutating the inventory.

### Networking

AAetherNetworkPlayerController now exposes:

- RequestInventory;
- MoveInventoryItem;
- SplitInventoryStack;
- MergeInventoryStacks;
- DiscardInventoryItem.

All inventory mutations execute through server RPCs.

The server resolves the active CharacterID from authenticated controller state and replicated Character PlayerState. The client does not submit an authoritative CharacterID.

The client receives an authoritative inventory snapshot only after a server operation.

## Tests

Source/AgeOfAether/Private/Tests/AetherItemInventoryTests.cpp

Coverage:

1. definition registration;
2. duplicate definition rejection;
3. definition lookup;
4. stack accumulation;
5. multi-stack creation;
6. preservation of existing instance identity when stacking;
7. split creates a new instance;
8. merge restores quantity;
9. movement;
10. partial removal;
11. final removal;
12. CharacterID ownership isolation;
13. cross-character mutation rejection;
14. inventory capacity;
15. transactional failure with no partial grant.

## Validation truth

The repository-side implementation was inspected for:

- balanced C++ delimiters;
- service/subsystem separation;
- CharacterID ownership;
- server-only mutation path;
- no client-supplied authoritative ownership;
- transactional grant behavior;
- no persistence masquerading as durable storage.

Unreal-dependent validation remains NOT VERIFIED because Unreal Engine 5.8.1, UHT, UBT, Editor and the Automation Framework are not executable in the current environment.

No CI check exists that can substitute for that validation.

## Local acceptance procedure

When the Unreal environment is available:

1. Generate/update Visual Studio project files.
2. Run UHT.
3. Compile AgeOfAetherEditor.
4. Open Unreal Editor.
5. Run AgeOfAether.Item.* automation tests.
6. Create/register a test ItemDefinition Data Asset.
7. Register/load the definition through the item subsystem.
8. Authenticate a test account.
9. Select a character.
10. Grant test items from server-side test/admin code.
11. Request inventory from the client.
12. Verify the authoritative 64-slot snapshot.
13. Test stacking and multi-stack behavior.
14. Test move/split/merge/discard.
15. Attempt invalid quantities and invalid slots.
16. Attempt inventory operations against another character's ItemInstance.
17. Test network emulation with latency/loss/jitter.
18. Verify only the server changes inventory state.

## Explicit boundaries

Not implemented in Phase 6:

- persistent item repository;
- database writes;
- equipment runtime;
- item stat modifiers;
- randomized options;
- sockets;
- enhancement;
- durability consumption;
- loot/drop generation;
- shops;
- trade;
- crafting;
- warehouse;
- grid occupancy based on Width/Height;
- production distributed item ID allocation.

Those systems will consume this item/inventory foundation in later phases.

## Security rule

Never accept from the client:

- authoritative CharacterID;
- ItemInstance ownership;
- item quantity as trusted state;
- item definition authority;
- durability;
- binding;
- item level;
- generated options;
- currency value.

The client requests an operation; the server validates and mutates the authoritative runtime state.

## Next phase

PHASE 7 — Progression System

The progression phase will build level/experience/stat allocation on the existing Character foundation and must integrate with item/equipment requirements without moving authority to the client.
