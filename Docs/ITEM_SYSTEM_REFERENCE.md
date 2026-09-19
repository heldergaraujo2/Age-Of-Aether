# AGE OF AETHER — ITEM SYSTEM REFERENCE

## Purpose

This document translates the structural lessons observed in the supplied MU-style Item.txt into an original AGE OF AETHER design.

The source file is a reference. It is not an implementation dependency.

## Source observations

The supplied file uses numbered sections 0–15. Each section contains item records with a category/type context and a schema appropriate to the item family.

Common observed columns:

- Type
- Slot
- Skill
- Width
- Height
- HaveSerial
- HaveOption
- DropItem
- Name

Weapon schemas add:

- Level
- DamageMin
- DamageMax
- AttackSpeed
- Durability
- MagicDurability
- MagicDamageRate
- ReqLevel
- ReqStrength
- ReqDexterity
- ReqEnergy
- ReqVitality
- ReqLeadership
- class flags

Armor schemas add:

- Defense
- MagicDefense
- DefenseSuccessRate
- AttackSpeed/WalkSpeed depending on category
- requirements
- class flags

Other schemas include:

- BuyMoney
- Resistance1–Resistance7
- Value
- skill/scroll requirements

## AGE OF AETHER mapping

We will not create one giant struct containing every possible field.

Instead:

### Common definition

- DefinitionID
- Name
- Description
- Category
- SubCategory
- Tags
- Width
- Height
- MaxStack
- Flags
- Requirements
- AllowedClasses
- Slot
- VisualID
- IconID

### Equipment properties

- BaseDamageMin
- BaseDamageMax
- BaseDefense
- BaseMagicDefense
- AttackSpeed
- MoveSpeed
- Durability
- MagicPower
- DefenseRate
- WeaponHandedness

### Requirements

- Level
- Strength
- Dexterity
- Energy
- Vitality
- Leadership
- Class
- Quest
- Reputation
- Custom predicates

### Effects

- Stat modifiers
- Buffs
- Debuffs
- Proc effects
- OnEquip
- OnUse
- OnHit
- OnKill
- Passive effects

### Economy

- BuyPrice
- SellPrice
- CurrencyType
- VendorAvailability
- Tradeable
- Sellable
- Destroyable

### Drop

- DropEnabled
- DropTables
- MinimumMonsterLevel
- EventRestrictions
- OwnershipRules

## ItemDefinition vs ItemInstance

ItemDefinition is static content.

ItemInstance is runtime/persistent state.

Example:

ItemDefinition:
- Aether Sword
- BaseDamage 100–120
- MainHand
- RequiredLevel 20

ItemInstance:
- InstanceID 7F...
- DefinitionID AETHER_SWORD
- Enhancement +7
- Durability 83
- Rare
- Luck
- Generated options
- Owner Character X

## Validation

A definition validator must detect:

- duplicate IDs;
- missing names;
- invalid dimensions;
- invalid category;
- invalid slot;
- invalid class;
- invalid requirements;
- invalid option/effect references;
- invalid visual references;
- invalid drop references;
- invalid shop references;
- impossible values;
- schema incompatibilities.

## Future editor

The long-term goal is an Unreal/editor/content tool that lets a developer/designer create a new item by filling data rather than writing gameplay code.

The tool should eventually provide:

- item creation;
- ID generation/validation;
- category selection;
- stat editing;
- requirements;
- class restrictions;
- options;
- sockets;
- enhancement profile;
- drop profile;
- shop profile;
- visual references;
- tooltip preview;
- validation;
- export/commit workflow.

## Research note

Public MU sources show that item addition often requires server definitions plus client presentation and sometimes drop/shop/visual configuration. AGE OF AETHER will consolidate these concerns into a validated data pipeline while retaining strict server authority.

