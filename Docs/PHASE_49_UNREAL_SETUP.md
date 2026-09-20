# Phase 49 — Minimal Unreal Setup

Use one test weapon and one armor item.

1. Define both in the existing item registry/catalog.
2. Create only the visual profiles needed for the two items.
3. Use the inventory subsystem to AddItem.
4. Equip the weapon/armor from Blueprint UI.
5. Feed the authoritative equipped item into the existing EquipmentVisualComponent.
6. Use one loot table to grant the same items.

Do not create a Blueprint per item. The visual profile is the only asset-side setup required.

Server authority:
- inventory/equipment/loot mutations belong on the authenticated server path;
- UI only requests operations and displays returned state.
