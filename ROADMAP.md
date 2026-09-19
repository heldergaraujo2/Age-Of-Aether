# AGE OF AETHER — MASTER ROADMAP
## MMORPG • Unreal Engine + C++ • Server-Authoritative • Data-Driven

**Repository:** https://github.com/heldergaraujo2/Age-Of-Aether  
**Technical project:** `AgeOfAether`  
**Status:** Phase 22 — Monster, NPC, Boss & AI Content (repository implementation complete; Unreal runtime validation pending local validation)  
**Source of truth:** This repository  
**Continuity file:** `PROJECT_MEMORY/00_CONTINUITY.md`

---



## Current execution phases

The repository uses the following execution sequence for implementation continuity:

- Phase 0 — Repository and Architecture Foundation — COMPLETE
- Phase 1 — Unreal Project Foundation — COMPLETE at repository level; local Unreal validation pending
- Phase 2 — Core Runtime — COMPLETE at repository level; local Unreal validation pending
- Phase 3 — Networking Foundation — COMPLETE at repository level; local Unreal validation pending
- Phase 4 — Accounts and Sessions — COMPLETE at repository level; local Unreal validation pending
- Phase 5 — Character Foundation — COMPLETE at repository level; local Unreal validation pending
- Phase 6 — Item and Inventory System — COMPLETE at repository level; local Unreal validation pending
- Phase 7 — Progression System — COMPLETE at repository level; local Unreal validation pending
- Phase 8 — Combat Runtime — COMPLETE at repository level; local Unreal validation pending
- Phase 9 — World Runtime — COMPLETE at repository level; local Unreal validation pending
- Phase 10 — Quest/Mission — COMPLETE at repository level; local Unreal validation pending
- Phase 11 — Social — COMPLETE at repository level; local Unreal validation pending
- Phase 12 — Economy & Crafting — COMPLETE at repository level; local Unreal validation pending
- Phase 13 — Multiplayer & Server Authority — COMPLETE at repository level; local Unreal validation pending
- Phase 14 — Persistence & Backend — COMPLETE at repository level; local Unreal persistence validation pending
- Phase 15 — Security & Anti-Cheat — COMPLETE at repository level; local Unreal security validation pending
- Phase 16 — MMORPG Scale & Dedicated Server — COMPLETE at repository level; local Unreal validation pending
- Phase 17 — AI/GPT Integration — COMPLETE at repository level; Unreal/provider validation pending local
- Phase 18 — Production & Live MMORPG — COMPLETE at repository level; Unreal runtime validation pending local
- Phase 19 — Universal Data Model & Content Registry — COMPLETE at repository level; Unreal runtime validation pending local
- Phase 20 — Asset Pipeline & Visual Asset Registry — COMPLETE at repository level; Unreal runtime validation pending local
- Phase 21 — Complete Item, Equipment & Enhancement Data — COMPLETE at repository level; Unreal runtime validation pending local
- Phase 22 — Monster, NPC, Boss & AI Content — NEXT
- Phase 13 — Multiplayer & Server Authority
- Phase 14 — Persistence & Backend
- Phase 15 — Security & Anti-Cheat
- Phase 16 — MMORPG Scale & Dedicated Server
- Phase 17 — AI/GPT Integration
- Phase 18 — Production & Live MMORPG

The numbered item-content sections later in this document are retained as design/content milestones. They do not override the current execution sequence above.

## Phase 9 — World Runtime

Repository implementation is COMPLETE.

Implemented:

- authoritative world zones;
- Safe, PvP, Event, Dungeon and City zone types;
- zone enable/disable state;
- minimum-level requirements;
- zone combat-policy metadata;
- deterministic spawn-point resolution;
- spawn minimum-level requirements;
- directed portal definitions;
- portal enable/disable state;
- portal minimum-level requirements;
- authoritative initial character spawn;
- authoritative portal transition;
- character CurrentZoneId and CurrentZoneType;
- CharacterService world-state commit boundary;
- replicated PlayerState world identity;
- server-side pawn teleport on accepted transition;
- authoritative GameState revision advancement;
- world configuration Data Asset;
- atomic world-definition loading;
- automation tests for configuration, definitions, spawns, portal rules, level rules, disabled zones, character spawn and transitions.

Security boundary:

- client cannot set world coordinates;
- client cannot set rotation;
- client cannot select an arbitrary spawn;
- client cannot bypass portal rules;
- client cannot bypass level requirements;
- server resolves the target spawn.

Unreal content boundary:

- no fake .umap assets;
- World Partition is the intended large-world system;
- actual map, Data Layer, HLOD, terrain, collision and NavMesh authoring require Unreal Editor and remain local/content-stage work.

Deliverable: Docs/PHASE_9_WORLD_RUNTIME.md

## Phase 11 — Social

Repository implementation is COMPLETE.

Implemented:
- Account-level friends and relationship requests;
- accept/reject/remove friend operations;
- block/unblock;
- character-based party membership;
- party leader, invitation, acceptance, leave, leader handoff and kick;
- party capacity;
- character-based guild membership;
- guild creation with normalized unique names;
- guild leader/officer/member roles;
- guild invitations, acceptance, role changes and leave rules;
- authoritative Local/Party/Guild/Whisper/System chat;
- server-side message length and cooldown validation;
- server-side blocked-account protection;
- server-side recipient routing;
- `FAetherSocialService`;
- `UAetherSocialSubsystem`;
- `UAetherSocialConfigDataAsset`;
- social PlayerController RPCs and Blueprint delegates;
- social automation tests.

Security boundary:
- AccountID comes from authenticated session state;
- CharacterID comes from active Character PlayerState;
- clients cannot self-assign party/guild roles;
- clients cannot bypass membership or block rules;
- chat delivery is decided by the server.

External online-service boundary:
- no EOS/Online Services dependency was introduced into core gameplay;
- future provider integration can adapt Unreal Online Services Social/Sessions interfaces without replacing the authoritative social domain.

Validation truth:
- repository/static validation PASSED;
- Unreal 5.8.1 UHT/UBT/Editor/PIE/network emulation/Automation Framework remain NOT VERIFIED;
- no CI pipeline exists to substitute for local Unreal validation.

Deliverable:
- `Docs/PHASE_11_SOCIAL.md`

Next implementation target:
**PHASE 12 — Economy & Crafting**

## Phase 10 — Quest/Mission

Repository implementation is COMPLETE.

Implemented:
- data-driven quest definition contract and Data Asset;
- quest IDs, lifecycle states and objective types;
- level and prerequisite requirements;
- character-owned quest state;
- active quest capacity limit;
- server-authoritative accept, abandon and completion operations;
- objective progression by explicit objective ID;
- objective progression by authoritative target/type events;
- required versus optional objective semantics;
- idempotent reward-grant state guard;
- reward contract carrying XP and item definitions/quantities;
- GameInstance quest subsystem boundary;
- server RPCs for quest list/accept/abandon/complete;
- authenticated AccountID and active CharacterID ownership enforcement;
- request replay/ordering guard;
- automation tests for definitions, registration, lifecycle, progression, prerequisites, security, capacity and optional objectives.

