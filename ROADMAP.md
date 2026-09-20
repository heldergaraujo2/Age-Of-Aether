# AGE OF AETHER — MASTER ROADMAP
## MMORPG • Unreal Engine + C++ • Server-Authoritative • Data-Driven

**Repository:** https://github.com/heldergaraujo2/Age-Of-Aether  
**Technical project:** `AgeOfAether`  
**Status:** Phase 39 — Playable Base Character (repository implementation complete; Unreal 5.8.1 runtime/content acceptance pending local validation)  
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
- Phase 22 — Monster, NPC, Boss & AI Content — COMPLETE at repository level; local Unreal validation pending
- Phase 23 — Skills, Effects, Buffs, Debuffs & Status — COMPLETE at repository level; local Unreal validation pending
- Phase 24 — Loot, Drop, Reward & Respawn — COMPLETE at repository level; local Unreal validation pending
- Phase 25 — Quest, Event, Dialogue & World Content Authoring — COMPLETE at repository level; local Unreal validation pending
- Phase 26 — Crafting, Mixing, Forge & Recipe Authoring — COMPLETE at repository level; local Unreal validation pending
- Phase 27 — World, Map, Interaction & Streaming Content — COMPLETE at repository level; local Unreal validation pending
- Phase 28 — Client Core Architecture — COMPLETE at repository level; Unreal runtime validation pending
- Phase 29 — Complete MMORPG UI/UX — COMPLETE at repository level; Unreal runtime/UI validation pending
- Phase 30 — Client Presentation, Performance & Final Integration — COMPLETE at repository level; Unreal runtime validation pending local validation
- Phase 31 — Universal Player Class & Evolution System — COMPLETE at repository level; Unreal runtime validation pending local
- Phase 32 — Five Base Classes & 25 Evolution Definitions — COMPLETE at repository level; Unreal runtime validation pending local
- Phase 33 — Configurable PvE/PvP Class Balance System — COMPLETE at repository level; Unreal runtime validation pending local
- Phase 34 — Class/Combat Integration & Server Authority — COMPLETE at repository level; Unreal runtime validation pending local
- Phase 35 — PvE/PvP Balance Simulation & Automated Validation — COMPLETE at repository level; Unreal runtime validation pending local
- Phase 36 — Class Presentation, Assets & Runtime Acceptance — COMPLETE at repository level; Unreal runtime validation pending local
- Phase 37 — Unreal Visual Foundation — COMPLETE at repository/source level; Unreal runtime validation pending local
- Phase 38 — Real Asset Pipeline — COMPLETE at repository/source level; Unreal import/runtime validation pending local
- Phase 39 — Playable Base Character — COMPLETE at repository/source level; Unreal visual/runtime validation pending local
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


---

# PHASE 31 — UNIVERSAL PLAYER CLASS & EVOLUTION SYSTEM — PLANNED

Purpose:
Create the authoritative, data-driven class architecture that supports five base player classes, five evolution stages per class, future branches and additional classes without rewriting gameplay code.

Scope:
- ClassDefinition and ClassEvolutionDefinition contracts;
- stable ClassID and EvolutionID identity;
- base-class to evolution graph;
- evolution order and prerequisites;
- level/quest/achievement/content unlock conditions;
- role metadata;
- base-stat modifiers and derived-stat modifiers;
- allowed equipment/class restrictions;
- starting equipment and starting skills references;
- progression metadata;
- class tags and gameplay capabilities;
- normalized lookup, duplicate detection and deterministic enumeration;
- cross-validation with existing item, skill, quest, world, content and asset registries;
- server-authoritative class/evolution ownership and transition contract;
- Automation coverage.

Design rule:
Classes are data. The C++ system defines how classes work; data defines which classes exist.

---

# PHASE 32 — FIVE BASE CLASSES & 25 EVOLUTION DEFINITIONS — PLANNED

Initial class roster:

1. ARQUEIRO — Caminho da Precisão
   1. Batedor
   2. Rastreador
   3. Caçador Espectral
   4. Atirador Fantasma
   5. Olho de Deus / Juiz dos Ventos

2. GUERREIRO — Caminho da Fúria
   1. Recruta
   2. Berserker
   3. Campeão de Guerra
   4. Lorde das Lâminas
   5. Avatar da Guerra

3. MAGO — Caminho do Arcano
   1. Aprendiz
   2. Feiticeiro Elemental
   3. Arquimago
   4. Tecelão do Éter
   5. Senhor do Caos Primordial

