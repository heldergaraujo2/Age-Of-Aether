# Phase 49 — Inventory, Loot & Equipment Gameplay

Complete at repository/source level; Unreal 5.8.1 runtime acceptance remains local.

Implemented:
- server-side character inventory state with stacking, capacity, split, remove and reset;
- equipment slot state with equip/unequip validation;
- stable item instance IDs;
- data-driven loot tables and loot rolls;
- inventory grant pipeline;
- Automation tests;
- no fake Unreal binary assets.

Visual integration remains intentionally simple: existing EquipmentVisualComponent consumes an item visual profile after authoritative equip state is known. New item content remains data + optional visual profile, not new character code.

Runtime acceptance:
- compile Editor/Game/Dedicated Server;
- register one item definition;
- add/split/remove it;
- equip/unequip it;
- connect its EquipmentVisualProfile and verify socket attachment;
- roll a loot table and grant it;
- test full inventory and invalid requests on server.