Security boundary:
- client cannot submit authoritative CharacterID or AccountID;
- client cannot advance objectives directly;
- objective progress is intended to be driven by server-side combat/world/inventory systems;
- quest completion is accepted only from active server state with all required objectives complete;
- reward-granted state prevents duplicate completion rewards.

Data boundary:
- quest definitions are content data;
- runtime quest state is character-owned runtime state;
- persistence of quest state belongs to the later persistence/backend phase;
- no fake Unreal content assets are generated without the editor.

Validation truth:
- repository/static inspection performed after implementation;
- Unreal 5.8.1 UHT/UBT/Editor/PIE/network emulation/Automation Framework are NOT VERIFIED in this environment;
- no CI pipeline exists to substitute for local Unreal validation.

Deliverable:
- Docs/PHASE_10_QUEST_MISSION.md

Next implementation target:
**PHASE 11 — Social**

## Phase 7 — Progression System

Repository implementation is COMPLETE.

Implemented:

- authoritative experience and level progression;
- bounded data-driven XP curve;
- maximum level 4000 by default;
- multi-level advancement and rollover XP;
- stat points per level;
- Strength, Agility, Vitality, Energy and Command allocation;
- configurable stat cap;
- derived-stat recalculation;
- CharacterID/AccountID ownership validation;
- disabled/deleted lifecycle protection;
- replicated level, XP, stat points, base stats and derived stats;
- progression subsystem and CharacterService commit boundary;
- progression Data Asset configuration contract;
- server-authoritative stat allocation RPC;
- automation tests for curve, level-up, rollover, multi-level progression, stat allocation, cap, invalid input and lifecycle.

Deliverable: Docs/PHASE_7_PROGRESSION.md

Local Unreal validation remains PENDING and is not represented as PASS.


## 1. Purpose of this roadmap

This document is the master execution roadmap for AGE OF AETHER.

The project is intended to become a persistent MMORPG with:

- Unreal Engine client;
- C++ gameplay/server foundation;
- Blueprint for presentation, composition and selected content workflows;
- server-authoritative gameplay;
- persistent character and item data;
- modular inventory and equipment;
- data-driven item definitions;
- extensible events, quests, skills, monsters, NPCs and rewards;
- transactional economy/trade;
- security and anti-cheat validation;
- persistence/database abstraction;
- dedicated-server readiness;
- automated testing;
- observability and administration;
- future multi-server/scaling capability.

The project must **not** be built as a collection of disconnected Blueprint prototypes.

The architecture must make it possible to add content without rewriting core systems.

---

# 2. Foundational architectural principle

AGE OF AETHER will use three complementary layers.

## 2.1 C++ — rules, authority and engine

C++ is the authoritative implementation for critical systems:

- server runtime;
- service lifecycle;
- networking foundation;
- session state;
- account/character authority;
- item validation;
- inventory operations;
- equipment;
- stats;
- combat;
- skills/effects;
- drops;
- rewards;
- quests;
- economy;
- trade;
- persistence;
- security;
- anti-cheat;
- transactions;
- audit;
- server-side simulation;
- automated tests.

Blueprint must not become the final authority for critical persistent data.

## 2.2 Blueprint — presentation and composition

Blueprints will be used where Unreal's visual workflow provides value:

- UI;
- widgets;
- animations;
- VFX;
- SFX;
- visual item presentation;
- character presentation;
- NPC presentation;
- event presentation;
- interactable composition;
- designer-friendly orchestration;
- non-critical presentation logic.

Blueprint can expose safe C++ APIs, but server authority remains in C++.

## 2.3 Data — content, not rules

Content will be data-driven.

Examples:

- ItemDefinition;
- MonsterDefinition;
- SkillDefinition;
- QuestDefinition;
- RewardDefinition;
- EventDefinition;
- DropTable;
- ClassDefinition;
- MapDefinition;
- NPCDefinition;
- RecipeDefinition;
- EnhancementDefinition;
- Buff/Debuff Definition.

The target model is:

**C++ = how the game works**  
**Data = what exists in the game**  
**Blueprint = how it is presented/composed**

---

# 3. Reference analysis — supplied Item.txt

The supplied `Item.txt` is treated as a **reference for data organization and game-design concepts**, not as code to copy.

The file identifies itself as part of 3DServers MuOnline Server files and contains the 3DServers copyright header. Therefore AGE OF AETHER will not copy proprietary implementation, code, assets, protocol, client files, maps or protected game content from it.

The useful part for our architecture is the observed **data model**: categorized item records, IDs, dimensions, serial/option/drop flags, names, level and combat attributes, requirements, class restrictions, equipment slots and specialized schemas.

The supplied file contains 16 numbered item sections, 0 through 15, with different column schemas.

Observed record counts in the supplied file:

| Section | Observed records | Reference role |
|---:|---:|---|
| 0 | 96 | Sword/weapon family |
| 1 | 11 | Axe family |
| 2 | 33 | Mace/scepter family |
| 3 | 19 | Spear/polearm family |
| 4 | 52 | Bow/ranged family |
| 5 | 69 | Staff family |
| 6 | 43 | Shield family |
| 7 | 161 | Helm family |
| 8 | 182 | Armor family |
| 9 | 182 | Pants family |
| 10 | 162 | Gloves family |
| 11 | 182 | Boots family |
| 12 | 240 | Wings family |
| 13 | 275 | Secondary-unit/pet/resistance-oriented records |
| 14 | 278 | Consumable/miscellaneous records |
| 15 | 38 | Skill scroll/parchment records |

These counts are an audit snapshot of the supplied file and are not an AGE OF AETHER content requirement.

The file contains repeated base columns such as:

- Type
- Slot
- Skill
- Width
- Height
- HaveSerial
- HaveOption
- DropItem
- Name

Weapon-oriented sections additionally expose fields such as:

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

Defensive sections expose variants such as:

- Defense
- MagicDefense
- DefenseSuccessRate
- AttackSpeed or WalkSpeed depending on equipment type
- requirements
- class flags

Other sections introduce:

- BuyMoney
- Resistance1–Resistance7
- Value
- specialized requirements

The supplied file therefore confirms an important architectural requirement:

> A single rigid item structure is not sufficient for every item type.

AGE OF AETHER will solve this through a common item definition plus modular stat/behavior components.

---

# 4. What we learn from MU item configuration research

Research into public MU Online server configuration confirms that item creation commonly spans more than one data layer.

Public documentation and community/server-file examples show patterns including:

- server-side item definitions;
- client-side item definitions;
- item IDs composed from category/section and index in some implementations;
- class restrictions;
- item requirements;
- item options;
- item drops;
- NPC/shop configuration;
- item models/textures;
- tooltip/name presentation;
- synchronization between server and client data.

For example, public MU server configuration examples expose the same family of fields seen in the supplied file, including Type, Slot, Skill, Width, Height, serial/option/drop flags, name, damage, speed, durability and class restrictions.

Public guides also describe adding an item as a multi-step process involving server definitions, drop configuration, client definitions and visual assets rather than a single file edit.

This is the concept we will adopt — **not the legacy implementation**.

AGE OF AETHER will replace the old fragmented workflow with one authoritative item/content pipeline.