4. TANK — Caminho da Muralha
   1. Guardião
   2. Fortaleza de Aço
   3. Colosso
   4. Bastião Imortal
   5. Titã Ancestral

5. HEALER — Caminho da Luz
   1. Iniciado
   2. Clérigo da Luz
   3. Oráculo Sagrado
   4. Serafim
   5. Avatar da Vida Eterna

Scope:
- formalize the five identities above as content data;
- preserve a consistent player-facing naming convention;
- define role, fantasy, strengths, weaknesses and intended combat identity;
- define evolution unlock progression without hard-coding individual classes;
- reserve space for future alternate branches;
- connect each evolution to skills, effects, items and visual assets through stable IDs;
- create automated structural/content validation.

Balance constraint:
No evolution receives an unconditional "map-wide hit" or permanent group immortality rule. Extreme fantasy abilities must expose tunable duration, cooldown, range, target count, resistance or PvP scaling parameters so the combat model remains configurable.

---

# PHASE 33 — CONFIGURABLE PVE/PVP CLASS BALANCE SYSTEM — PLANNED

Purpose:
Move class balance values out of gameplay code and into versioned, data-driven configuration so practical testing can change balance without rewriting combat rules.

Planned configuration layers:
- base class modifiers;
- evolution modifiers;
- PvE modifiers;
- PvP modifiers;
- damage;
- critical damage;
- defense;
- healing;
- shields;
- area damage;
- crowd-control effectiveness;
- crowd-control resistance;
- movement speed;
- attack/cast speed;
- threat/aggro;
- lifesteal/regeneration;
- buff/debuff effectiveness;
- resource costs/recovery;
- incoming/outgoing damage scaling;
- target-type modifiers where justified.

Profiles:
- Development;
- Testing;
- Production.

The configuration must be versionable and validated before activation.

Required safety:
- finite numeric values;
- bounded percentages/multipliers;
- no negative damage/defense/healing modifiers unless explicitly modeled as a separate mechanic;
- deterministic resolution;
- safe fallback profile;
- audit-friendly version identity;
- server-authoritative activation;
- client never supplies authoritative balance values.

Recommended data layout:
Config/Balance/
- Classes/
- Evolutions/
- CombatBalance/

The exact serialization format will be selected during implementation to remain compatible with Unreal 5.8.1 and the project's existing data pipeline.

---

# PHASE 33 IMPLEMENTATION STATUS — COMPLETE AT REPOSITORY LEVEL

Implemented:
- Versioned external class balance configuration at `Config/Balance/ClassBalanceProfiles.aetherbalance`.
- Development, Testing and Production profiles.
- Active profile plus Production fallback.
- 20 PvE and 20 PvP configurable multipliers per class/evolution definition.
- Deterministic class + evolution composition.
- Numeric safety envelope [0,10], finite-value checks and exact external row schema.
- Eight Automation tests covering parsing, PvE/PvP separation, composition, invalid values, safety, profile activation/fallback, serialization and validation.
- External config audit: 90 rows, 30 per profile, 5 base class rows + 25 evolution rows per profile, zero malformed rows.
- Initial production values are neutral 1.0; no production balance judgment is embedded in this phase.

Validation truth:
- Repository/source implementation reviewed.
- Unreal 5.8.1 UHT/UBT/Editor/PIE/Automation/Dedicated Server remains NOT VERIFIED.
- Runtime file loading/hot reload is not claimed complete until Unreal validation.

Next implementation target:
**PHASE 34 — CLASS/COMBAT INTEGRATION & SERVER AUTHORITY**

# PHASE 34 — CLASS/COMBAT INTEGRATION & SERVER AUTHORITY — COMPLETE AT REPOSITORY LEVEL

Purpose:
Connect class/evolution identity and balance configuration to the existing authoritative combat and character state systems.

Scope:
- class ownership on Character;
- evolution transition validation;
- class-derived stats;
- equipment compatibility;
- skill compatibility;
- class/evolution modifiers in the combat calculation pipeline;
- PvE versus PvP context resolution;
- target-type and damage-category resolution;
- buff/debuff/healing integration;
- persistence and replication of class/evolution state;
- request/replay/ordering protection;
- audit trail for class changes and balance profile activation;
- deterministic server-side calculations.

Target combat flow:

Input
-> authenticated character
-> class/evolution resolution
-> combat context (PvE/PvP)
-> skill/effect resolution
-> balance profile
-> target modifiers
-> authoritative calculation
-> state mutation
-> persistence
-> replication
-> client presentation.

No client-side class multiplier may become authoritative.

Implementation status:
- Stable ClassID/EvolutionID added to authoritative character state.
- PvE/PvP balance context integrated into deterministic combat calculation.
- Server-side class/evolution and balance resolution added.
- Client class/evolution spoof claims rejected.
- Evolution transition validation added.
- Repository automation tests added for authority, context separation, spoofing, evolution transitions and finite combat results.
- Unreal 5.8.1 compile/runtime remains NOT VERIFIED.

Next implementation target:
**PHASE 35 — PVE/PVP BALANCE SIMULATION & AUTOMATED VALIDATION**

---

# PHASE 35 — PVE/PVP BALANCE SIMULATION & AUTOMATED VALIDATION — COMPLETE AT REPOSITORY LEVEL

Purpose:
Build a deterministic simulation/test layer capable of exercising class matchups and PvE encounters before practical Unreal gameplay testing.

Implementation status:
- Deterministic non-mutating PvE/PvP simulation service added.
- Independent attacker/target class/evolution balance resolution added.
- Finite and safety-bound diagnostics added.
- Determinism and extreme-value validation added.
- Simulation cannot write or promote production balance.
- Automation coverage added for neutral, both-context, deterministic, invalid-input, missing-definition and extreme scenarios.
- Unreal 5.8.1 runtime remains NOT VERIFIED.

Scope:
- deterministic combat simulation inputs;
- reproducible seeds;
- class-vs-class scenarios;
- evolution-vs-evolution scenarios;
- class-vs-monster scenarios;
- boss scenarios;
- party composition scenarios;
- burst/sustain/survivability measurements;
- time-to-kill;
- damage/healing/mitigation statistics;
- cooldown/resource pressure;
- sample-size controls;
- regression baselines;
- balance-budget diagnostics;
- outlier detection;
- report generation.

The simulator is a diagnostic tool. It must not automatically declare a class "best" or alter production balance without explicit configuration changes.

Required tests:
- deterministic replay;
- invalid balance configuration rejection;
- extreme-value protection;
- PvE/PvP context separation;
- class/evolution reference integrity;
- regression detection;
- reproducible simulation results.

---

# PHASE 36 — CLASS PRESENTATION, ASSETS & RUNTIME ACCEPTANCE — PLANNED

Purpose:
Connect the class/evolution system to actual Unreal presentation and validate the complete player-class experience.

Scope:
- class/evolution selection UI;
- class identity and progression presentation;
- class-specific skeletal meshes;
- equipment restrictions and attachments;
- animation sets;
- class/evolution VFX/SFX;
- skill presentation;
- icons;
- class transition effects;
- async asset loading;
- placeholder/fallback assets;
- performance budgets;
- multiplayer PIE validation;
- dedicated-server validation;
- persistence/reconnect validation;
- complete class acceptance matrix.

Runtime acceptance must be performed in Unreal 5.8.1 and must not be marked PASS from repository/static validation alone.

---

# CLASS SYSTEM — DESIGN RULES

The five base classes are the initial roster, not a hard-coded engine limitation.

The architecture must support:
- adding a sixth class through data;
- adding alternate evolution branches later;
- changing evolution requirements without C++ rewrites;
- changing balance values without recompiling gameplay code;
- replacing visual assets without changing class identity;
- testing experimental balance profiles safely.

Player-facing names may follow the pattern:

Base Class -> Evolution Title

but the authoritative IDs must remain stable even if display names or localization change.

The class system must separate:
- ClassDefinition: what the archetype is;
- EvolutionDefinition: how the archetype develops;
- ClassBalanceDefinition: how its combat values are tuned;
- Runtime Character Class State: what this player currently owns;
- Presentation: how the class looks and feels.

---

# CLASS BALANCE PRINCIPLE

Balance is not one percentage.

A class is evaluated through a multidimensional contract:
- damage;
- effective health;
- mitigation;
- mobility;
- range;
- area coverage;
- control;
- healing/support;
- resource economy;
- cooldown pressure;
- threat;
- utility;
- counterplay.

The project will use configurable values and deterministic simulations to expose imbalances during testing. Final production values remain a design decision and will not be hard-coded into the architecture.

---

# CLASS ROADMAP CONTINUITY

After Phase 30, the implementation sequence is:

**31 -> Class Architecture -> 32 -> 25 Class/Evolution Data -> 33 -> Balance Configuration -> 34 -> Combat Integration -> 35 -> Simulation/Automated Balance Validation -> 36 -> Unreal Presentation & Runtime Acceptance**

No Phase 31+ phase should be marked complete merely because files exist. Each phase must report:
- STATUS
- IMPLEMENTADO
- COMPILADO
- TESTADO
- VALIDADO
- FALHAS
- PENDÊNCIAS
- RISCOS
- PRÓXIMO PASSO

Unreal 5.8.1 runtime validation remains a separate gate and cannot be inferred from GitHub/static CI success.


---

# VISUAL & PLAYABLE IMPLEMENTATION PROGRAM — PHASES 37–56

The complete post-runtime visual/playable implementation roadmap is now maintained in:

**ROADMAP_VISUAL_AND_PLAYABLE.md**

This document is the canonical execution plan for transforming the repository/source foundation into the actual Unreal Engine 5.8.1 visual and playable MMORPG.

Execution sequence:

37. Unreal Visual Foundation  
38. Real Asset Pipeline  
39. Playable Base Character  
40. Input, Movement & Camera  
41. Animation System  
42. Equipment & Item Visuals  
43. Five Classes & 25 Evolutions Presentation  
44. Playable Combat  
45. Skills, Buffs, Debuffs & VFX  
46. Monsters, NPCs & Bosses  
47. World, Maps & Streaming  
48. Interaction, NPC, Quest & Events  
49. Inventory, Loot & Equipment Gameplay  
50. Crafting, Forge, Economy & Shops  
51. Complete MMORPG UI/UX  
52. Audio & Ambience  
53. Multiplayer, Dedicated Server & Persistence  
54. Performance, Streaming & Scale  
55. Initial Complete Content Package  
56. Alpha, Beta & Release Candidate

The new roadmap explicitly requires a vertical slice before mass content production and defines acceptance criteria for assets, gameplay systems, FBX imports, multiplayer, persistence, performance and final release.

The first phase must begin only after the Unreal 5.8.1 runtime foundation gate is locally validated.

Next visual/playable target after the Unreal gate:

**PHASE 37 — UNREAL VISUAL FOUNDATION**


## Phase 37 — Unreal Visual Foundation — REPOSITORY IMPLEMENTATION COMPLETE

Implemented the first low-friction Unreal visual/playable foundation:
- explicit Enhanced Input plugin and module dependency;
- UAetherGameInstance and project GameInstance configuration;
- UAetherAssetManager and AetherVisual primary asset scan;
- UAetherVisualFoundationDataAsset with stable AssetID/DefinitionID/fallback/version;
- AAetherCharacter third-person camera, movement and runtime-created Enhanced Input bootstrap;
- AAetherDevelopmentWorldActor temporary ground;
- automatic PlayerStart creation for an empty development level;
- lightweight native AAetherFoundationHUD;
- AetherInteract, AetherTarget and AetherMovement trace channels;
- Phase 37 Automation tests and repository validation.

Safety/authority:
- server remains authoritative over character existence and replicated movement;
- visual/input bootstrap does not mutate authoritative gameplay state;
- no fake .uasset/.umap files were added;
- real FBX, skeleton, materials, animations, VFX and production UI remain asset/editor work for later phases.

Validation truth:
- repository/static validation is the only validation executed in the current environment;
- Unreal 5.8.1 UHT/UBT/Editor/PIE/Automation/Dedicated Server runtime is still NOT VERIFIED;
- the final Phase 37 runtime gate requires creating one empty development map in Unreal, setting it as Game Default Map, then testing character spawn, WASD, mouse camera, jump, HUD, 2-client PIE and Dedicated Server.

Detailed report:
**Docs/PHASE_37_UNREAL_VISUAL_FOUNDATION.md**

Next implementation target after the runtime gate:
**PHASE 38 — REAL ASSET PIPELINE**


## Phase 38 — Real Asset Pipeline — COMPLETE AT REPOSITORY LEVEL

Implemented:
- FAetherAssetImportProfile;
- FAetherAssetSourceMetadata;
- FAetherAssetPipelineRecord;
- FAetherAssetPipelineRegistry;
- EAetherAssetValidationStatus;
- stable lowercase AssetID normalization;
- source/origin/license/author/version/revision metadata;
- Unreal asset path contract;
- skeleton/fallback/dependency references;
- dependency cycle detection;
- missing dependency/fallback detection;
- canonical FBX 2020.2 bootstrap profile;
- /Game/Aether content folder convention;
- Phase 38 automation tests;
- CI validation for the full asset pipeline contract.