---

# 5. Target AGE OF AETHER item architecture

## 5.1 ItemDefinition

Immutable/game-design definition.

Conceptual fields:

- ItemID
- InternalName
- DisplayName
- Description
- Category
- SubCategory
- EquipmentSlot
- Width
- Height
- ItemClass
- ItemTags
- MaxStack
- BaseLevel
- BaseDurability
- Requirements
- AllowedClasses
- BindingRules
- TradeRules
- DropRules
- EconomyRules
- EnhancementRules
- VisualDefinitionID
- IconDefinitionID
- EffectDefinitionID
- StatDefinition
- OptionDefinition
- SocketDefinition
- GameplayFlags

The final C++ representation will be decided after the Unreal project baseline is created.

## 5.2 ItemInstance

A concrete object owned by a player.

Conceptual fields:

- InstanceID
- DefinitionID
- OwnerCharacterID
- CurrentLocation
- Quantity
- ItemLevel
- Durability
- Quality
- Rarity
- EnhancementLevel
- Luck
- Binding
- GeneratedOptions
- Sockets
- CustomData
- CreationTimestamp
- ModificationTimestamp

An ItemDefinition says **what the item type is**.

An ItemInstance says **which actual item exists in the world/player inventory**.

This separation is mandatory for persistent MMORPG behavior.

---

# 6. Item identity

We will not blindly copy MU's category/index model.

AGE OF AETHER will use an explicit stable identifier strategy.

Recommended conceptual identity:

`ItemDefinitionID` = globally unique definition identifier.

`ItemInstanceID` = globally unique runtime/persistent instance identifier.

The final representation may use GUIDs, 64-bit IDs, structured IDs, or a hybrid depending on persistence/network constraints.

Requirements:

- no accidental collisions;
- deterministic lookup;
- safe serialization;
- database compatibility;
- replication compatibility;
- audit compatibility;
- future sharding compatibility.

---

# 7. Item categories

The initial taxonomy will support at least:

### Equipment
- Weapons
- Off-hand
- Helm
- Chest
- Legs
- Gloves
- Feet
- Back
- Ring
- Necklace
- Mount
- Pet/Companion

### Consumables
- HP
- MP
- Stamina
- Buff consumables
- Recovery
- Temporary items

### Materials
- Crafting materials
- Enhancement materials
- Event materials
- Quest materials

### Skill items
- Scrolls
- Books
- Skill unlock items

### Quest items

### Currency-related items

### Cosmetic items

### Event items

### Containers

### Special/system items

The taxonomy remains extensible.

---

# 8. Equipment system

Target equipment slots:

- Head
- Chest
- Legs
- Hands
- Feet
- MainHand
- OffHand
- Back
- Ring
- Necklace
- Mount
- Pet

Equipment rules will validate:

1. Item exists.
2. Item instance exists.
3. Item belongs to the requesting character.
4. Item is in an allowed inventory location.
5. Character is alive/allowed to equip.
6. Slot is compatible.
7. Class requirement is satisfied.
8. Level requirement is satisfied.
9. Stat requirements are satisfied.
10. Quest/event restrictions are satisfied.
11. Binding/trade state allows operation.
12. Existing equipment conflict is handled atomically.
13. Derived stats are recalculated.
14. Persistence is updated.
15. Audit event is generated.
16. Client receives authoritative result.

---

# 9. Inventory system

Inventory must support:

- add;
- remove;
- move;
- swap;
- split;
- merge;
- stack;
- unstack;
- use;
- equip;
- unequip;
- drop;
- pickup;
- destroy;
- trade;
- sell;
- buy;
- quest consumption;
- crafting consumption;
- event consumption.

The inventory must be server-authoritative.

The client may request:

`MoveItem(source, destination)`

The server decides whether the operation is legal.

Never:

`Client says "I now own item X"`

---

# 10. Grid inventory

Because the supplied reference includes Width and Height, AGE OF AETHER will support a grid-based inventory model.

Example:

`Sword = 1x4`

`Armor = 2x3`

`Potion = 1x1`

The actual dimensions will be data-driven.

The inventory service will calculate occupied cells and reject overlaps.

The UI will visualize the authoritative layout.

---

# 11. Item options

The system will not hard-code every option into ItemDefinition.

Instead:

`ItemDefinition`

+

`OptionDefinition`

+

`ItemInstance.GeneratedOptions`

This supports:

- bonus Strength;
- bonus Agility;
- bonus Vitality;
- bonus Energy;
- bonus HP;
- bonus MP;
- attack;
- defense;
- attack speed;
- critical chance;
- critical damage;
- elemental effects;
- resistances;
- life steal;
- mana steal;
- skill modifiers;
- custom effects.

New option types should be added without rewriting inventory/equipment.

---

# 12. Enhancement system

Target:

- +0 to configurable maximum;
- success probability;
- failure;
- downgrade;
- destruction;
- preservation;
- materials;
- currency;
- pity/guarantee rules if desired;
- event modifiers;
- safe-zone restrictions.

All rates must be data-driven.

The enhancement service must be transactional.

---

# 13. Drop system

Architecture:

`Monster`

→ `DropTable`

→ `DropRule`

→ `Roll`

→ `ItemGeneration`

→ `ItemInstance`

→ `Inventory/LootWorld`

Support:

- weighted drops;
- guaranteed drops;
- rare drops;
- conditional drops;
- level restrictions;
- class restrictions;
- event modifiers;
- party contribution;
- ownership rules;
- anti-duplication controls.

---

# 14. Shops and economy

Item data must be reusable by:

- NPC shops;
- player shops;
- marketplace;
- rewards;
- quests;
- events;
- drops;
- crafting;
- trade.

Economy operations require:

- transaction ID;
- source;
- destination;
- amount;
- currency;
- character/account;
- timestamp;
- result;
- audit record.

---

# 15. Persistence

Persistent data includes:

- accounts;
- characters;
- stats;
- inventory;
- ItemInstances;
- equipment;
- warehouse;
- currency;
- quests;
- achievements;
- guilds;
- parties;
- mail;
- transactions;
- audit records.

Architecture:

`Gameplay`

→ `Application Services`

→ `Repositories`

→ `Persistence`

→ `Database`

No gameplay class should directly embed database implementation details.

---

# 16. Server authority

Critical item operations are always validated by the server.

Protected operations include:

- create item;
- destroy item;
- move item;
- equip;
- unequip;
- use;
- trade;
- sell;
- buy;
- drop;
- pickup;
- enhancement;
- option generation;
- socket modification;
- reward;
- quest completion;
- event reward.

The client is never trusted for:

- item ownership;
- item value;
- item level;
- rarity;
- options;
- durability;
- currency;
- reward;
- damage;
- cooldown;
- movement;
- trade state.

---

# 17. Unreal implementation strategy

## C++ modules

Initial conceptual modules:

`Core`
`World`
`Characters`
`Items`
`Inventory`
`Equipment`
`Combat`
`Skills`
`Progression`
`Quests`
`Social`
`Economy`
`Events`
`Persistence`
`Security`
`Administration`
`Testing`

Modules will be created when their phase begins. We will not create hundreds of empty classes just to make the tree look complete.

## Blueprint

Blueprint assets will consume safe C++ APIs.

Examples:

- `BP_ItemVisual`
- `WBP_Inventory`
- `WBP_ItemTooltip`
- `BP_NPC`
- `BP_EventPresentation`

Names are provisional.

---

# 18. Data representation

We will evaluate:

### Primary Data Assets
Good for rich individual definitions and inheritance.

### Data Tables
Good for structured bulk content and spreadsheet-like authoring.

### Config/JSON
Good for server/runtime configuration and external tooling where appropriate.

### Database
For persistent player-owned state, not static game design definitions unless a later content-management architecture requires it.

A likely final solution will be hybrid.

---

# 19. Future Item Editor

One major project objective is an internal content pipeline that makes item creation practical.

Target workflow:

1. Designer creates ItemDefinition.
2. Tool validates ID.
3. Tool validates category.
4. Tool validates dimensions.
5. Tool validates requirements.
6. Tool validates class restrictions.
7. Tool validates equipment slot.
8. Tool validates option references.
9. Tool validates visual asset references.
10. Tool validates drop/shop references.
11. Tool reports errors/warnings.
12. Content is committed to Git.
13. Server loads validated definitions.
14. Client loads matching visual data.
15. Automated tests verify the item.
16. Item becomes available in-game.

Long-term goal:

**Adding a new item should not require modifying core inventory C++ code.**

---

# 20. Item pipeline

The canonical pipeline will be:

`Item Definition`
→ `Validation`
→ `Content Registry`
→ `Server Runtime`
→ `ItemInstance Generation`
→ `Inventory/Equipment`
→ `Persistence`
→ `Replication`
→ `Client UI/Visuals`

---

# 21. Full MMORPG roadmap

## PHASE 0 — Repository and architecture foundation

Deliver:

- repository organization;
- Unreal version decision;
- source-of-truth policy;
- Git strategy;
- continuity system;
- architecture documentation;
- coding standards;
- naming conventions;
- branch strategy;
- test strategy;
- asset policy;
- licensing/IP policy.

Acceptance:

- new agent can understand project from repository documentation.

---

## PHASE 1 — Unreal project foundation

Deliver:

- clean Unreal project;
- C++ project/module;
- Config;
- Source;
- Content;
- Plugins;
- Tests;
- Docs;
- Project Memory;
- proper ignore rules;
- optional Git LFS for large binary assets;
- reproducible local setup.

Acceptance:

- project opens;
- C++ compiles;
- baseline map loads;
- clean repository clone can reproduce the project.

---

## PHASE 2 — Core Runtime

**Repository implementation status:** COMPLETE  
**Local Unreal validation:** PENDING

Implemented repository-side:
- runtime lifecycle and health states;
- server clock;
- scheduler;
- configuration;
- service registry;
- core logging;
- Unreal runtime subsystem;
- core automation tests.

Validation that requires Unreal Engine 5.8.1, UHT/UBT and the local Editor remains pending and is not represented as PASS.

Deliverable: Docs/PHASE_2_CORE_RUNTIME.md


Deliver:

- runtime state;
- lifecycle;
- server clock;
- scheduler;
- timer abstraction;
- logging;
- configuration;
- service registry;
- error handling;
- health state.

Acceptance:

- runtime initializes/shuts down cleanly.

---

## PHASE 3 — Networking foundation

Deliver:

- client/server separation;
- request/response foundation;
- replication strategy;
- authoritative server state;
- network validation;
- protocol/version abstraction.

Acceptance:

- client can connect to server and receive authoritative state.

---

## PHASE 4 — Accounts and sessions

Deliver:

- AccountID;
- authentication abstraction;
- session;
- login;
- logout;
- reconnect;
- timeout;
- heartbeat;
- permissions;
- account status.

Never store plaintext passwords.

---

## PHASE 5 — Character foundation

**Repository implementation status:** COMPLETE  
**Local Unreal/network validation:** PENDING

Implemented repository-side:
- CharacterID identity contract;
- account-to-character ownership;
- normalized unique character names;
- configurable character-per-account limit (5);
- character classes: Warrior, Mage, Archer, Cleric;
- character lifecycle states;
- base stats;
- derived stats;
- authoritative world location/rotation;
- character creation;
- character listing;
- character selection;
- character deselection;
- active-character ownership protection;
- server-authoritative character PlayerState;
- replicated character identity;
- replicated character pawn;
- authoritative character spawn after selection;
- character location persistence in the runtime service;
- logout cleanup of the active character;
- automation coverage for creation, names, ownership, limits, selection, location authority and listing.

Unreal integration:
- AAetherCharacter derives from ACharacter;
- AAetherCharacterPlayerState carries replicated character identity visible to clients;
- AAetherNetworkGameMode uses the character pawn and character PlayerState;
- players start as spectators and receive a character pawn only after server-side authentication and character selection;
- character operations are exposed through server RPCs on AAetherNetworkPlayerController.

Security boundary:
- the client requests creation/selection;
- the server validates authenticated account ownership and character state;
- the client cannot select another account's character;
- the client cannot choose authoritative character ownership or lifecycle state;
- location is written to the character service only through server-side operations.

Persistence boundary:
- character data is runtime-only by design;
- database persistence, migrations, durable character storage and crash recovery remain later backend/persistence work.

Acceptance:
- repository implementation and automation coverage complete;
- Unreal UHT/UBT, Editor, PIE multiplayer, possession, replication, movement and logout cleanup require local Unreal 5.8.1 validation.

Deliverable: Docs/PHASE_5_CHARACTER_FOUNDATION.md

## PHASE 6 — Progression

Deliver:

- level;
- XP;
- MasterLevel;
- XP calculator;
- stat points;
- reward service;
- centralized progression rules.

---

## PHASE 6 — Item and Inventory System

**Repository implementation status:** COMPLETE  
**Local Unreal validation:** PENDING

Implemented repository-side:

- FAetherItemDefinition and stable definition identity;
- FAetherItemInstance and separate instance identity;
- CharacterID-owned inventory state;
- fixed 64-slot inventory foundation;
- stack accumulation and multi-stack grants;
- remove;
- move;
- split;
- merge;
- transactional item grants with no partial mutation on capacity failure;
- Primary Data Asset contract for item definitions;
- inventory GameInstance subsystem boundary;
- server-authoritative inventory RPCs;
- authoritative inventory snapshots to the owning client;
- automation tests for definitions, stacking, mutations, ownership isolation and transactional capacity failure.

Security boundary:

- client requests inventory operations;
- server resolves the authenticated active CharacterID;
- server validates item ownership through CharacterID-scoped inventory state;
- client cannot submit authoritative ownership or item state;
- item persistence remains deferred to later persistence/backend phases.

Explicitly deferred:

- equipment;
- item modifiers/options;
- sockets/enhancement;
- drops/loot;
- shops/trade/crafting;
- persistent item repository/database;
- production grid occupancy based on Width/Height.

Deliverable: Docs/PHASE_6_ITEM_INVENTORY.md

## PHASE 7 — Item Definition Registry

This is the first major item milestone.

Deliver:

- ItemDefinition;
- ItemDefinitionID;
- categories;
- subcategories;
- tags;
- item registry;
- validation;
- Data Asset/Table strategy;
- class restrictions;
- requirements;
- equipment slots;
- dimensions;
- flags;
- serialization.

Acceptance:

- server can load item definitions and validate them.

---

## PHASE 8 — Item Instance system

Deliver:

- unique ItemInstanceID;
- owner;
- location;
- quantity;
- durability;
- rarity;
- quality;
- enhancement level;
- options;
- sockets;
- binding;
- custom data.

Acceptance:

- two instances of the same definition can have different state.

---

## PHASE 9 — Inventory

Deliver:

- grid;
- dimensions;
- stacking;
- splitting;
- moving;
- swapping;
- merging;
- pickup;
- drop;
- destroy;
- use;
- validation;
- UI.

Acceptance:

- all inventory operations are authoritative and transactional.

---

## PHASE 10 — Equipment

Deliver:

- equipment slots;
- requirements;
- class validation;
- equip/unequip;
- two-handed handling;
- derived stat recalculation;
- visual representation.

---

## PHASE 11 — Options and item effects

Deliver:

- OptionDefinition;
- option generation;
- stat modifiers;
- conditional effects;
- item effects;
- stacking rules;
- conflicts;
- recalculation pipeline.

---

## PHASE 12 — Enhancement

Deliver:

- +levels;
- success/failure;
- downgrade;
- destruction;
- preservation;
- materials;
- currency;
- configurable rates;
- audit.

---

## PHASE 13 — Sockets / special item systems

Deliver:

- sockets;
- socket materials;
- socket effects;
- unique item mechanics;
- binding;
- event-specific properties.

---

## PHASE 14 — Drop Engine

Deliver:

- DropTable;
- DropRule;
- weighted selection;
- guaranteed drops;
- rare drops;
- conditional drops;
- loot ownership;
- item generation;
- anti-duplication.

---

## PHASE 15 — World

Deliver:

- maps;
- zones;
- portals;
- spawn system;
- safe zones;
- PvP zones;
- event zones;
- weather/world rules.

---

## PHASE 16 — NPC and Monster systems

Deliver:

- NPC definitions;
- capability system;
- shops;
- quests;
- teleport;
- banking;
- crafting;
- monster definitions;
- AI states;
- aggro;
- respawn.

---

## PHASE 17 — Combat

Deliver:

- attack validation;
- target validation;
- range;
- cooldown;
- accuracy;
- damage;
- defense;
- resistance;
- critical;
- effects;
- death;
- rewards;
- combat logs.

Central pipeline:

`Request`
→ `Validate`
→ `Target`
→ `Range`
→ `Cooldown`
→ `Accuracy`
→ `Damage`
→ `Defense`
→ `Resistance`
→ `Critical`
→ `Effects`
→ `HP/Shield`
→ `Death`
→ `Reward`
→ `Audit`

---

## PHASE 18 — Skills and effects

Deliver:

- skill definitions;
- skill requirements;
- cooldown;
- resource cost;
- targeting;
- buffs;
- debuffs;
- periodic effects;
- dispel;
- immunity;
- stacking.

---

## PHASE 19 — Quests and rewards

Deliver:

- quest definitions;
- objectives;
- state machine;
- progress;
- rewards;
- prerequisites;
- branching;
- repeatability.

---

## PHASE 20 — Party and guild

Deliver:

- party;
- invitations;
- roles;
- loot rules;
- XP distribution;
- guild;
- ranks;
- permissions;
- guild storage;
- guild events;
- guild wars.

---

## PHASE 21 — Economy and trade

Deliver:

- currency;
- NPC shop;
- player shop;
- marketplace abstraction;
- transactional trade;
- buy/sell;
- economic audit;
- duplication prevention.

Trade pipeline:

`Request`
→ `Accept`
→ `Lock`
→ `Validate`
→ `Confirm`
→ `Transaction`
→ `Commit`
→ `Audit`

---

## PHASE 22 — Crafting

Deliver:

- recipes;
- inputs;
- requirements;
- success/failure;
- outputs;
- stations;
- profession rules;
- economy integration.

---

## PHASE 23 — Events

Deliver:

- event framework;
- schedules;
- participation;
- event zones;
- objectives;
- contribution;
- rewards;
- event bosses;
- phases;
- enrage;
- event-specific loot.

---

## PHASE 24 — Master progression

Deliver:

- MasterLevel;
- achievements;
- titles;
- rankings;
- resets/rebirth if desired;
- long-term progression.

---

## PHASE 25 — Social

Deliver:

- friends;
- ignore;
- chat;
- notifications;
- mail;
- item/currency mail;
- expiration;
- read state.

---

## PHASE 26 — Pets, mounts and companions

Deliver:

- pets;
- companions;
- summons;
- mounts;
- progression;
- abilities;
- equipment where applicable.

---

## PHASE 27 — Security and anti-cheat

Validate:

- movement;
- speed;
- attack;
- damage;
- cooldown;
- inventory;
- currency;
- skills;
- teleport;
- trade;
- rewards;
- packets;
- rate limits;
- impossible states.

Add:

- security logs;
- suspicious-action detection;
- server-side invariants;
- audit trail.

---

## PHASE 28 — Persistence hardening

Deliver:

- repositories;
- transactional writes;
- save policies;
- logout saves;
- periodic saves;
- critical-operation saves;
- crash recovery;
- backup strategy;
- rollback;
- consistency checks.

Never save the entire character every frame.

---

## PHASE 29 — Dedicated server

Deliver:

- dedicated-server target;
- no rendering;
- no client-only UI;
- server startup;
- configuration;
- deployment;
- health checks;
- logging.

---

## PHASE 30 — Load and stress testing

Test:

- players;
- inventory operations;
- combat;
- AI;
- drops;
- trades;
- persistence;
- database latency;
- network traffic;
- server tick;
- memory;
- CPU.

---

## PHASE 31 — Optimization

Optimize only after measurement.

Targets:

- CPU;
- RAM;
- network;
- replication;
- database;
- AI;
- ticks;
- allocations;
- content loading;
- asset streaming.

---

## PHASE 32 — Production readiness

Deliver:

- deployment pipeline;
- versioning;
- migrations;
- backups;
- monitoring;
- alerting;
- administration;
- GM tools;
- incident recovery;
- security review;
- load validation;
- release checklist.

---

# 22. Long-term multi-server architecture

The architecture must allow future:

- Login Server;
- Gateway;
- World Server;
- Game Server;
- Chat Server;
- Instance Server;
- Matchmaking;
- regional servers;
- channels;
- sharding;
- services;
- database replicas;
- caching;
- queues.

We will not prematurely implement all of these.

We will preserve the boundaries that make them possible.

---

# 23. Administration / GM

Roles:

- PLAYER
- VIP
- MODERATOR
- GM
- ADMIN
- OWNER

Permissions must be granular.

Every administrative action must be auditable.

Potential commands:

- inspect player;
- grant item;
- remove item;
- grant currency;
- teleport;
- spawn monster;
- start event;
- cancel event;
- mute;
- kick;
- ban;
- inspect audit;
- inspect server health.