Simplification:
- the first real asset can be imported and validated one at a time;
- no mass asset production before the first asset passes;
- no fake .uasset/.umap binaries are committed;
- visual assets remain client-side presentation and never become gameplay authority.

Validation truth:
- repository/static CI: PASS;
- real Unreal 5.8.1 FBX import: NOT VERIFIED in this environment;
- first local acceptance asset should be one simple skeletal FBX.

Detailed report:
Docs/PHASE_38_REAL_ASSET_PIPELINE.md

Next implementation target:
**PHASE 39 — PLAYABLE BASE CHARACTER**


- Phase 40 — Input, Movement & Camera — COMPLETE at repository/source level; Unreal 5.8.1 runtime validation pending local.


## Phase 41 — Animation System — COMPLETE at repository/source level
- Added reusable animation profile and base AnimInstance state contract.
- Added Idle/Walk/Run/Jump/Fall state derivation plus speed, direction, vertical velocity and sprint presentation values.
- Linked animation profile directly to the existing playable visual profile so one Data Asset can configure the visual slice.
- Added Automation coverage and no fake Unreal binaries.
- Unreal 5.8.1 runtime remains unverified; next target Phase 42 — Equipment & Item Visuals.


## Phase 42 — Equipment & Item Visuals — COMPLETE at repository/source level
- Added data-driven equipment visual profile and reusable equipment visual component.
- Supports skeletal/static meshes, slots, sockets, transforms and material overrides.
- Integrated the component into AAetherCharacter without changing gameplay authority.
- Added Automation tests and Phase 42 documentation.
- No fake Unreal binaries were committed.
- Unreal 5.8.1 runtime import/attachment/PIE/Dedicated Server remains a local acceptance gate.
- Next target: Phase 43 — Five Classes & 25 Evolutions Presentation.


## Phase 43 — Five Classes & 25 Evolutions Presentation — COMPLETE at repository/source level

Implemented:
- canonical 5×5 class/evolution presentation catalog with exactly 25 stable entries;
- requested class names, evolution names and path titles;
- reusable class/evolution presentation component integrated into AAetherCharacter;
- optional visual profile, animation profile, icon, VFX and SFX references per evolution;
- same-class stage-1 fallback for entries without authored presentation data;
- Dedicated Server presentation guard;
- Automation coverage for catalog size, identities, display data and fallback;
- Unreal setup documentation designed around one catalog and one reusable visual/animation pipeline.

Low-bureaucracy workflow:
- no per-class C++;
- no 25 custom character Blueprints;
- first vertical slice can use only warrior.01 with real assets;
- later evolutions require data assignment only.

Validation truth:
- repository/source implementation complete;
- no fake .uasset/.umap/FBX/animation binaries added;
- Unreal 5.8.1 UHT/UBT/Editor/PIE/Automation/Dedicated Server runtime remains NOT VERIFIED here;
- actual FBX/animation/icon/VFX/SFX assignment remains local Unreal content work.

Detailed report:
Docs/PHASE_43_FIVE_CLASSES_25_EVOLUTIONS_PRESENTATION.md

Next target:
**PHASE 44 — PLAYABLE COMBAT**


## Phase 44 — Playable Combat — COMPLETE at repository/source level

Implemented:
- Reused the existing server-authoritative combat subsystem/service.
- Added left-mouse basic attack input to AAetherCharacter.
- Added camera-forward Pawn target acquisition.
- Client submits only target CharacterID through the authenticated combat gateway.
- Added optional data-driven BasicAttackMontage to the reusable animation profile.
- Added visual component attack montage trigger.
- Dedicated Server never loads/plays the presentation montage.
- Added Phase 44 documentation.
- No fake Unreal binaries.

Validation truth:
- Source/repository implementation complete.
- Unreal 5.8.1 UHT/UBT/Editor/PIE/2-client/Dedicated Server runtime is NOT VERIFIED here.
- Local acceptance remains required for real montage import, target collision, server combat result/cooldown and multiplayer behavior.

## Phase 45 — Skills, Buffs, Debuffs & VFX — COMPLETE at repository/source level