---

# 24. Testing strategy

Testing begins with the foundation.

## Unit

- item validation;
- inventory;
- equipment;
- requirements;
- stat calculation;
- drop rolls;
- reward calculation;
- trade state machine.

## Integration

- character + inventory;
- inventory + persistence;
- item + equipment;
- monster + drop;
- combat + rewards;
- trade + economy.

## Security

- forged item IDs;
- forged ownership;
- duplicated requests;
- replay;
- invalid quantities;
- negative values;
- impossible movement;
- cooldown bypass;
- trade race conditions.

## Load

- simultaneous players;
- inventory activity;
- combat activity;
- events;
- persistence.

---

# 25. Documentation and continuity

Every major implementation must update:

- README;
- roadmap status;
- current state;
- decisions;
- tests;
- continuity/handoff file.

The canonical continuity file is:

`PROJECT_MEMORY/00_CONTINUITY.md`

Another agent must be able to start from that file and understand:

- what AGE OF AETHER is;
- repository;
- architecture;
- current phase;
- completed work;
- tests;
- known failures;
- pending work;
- decisions;
- risks;
- exact next step;
- relevant files;
- commands required for validation.

---

# 26. Mandatory status format

After every major stage:

### STATUS
Current project state.

### IMPLEMENTADO
What was actually implemented.

### COMPILADO
Whether a clean build succeeded.

### TESTADO
Tests actually executed.

### VALIDADO
Behavior verified in Unreal/runtime.

### FALHAS
Known failures.

### PENDÊNCIAS
Remaining tasks.

### RISCOS
Known technical risks.

### PRÓXIMO PASSO
The next concrete action.

No step may be marked completed merely because code was written.

---

# 27. Development workflow

The standard cycle is:

1. Define phase.
2. Inspect repository.
3. Inspect existing implementation.
4. Decide what is reusable.
5. Implement smallest coherent increment.
6. Compile.
7. Run automated tests.
8. Open/test in Unreal.
9. Validate behavior.
10. Document.
11. Update continuity.
12. Commit.
13. Proceed.

For editor-dependent actions, the user will execute the Unreal-side step when no editor automation/integration is available.

---

# 28. Item creation workflow — final target

A future developer should be able to add an item approximately like this:

### Step 1
Create ItemDefinition.

### Step 2
Choose category.

### Step 3
Set dimensions.

### Step 4
Set requirements.

### Step 5
Set base stats.

### Step 6
Assign equipment slot.

### Step 7
Assign allowed classes.

### Step 8
Assign options/effects.

### Step 9
Assign visual/icon.

### Step 10
Assign drop/shop availability.

### Step 11
Run validator.

### Step 12
Commit content.

### Step 13
Run item tests.

### Step 14
Launch server.

### Step 15
Test in Unreal.

No modification of the inventory engine should be necessary for a normal new item.

---

# 29. Item ID and content validation rules

The future content validator must detect:

- duplicate ItemDefinitionID;
- missing display name;
- invalid category;
- invalid dimensions;
- invalid slot;
- missing visual reference;
- invalid class reference;
- invalid requirement;
- negative invalid stats;
- impossible stack size;
- invalid option reference;
- invalid drop reference;
- invalid shop reference;
- incompatible equipment configuration;
- duplicate content aliases;
- deprecated fields;
- schema version mismatch.

---

# 30. Data migration

The item system must support schema evolution.

Every persistent definition/state must have a migration strategy.

Example:

`ItemSchemaVersion = 1`

→ later

`ItemSchemaVersion = 2`

Existing ItemInstances must remain loadable through migration.

Never silently reinterpret persistent fields.

---

# 31. Source/reference policy

The supplied Item.txt is a **design reference**.

Public MU documentation is a **research reference**.

Neither becomes AGE OF AETHER source code.

AGE OF AETHER must have:

- original C++;
- original Blueprint assets;
- original game rules;
- original data;
- original art;
- original world;
- original networking;
- original persistence;
- original content.

The project may use familiar MMORPG design patterns without reproducing another game's protected implementation or assets.

---

# 32. Research references

The initial research used public sources describing MU item configuration:

- MUDevs example Item.txt structure: https://github.com/MUDevs-Emulator/Default-configuration/blob/master/Season%206/Data/Item/Item.txt
- MU Online Help Forum — historical custom-item workflow: https://forum.muonlinehelp.com/topic170-how-to-add-new-items-to-your-mu-online-server.html
- ViciadosMU — item/server/client configuration workflow: https://viciadosmu.com.br/tutoriais/adicionar-itens-no-servidor
- ViciadosMU — custom ItemList: https://viciadosmu.com.br/en/tutoriais/criar-itens-customizados-itemlist
- IGCN — client custom item workflow: https://www.igcn.mu/guides/client-customization/adding-custom-items-to-the-game-r14/

These sources describe different MU versions/server implementations, so their exact fields and workflows must not be assumed universal.

---

# 33. Immediate implementation order

The next work is **not** to implement the entire MMORPG.

The immediate sequence is:

1. Establish clean Unreal project baseline.
2. Establish Git/Git LFS policy where appropriate.
3. Establish C++ module.
4. Establish Core Runtime.
5. Establish server authority foundation.
6. Establish Data Registry.
7. Establish ItemDefinition.
8. Establish ItemInstance.
9. Establish Inventory.
10. Establish Equipment.
11. Establish Item validation tests.
12. Only then expand to combat/world/content.

---

# 34. Definition of done

A phase is complete only when:

- implementation exists;
- code compiles;
- tests execute;
- expected behavior is verified;
- repository documentation is updated;
- continuity is updated;
- known risks are recorded;
- next step is explicit.

A placeholder must be explicitly labeled as a placeholder.

No fake implementation.

No "implemented" claim for a class that merely exists but does not perform its intended function.

---

# 35. Current project state

At roadmap creation:

- Repository is connected and accessible.
- Repository is currently at the initial/empty foundation stage.
- The supplied Item.txt has been analyzed.
- Public MU item-configuration workflows have been researched.
- The target C++ + Blueprint + Data architecture has been defined.
- The master roadmap has been established.
- The continuity protocol has been established.
- No MMORPG gameplay system is considered implemented yet.

**Current implementation milestone: PHASE 4 — Accounts and Sessions.**


## Phase 12 — Economy & Crafting

Repository implementation is complete.

Implemented:
- character-owned Gold wallets using int64;
- server-authoritative currency set/add/remove operations;
- overflow and insufficient-funds validation;
- data-driven shop definitions and entries;
- fixed buy/sell prices and quantity limits;
- authoritative buy/sell flows integrated with ItemService;
- data-driven crafting recipes and ingredients/outputs;
- crafting level, currency and ingredient requirements;
- complete-output inventory preflight before ingredient consumption;
- atomic Economy Config Data Asset registration;
- economy transaction result contract;
- economy PlayerController RPCs and Blueprint delegate;
- dedicated economy request ordering guard;
- automation tests for wallet, shops, buying, selling, crafting, validation and overflow.

Security boundary:
- client cannot author wallet balance;
- client cannot author prices;
- client cannot author recipe contents;
- client cannot bypass inventory ownership;
- client cannot bypass ingredient, currency, level or capacity rules;
- active CharacterID is resolved from authenticated server state.

Validation truth:
- repository/static validation PASSED;
- Unreal 5.8.1 UHT/UBT/Editor/PIE/multiplayer/Automation Framework/network emulation remain NOT VERIFIED because Unreal is unavailable here;
- no CI pipeline exists to substitute for local Unreal validation.

Deliverable:
- Docs/PHASE_12_ECONOMY_CRAFTING.md

Next implementation target:
**PHASE 13 — MULTIPLAYER & SERVER AUTHORITY**


## Phase 14 — Persistence & Backend

Repository implementation is COMPLETE.

Implemented:
- versioned FAetherCharacterPersistenceSnapshot contract;
- persistent CharacterID/AccountID identity binding;
- character progression, combat and world-state snapshot;
- inventory snapshot;
- economy wallet snapshot;
- quest-state snapshot;
- checksum generation and verification;
- snapshot validation and schema migration boundary;
- optimistic-concurrency revision control;
- rollback-safe snapshot restoration;
- FAetherPersistenceService;
- UAetherPersistenceSaveGame;
- UAetherPersistenceSubsystem;
- alternating durable SaveGame slots;
- disk recovery by highest valid storage revision;
- runtime save/load orchestration across Character, Item, Economy and Quest services;
- stable character identity restoration;
- inventory/economy/quest restore preflight validation;
- persistence automation coverage.

Security boundary:
- client cannot author persistent snapshots;
- AccountID/CharacterID are validated server-side;
- stale writes are rejected;
- corrupted/tampered snapshots are rejected by validation/checksum;
- item ownership and quantity are revalidated on restore;
- currency cannot restore to a negative or duplicate balance;
- quest IDs/objective state are revalidated against server definitions.

Durability boundary:
- the current adapter is a real Unreal SaveGame-backed server persistence implementation;
- two alternating slots provide basic crash-tolerant last-good-state recovery;
- no fake external database provider was introduced;
- the service boundary allows a production database/API repository to replace the local adapter later.

Validation truth:
- repository/static validation PASSED;
- changed-source delimiter audit PASSED;
- escaped-newline audit PASSED;
- persistence rollback audit PASSED;
- Unreal 5.8.1 UHT/UBT/Editor/Automation/disk round-trip/dedicated-server runtime remain NOT VERIFIED because Unreal is unavailable in this environment;
- no CI pipeline exists to substitute for local Unreal validation.

Deliverable:
- Docs/PHASE_14_PERSISTENCE_BACKEND.md

Next implementation target:
**PHASE 15 — SECURITY & ANTI-CHEAT**


## Phase 15 — Security & Anti-Cheat

Repository implementation is COMPLETE at the source/repository level.

Implemented:
- FAetherSecurityConfig;
- FAetherSecurityService;
- UAetherSecuritySubsystem;
- UAetherSecurityConfigDataAsset;
- per-connection request token bucket;
- category-aware request replay protection;
- authentication gating;
- temporary quarantine;
- suspicion scoring;
- bounded security audit trail;
- authoritative movement anomaly sampling;
- security gates on all 43 server RPC implementations in AAetherNetworkPlayerController;
- hardened multiplayer request refill logic separating heartbeat time from request-token refill time;
- security and multiplayer automation coverage.

Security categories:
- Authentication;
- Session;
- Character;
- Inventory;
- Progression;
- Combat;
- World;
- Quest;
- Social;
- Economy;
- Generic networking.

Validation truth:
- source/static validation PASSED;
- 43/43 server RPC security-gate audit PASSED;
- replay-window audit PASSED;
- delimiter audit PASSED;
- escaped-newline audit PASSED;
- multiplayer refill isolation audit PASSED;
- Unreal 5.8.1 UHT/UBT/Editor/PIE/Automation/network-emulation/dedicated-server attack runtime remain NOT VERIFIED because Unreal is unavailable in this environment;
- no CI pipeline exists to substitute for local Unreal validation.

Important boundary:
- the security layer is a defense-in-depth layer and does not replace gameplay-service authority;
- permanent bans, persistent moderation evidence, external anti-cheat, distributed reputation and production security ingestion remain future backend work.

Deliverable:
- Docs/PHASE_15_SECURITY_ANTI_CHEAT.md

Next implementation target:
**PHASE 16 — MMORPG SCALE & DEDICATED SERVER**


## Phase 16 — MMORPG Scale & Dedicated Server

Repository implementation is COMPLETE.

Implemented:
- restored/verified Unreal project foundation required for UBT targets;
- dedicated server target using TargetType.Server;
- FAetherScaleConfig and data-driven scale limits;
- server-node identity and lifecycle;
- accepting/draining admission;
- node health heartbeat;
- load snapshots and load score;
- automatic drain at hard load;
- cross-server transfer request contract with expiration;
- scale subsystem and Data Asset;
- scale automation tests.

Architecture:
- Phase 13 remains the connection-level multiplayer authority;
- Phase 16 adds node-level capacity and lifecycle above connection admission;
- gameplay state remains server-authoritative;
- transfer state cannot be authored by clients;
- no cloud provider, gateway or production database is fabricated.

Replication boundary:
- Replication Graph is documented as the intended scalable Unreal replication option;
- no unverified engine/plugin dependency was hard-wired before local UE 5.8.1 validation.

World boundary:
- World Partition remains the intended large-world streaming technology;
- real maps, Data Layers, HLOD, terrain, collision, NavMesh and streaming sources require Unreal Editor.

Validation truth:
- repository/static validation PASSED;
- dedicated-server target/source inspection PASSED;
- delimiter and escaped-newline audits PASSED;
- Unreal 5.8.1 UHT/UBT/Editor/Automation/dedicated-server cook/runtime/multi-client load/Replication Graph/World Partition remain NOT VERIFIED because Unreal is unavailable;
- no CI pipeline exists to substitute for local Unreal validation.

Deliverable:
- Docs/PHASE_16_SCALE_DEDICATED_SERVER.md

Next implementation target:
**PHASE 17 — AI/GPT INTEGRATION**


## Phase 17 — AI/GPT Integration

Repository implementation is COMPLETE.

Implemented:
- provider-agnostic AI contracts;
- AI request/result/tool contracts;
- server-side AI service;
- GameInstance AI subsystem;
- data-driven AI configuration;
- per-account rate limiting;
- per-account replay protection;
- bounded context/output;
- character-scoped bounded runtime memory;
- provider failure handling;
- output leakage defense-in-depth;
- tool proposal validation;
- automation tests.

Authority boundary:
- AI may produce dialogue and bounded proposals;
- AI never directly mutates gameplay state;
- quest, economy, world, combat, social and persistence systems remain authoritative;
- tool proposals must be validated by the corresponding gameplay subsystem.

Provider boundary:
- no API key;
- no provider secret;
- no client-side OpenAI dependency;
- live provider adapter remains a server/backend concern.

Validation truth:
- repository/static validation PASSED;
- Unreal 5.8.1 UHT/UBT/Editor/Automation/live provider/network validation remains NOT VERIFIED;
- no CI pipeline exists to substitute for Unreal validation.