Implemented:
- data-driven SkillID definitions with bounded cooldown/range/resource/effect values;
- deterministic normalized skill registry with duplicate rejection and atomic batch registration;
- server-authoritative CastSkill request through the existing security/combat gateway;
- class compatibility validation;
- authoritative damage/heal effects;
- runtime buff/debuff status records with stable EffectID;
- reusable client-only skill presentation component with optional montage/VFX/SFX references;
- automatic safe training skill for first vertical-slice validation;
- Automation tests and Unreal setup documentation;
- no fake Unreal binaries.

Validation truth:
- repository/source implementation complete;
- Unreal 5.8.1 UHT/UBT/Editor/PIE/2-client/Dedicated Server runtime remains NOT VERIFIED here;
- real VFX/SFX/montage import and visual acceptance remain local Unreal work.

Low-bureaucracy workflow:
- one generic skill pipeline;
- one reusable visual component;
- new skills are data + optional assets, not new C++ classes;
- visuals are optional and never authoritative.

Next target:
**PHASE 46 — MONSTERS, NPCs & BOSSES**


## Phase 46 — Monsters, NPCs & Bosses — COMPLETE at repository/source level

Implemented:
- data-driven Monster/NPC/Boss creature definitions;
- generic AI profiles: Passive, Guard, Patrol, Aggressive and Boss;
- bounded health, attack, defense, movement speed and level validation;
- loot, quest, dialogue, shop and interaction content IDs;
- optional skeletal mesh, Animation Blueprint, attack montage, VFX and SFX references;
- stable spawn definitions with zone, transform, MaxAlive and bounded respawn time;
- normalized creature registry with duplicate/reference validation;
- UAetherCreatureSubsystem World Subsystem for deterministic data-driven spawning/despawning;
- reusable AAetherCreatureActor presentation shell;
- UAetherCreatureCatalog Data Asset for authoring content without new C++;
- Automation coverage and Unreal setup documentation;
- no fake Unreal binary assets.

Low-bureaucracy workflow:
- one catalog;
- one reusable creature actor;
- one real skeletal FBX can validate the whole visual pipeline;
- additional monsters/NPCs/bosses are data rows plus optional asset assignments.

Validation truth:
- repository/source implementation complete;
- Unreal 5.8.1 UHT/UBT/Editor/PIE/2-client/Dedicated Server runtime remains NOT VERIFIED here;
- real FBX/animation/VFX/SFX import and runtime spawn/replication remain local Unreal acceptance work.

Next target:
**PHASE 47 — WORLD, MAPS & STREAMING**


## Phase 47 — World, Maps & Streaming — COMPLETE at repository/source level

Implemented:
- data-driven map definitions with Persistent/Streamed/Instance modes;
- stable MapID, zone linkage and Unreal asset path contracts;
- validated map-to-map streaming links;
- UAetherWorldMapCatalog Data Asset;
- UAetherWorldMapSubsystem active-map and streaming-link gating;
- Automation tests and Unreal setup documentation;
- no fake .umap/.uasset binaries.

Validation truth:
- repository/source implementation complete;
- Unreal 5.8.1 UHT/UBT/Editor/PIE/2-client/Dedicated Server runtime remains NOT VERIFIED here;
- real map creation, World Partition/level streaming and multiplayer acceptance remain local Unreal work.

Next target:
**PHASE 48 — INTERACTION, NPC, QUEST & EVENTS**


## Phase 48 — Interaction, NPC, Quest & Events — COMPLETE at repository/source level

Implemented:
- Talk/Quest/Shop/Service/Event interaction contracts.
- Stable InteractionID/QuestID/EventID registry with duplicate and value validation.
- Level and prerequisite quest gating.
- Quest lifecycle and bounded objective progression.
- Repeatable/non-repeatable quest rules.
- Timed event definitions with bounded duration.
- UAetherInteractionSubsystem and UAetherInteractionCatalog Data Asset.
- Automation tests and Unreal setup documentation.
- No fake Unreal binary assets.

Low-bureaucracy workflow:
- One catalog + reusable NPC/widget/VFX presentation.
- Existing NPC creature definitions carry InteractionID/QuestActorID/DialogueID/ShopID.
- New content is data, not new C++ classes.

Validation truth:
- Repository/source implementation complete.
- Unreal 5.8.1 runtime remains NOT VERIFIED here.
- Local acceptance still required for actual widget, dialogue, quest/event presentation, multiplayer and Dedicated Server behavior.

Next target:
**PHASE 49 — INVENTORY, LOOT & EQUIPMENT GAMEPLAY**