Deliverable:
- Docs/PHASE_17_AI_GPT_INTEGRATION.md

Next implementation target:
**PHASE 18 — Production & Live MMORPG**


## Phase 18 — Production & Live MMORPG

Repository implementation is COMPLETE at source/repository level. Production lifecycle hardening also completed: drain state is preserved while checks update, and shutdown/readiness semantics are covered by tests.

Implemented:
- FAetherProductionConfig;
- production lifecycle/readiness state;
- startup health gates;
- graceful draining;
- bounded operational metrics;
- bounded operational audit events;
- production GameInstance subsystem;
- production Data Asset;
- production automation tests;
- production release gate documentation.

Operational boundary:
- a node starts non-ready;
- readiness requires all declared checks;
- draining keeps liveness while removing readiness;
- metrics/events are bounded in memory;
- no fake cloud, Kubernetes, database, billing, launcher, CDN or external observability service was fabricated.

Validation truth:
- repository/static implementation review PASSED;
- test source audit PASSED after correcting invalid void-return assertion;
- Unreal 5.8.1 UHT/UBT/Editor/Automation/dedicated-server cook/runtime/load/soak/backup/restore remain NOT VERIFIED because Unreal is unavailable;
- no CI exists to substitute for local Unreal validation.

Deliverable:
- Docs/PHASE_18_PRODUCTION_LIVE_MMO.md

Roadmap status:
**ALL 18 IMPLEMENTATION PHASES ARE COMPLETE AT REPOSITORY LEVEL.**

Future work is production hardening against the real Unreal 5.8.1 environment and live operational infrastructure.
 
# 29. Content Pipeline & Client Program (Phases 19–30)

The original implementation roadmap (Phases 0–18) is complete at repository/source level. The next program is documented in **`ROADMAP_CONTENT_AND_CLIENT.md`** and is incorporated into project continuity.

Execution order:

- Phase 19 — Universal Data Model & Content Registry
- Phase 20 — Asset Pipeline & Visual Asset Registry
- Phase 21 — Complete Item, Equipment & Enhancement Data
- Phase 22 — Monster, NPC, Boss & AI Content
- Phase 23 — Skills, Effects, Buffs, Debuffs & Status
- Phase 24 — Loot, Drop, Reward & World Spawn
- Phase 25 — Quest, Event, Dialogue & World Content Authoring
- Phase 26 — Crafting, Mixing, Forge & Recipe Authoring
- Phase 27 — World, Map, Interaction & Streaming Content
- Phase 28 — Client Core Architecture
- Phase 29 — Complete MMORPG UI/UX
- Phase 30 — Client Presentation, Performance & Final Integration

The client must not become the authority for gameplay state. Stable definition IDs are the bridge between server gameplay data and client visual/presentation assets.

**Next implementation target: Phase 19 — Universal Data Model & Content Registry.**

## Phase 19 — Universal Data Model & Content Registry

**Status: COMPLETE at repository/source level.**

Implemented:
- stable content definition IDs and shared metadata;
- generic data-driven definition contract;
- central content registry;
- duplicate ID detection;
- missing-reference validation;
- circular dependency detection;
- explicit numeric/probability/non-negative validation;
- deterministic definition enumeration;
- structured validation diagnostics;
- Unreal Automation test coverage.

Validation truth:
- repository/static validation: PASS;
- Unreal 5.8.1 UHT/UBT/Automation runtime: NOT VERIFIED because Unreal is unavailable in this environment.

Deliverable:
- `Docs/PHASE_19_DATA_MODEL_CONTENT_REGISTRY.md`

Implementation files:
- `Source/AgeOfAether/Public/Data/AetherContentTypes.h`
- `Source/AgeOfAether/Public/Data/AetherContentRegistry.h`
- `Source/AgeOfAether/Private/Data/AetherContentRegistry.cpp`
- `Source/AgeOfAether/Private/Tests/AetherContentRegistryTests.cpp`

**Next implementation target: Phase 20 — Asset Pipeline & Visual Asset Registry.**


## Phase 20 — Asset Pipeline & Visual Asset Registry

**Status: COMPLETE at repository/source level.**

Implemented:
- stable AssetID identity;
- visual asset type contract;
- asset descriptors for Unreal path, fallback, skeleton, animation, material, icon, VFX and SFX;
- source/import metadata for FBX workflow;
- registry with deterministic enumeration;
- duplicate and cross-reference validation;
- structural validation;
- Automation tests.

Validation truth:
- repository/static validation: PASS;
- Unreal 5.8.1 UHT/UBT/Editor/Automation/FBX import/runtime rendering: NOT VERIFIED because Unreal is unavailable in this environment.

Deliverable:
- \`Docs/PHASE_20_ASSET_PIPELINE_VISUAL_REGISTRY.md\`

**Next implementation target: Phase 21 — Complete Item, Equipment & Enhancement Data.**


## Phase 21 — Complete Item, Equipment & Enhancement Data

Repository implementation is COMPLETE.

Implemented:
- stable ItemDefinition IDs and immutable definition contract;
- item category, rarity, level, class requirements and equipment slots;
- stack limits, weight and durability;
- binding/trade/drop/shop/craft rules;
- buy/sell economy and currency IDs;
- icon/world/equipped visual asset IDs;
- modular stats, options, effects and tags;
- enhancement levels, materials, currency, success chance, failure behavior, protection items, stat scaling and visual assets;
- ItemInstanceSnapshot separation from definition data;
- deterministic item registry and duplicate detection;
- Phase 19 content and Phase 20 asset cross-reference validation;
- automation tests for valid/invalid data, equipment, enhancement, cross-reference and deterministic behavior.

Validation truth:
- repository/static validation PASSED;
- Unreal 5.8.1 UHT/UBT/Editor/PIE/Automation Framework remain NOT VERIFIED because Unreal is unavailable in this environment;
- no compilation success is claimed.

Deliverable:
- Docs/PHASE_21_ITEM_EQUIPMENT_ENHANCEMENT.md

Next implementation target:
**PHASE 22 — Monster, NPC, Boss & AI Content**


## Phase 22 — Monster, NPC, Boss & AI Content

Repository implementation is COMPLETE.

Implemented:
- MonsterDefinition, NPCDefinition and BossDefinition;
- combat statistics, rewards and respawn contracts;
- configurable AI behavior profiles and targeting data;
- NPC faction/dialogue/quest/shop/service references;
- boss multi-phase triggers and phase content;
- stable presentation asset IDs;
- cross-reference validation against Phase 19 content and Phase 20 assets;
- global ID uniqueness and deterministic registry ordering;
- automation tests for validation, cross references, duplicates and determinism.

Validation truth:
- repository/static validation PASSED;
- Unreal 5.8.1 UHT/UBT/Editor/PIE/Automation Framework remain NOT VERIFIED because Unreal is unavailable in this environment;
- no compilation success is claimed.

Deliverable:
- Docs/PHASE_22_MONSTER_NPC_BOSS_AI.md

Next implementation target:
**PHASE 23 — Skills, Effects, Buffs, Debuffs & Status**
