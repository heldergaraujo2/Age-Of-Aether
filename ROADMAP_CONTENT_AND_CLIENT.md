# AGE OF AETHER — CONTENT PIPELINE, DATA-DRIVEN GAMEPLAY & CLIENT ROADMAP

> **Purpose:** extend the completed repository foundation (Phases 0–18) into the content/data pipeline and, only after that foundation is stable, the complete Unreal client architecture.
>
> **Target Unreal:** 5.8.1
> **Architecture:** server-authoritative, C++ core, Blueprint presentation/composition, data-driven content, modular services.
>
> This document is the implementation contract for the next development cycle. It deliberately separates **rules/data**, **runtime systems**, **visual assets**, and **client presentation** so that adding content does not require rewriting gameplay code.

---

# 0. Guiding architecture

The project must follow four layers:

1. **Gameplay code** — generic systems and rules.
2. **Definitions/data** — IDs, stats, recipes, skills, loot, quests, NPC behavior and configuration.
3. **Assets** — meshes, skeletons, animations, materials, textures, icons, VFX, SFX, maps.
4. **Client presentation** — UI, HUD, menus, animation state, camera, input, effects and visual feedback.

The fundamental rule is:

**Code provides capability. Data creates content. Assets provide appearance. Client presents the result.**

A new monster, weapon, NPC, recipe or armor set should normally require **data + assets**, not a new C++ system.

The server remains authoritative over:

- identity;
- inventory;
- equipment;
- stats;
- progression;
- combat;
- crafting;
- economy;
- quests;
- drops;
- NPC transactions;
- world state;
- social state;
- anti-cheat/security;
- persistence.

The client owns presentation:

- meshes;
- materials;
- animations;
- VFX/SFX;
- camera;
- UI;
- local input;
- interpolation/prediction where explicitly allowed;
- presentation state.

The client must never become the authority merely because it displays something.

---

# PHASE 19 — UNIVERSAL DATA MODEL & CONTENT REGISTRY

## Objective

Create the universal content identity layer used by every future item, monster, NPC, skill, recipe, loot table, quest, visual asset and client resource.

## 19.1 Stable IDs

Every content object receives a stable identifier, for example:

- `Item.Sword.Iron.001`
- `Item.Fragment.Dragon.001`
- `Monster.Goblin.001`
- `Monster.Boss.Dragon.001`
- `NPC.Blacksmith.001`
- `Skill.Fireball.001`
- `Recipe.Weapon.DragonSword.001`
- `Loot.Goblin.Common.001`
- `Quest.Village.Intro.001`

IDs are machine identities and must not depend on display names.

## 19.2 Definition metadata

Every definition should support:

- DefinitionID;
- SchemaVersion;
- DisplayName;
- Description;
- Tags;
- Category;
- Enabled;
- Deprecated;
- ContentVersion;
- optional localization key;
- validation metadata.

## 19.3 Registry

Create a central registry capable of:

- registering definitions;
- resolving by ID;
- detecting duplicates;
- detecting missing references;
- validating dependencies;
- exposing diagnostics;
- supporting deterministic loading;
- providing safe fallback definitions where appropriate.

The registry must not become a giant hardcoded switch statement.

## 19.4 Data validation

Validation must catch:

- duplicate IDs;
- missing references;
- invalid numeric ranges;
- impossible quantities;
- invalid probabilities;
- circular dependencies where prohibited;
- missing visual references;
- incompatible equipment slots;
- nonexistent skill IDs;
- nonexistent loot tables;
- invalid recipe outputs;
- invalid NPC services;
- schema mismatches.

Example:

`Recipe.DragonSword.001 -> Item.Fragment.Unknown -> referenced definition does not exist.`

## Definition of done

- generic definition contract exists;
- registry exists;
- validator exists;
- duplicate/missing-reference tests exist;
- no content system requires hardcoded C++ IDs.

---

# PHASE 20 — ASSET PIPELINE & VISUAL ASSET REGISTRY

## Objective

Establish the definitive relationship between external 3D source files and Unreal runtime assets.

## 20.1 Import rule

FBX is an **authoring/import format**, not the server's runtime representation.

Pipeline:

`FBX -> Unreal import -> UAsset -> Asset Registry -> Definition -> Spawn/Equip -> Render`

The same principle applies to:

- skeletal meshes;
- static meshes;
- animations;
- textures;
- materials;
- icons;
- VFX;
- SFX.

## 20.2 Asset definition

Create an asset descriptor containing:

- AssetID;
- AssetType;
- Unreal asset reference;
- fallback asset;
- skeleton reference;
- animation set;
- material set;
- icon;
- VFX set;
- SFX set;
- scale;
- socket/profile information;
- content version.

## 20.3 Server/client separation

The server should normally transmit:

`Monster.Goblin.001`

not:

`/Game/Aether/Monsters/Goblin/Goblin.uasset`

The client resolves the ID to its local Unreal asset.

This prevents gameplay code from depending on presentation paths and allows visual replacement without changing server rules.

## 20.4 Asset validation

Detect:

- definition references missing asset;
- wrong asset type;
- missing skeleton;
- animation/skeleton mismatch;
- invalid socket;
- missing icon;
- unsupported material;
- duplicate asset ID;
- missing fallback.

## 20.5 Content structure

Recommended Unreal structure:

`Content/Aether/`

- Characters/
- Monsters/
- NPCs/
- Items/
- Weapons/
- Armor/
- Skills/
- Effects/
- Audio/
- UI/
- Maps/
- Data/
- Materials/
- Textures/
- Animations/

## Definition of done

A new visual asset can be imported and registered without changing gameplay C++.

---

# PHASE 21 — COMPLETE ITEM, EQUIPMENT & ENHANCEMENT DATA

## Objective

Turn the existing item/inventory foundation into a complete MMORPG content model.

## 21.1 Item definitions

Support:

- ItemID;
- type/category;
- rarity;
- level;
- required character level;
- class restrictions;
- equipment slot;
- stack size;
- weight;
- durability;
- sell/buy values;
- binding;
- tradability;
- drop eligibility;
- shop eligibility;
- crafting eligibility;
- icon;
- world mesh;
- equipped visual;
- stats;
- options;
- effects;
- tags.

## 21.2 Equipment

Support:

- weapon;
- off-hand;
- shield;
- helmet;
- chest;
- gloves;
- pants;
- boots;
- accessories;
- rings;
- necklace;
- wings/cosmetic slot;
- mount/cosmetic slot;
- future custom slots.

Equipment validation must be server-side.

## 21.3 Enhancement

Data-driven enhancement should support:

- enhancement level;
- required materials;
- currency cost;
- success chance;
- failure behavior;
- destruction/break behavior if desired;
- protection items;
- stat scaling;
- visual changes;
- maximum level.

Nothing should require a separate C++ function for each +level.

## Definition of done

A designer can create a normal item, equipment piece and enhancement path entirely through definitions/assets.

---

# PHASE 22 — MONSTER, NPC, BOSS & AI CONTENT

## Objective

Create configurable living-world entities.

## 22.1 Monster definition

Support at minimum:

- ID/name/level;
- HP/max HP;
- mana/max mana;
- stamina;
- attack min/max;
- magic attack min/max;
- defense;
- magic defense;
- movement speed;
- attack speed;
- attack range;
- critical chance/damage;
- accuracy/evasion;
- resistances;
- elemental affinity;
- size/scale;
- faction;
- aggro range;
- leash range;
- AI profile;
- skills;
- loot table;
- XP;
- currency reward;
- respawn time;
- spawn behavior;
- mesh;
- animation set;
- VFX/SFX;
- boss flag;
- boss phases.

## 22.2 Attack profiles

A monster can reference reusable attack definitions:

- physical melee;
- ranged projectile;
- magic;
- cone;
- area;
- line;
- summon;
- damage-over-time;
- heal;
- buff;
- debuff;
- hybrid.

## 22.3 NPC definition

Support:

- ID;
- name;
- faction;
- mesh;
- animation;
- dialogue;
- shop;
- quest provider;
- quest turn-in;
- crafting;
- repair;
- bank/storage;
- teleport;
- trainer;
- services;
- interaction radius;
- schedule;
- location rules.

## 22.4 Boss system

Boss definitions should support:

- phases;
- thresholds;
- phase transitions;
- unique attacks;
- enrage;
- adds/summons;
- special loot;
- world announcements;
- respawn rules.

## Definition of done

Adding a new normal monster, elite, boss or NPC requires definitions/assets rather than custom gameplay code.

---

# PHASE 23 — SKILLS, EFFECTS, BUFFS, DEBUFFS & STATUS SYSTEM

## Objective

Create one reusable ability/effect framework for players, monsters, bosses and future NPC abilities.

## Skills

Each skill should support:

- SkillID;
- name;
- description;
- category;
- level;
- resource cost;
- cooldown;
- cast time;
- range;
- target rules;
- area/radius;
- requirements;
- animation;
- VFX;
- SFX;
- effects;
- damage/heal formula;
- projectile definition;
- interrupt rules.

## Effects

Support reusable effects:

- direct damage;
- heal;
- shield;
- stat modifier;
- movement modifier;
- silence;
- stun;
- slow;
- root;
- poison;
- burn;
- regeneration;
- taunt;
- invisibility;
- dispel;
- crowd-control immunity.

Effects need:

- duration;
- stacks;
- stack limit;
- source;
- target;
- periodic interval;
- dispel category;
- server authority.

## Definition of done

The same skill/effect framework can be used by players and monsters without duplicating combat code.

---

# PHASE 24 — LOOT, DROP, REWARD & WORLD SPAWN SYSTEM

## Objective

Make rewards completely data-driven.

## Loot tables

Support:

- guaranteed drops;
- weighted groups;
- independent rolls;
- quantity ranges;
- rarity;
- probability;
- conditional drops;
- boss-only drops;
- event-only drops;
- class restrictions;
- level restrictions;
- pity/guarantee rules where explicitly configured.

## Spawn system

Support:

- spawn group;
- zone;
- coordinates;
- rotation;
- population;
- respawn;
- random radius;
- level range;
- time windows;
- event activation;
- boss schedules.

## Authority

The server performs:

1. kill confirmation;
2. loot-table selection;
3. random roll;
4. item instance creation;
5. ownership/tagging;
6. inventory/world-drop decision;
7. persistence.

The client only presents the result.

---

# PHASE 25 — QUEST, EVENT, DIALOGUE & WORLD CONTENT AUTHORING

## Objective

Extend the existing quest foundation into a full authoring model.

Support:

- quest chains;
- branching objectives;
- kill objectives;
- collect objectives;
- interact objectives;
- travel objectives;
- dialogue objectives;
- craft objectives;
- escort objectives;
- timed objectives;
- world-event objectives;
- optional objectives;
- prerequisites;
- rewards;
- reputation;
- unlocks;
- scripts/events.

Dialogue should be data-driven and localization-ready.

World events should be configurable:

- start condition;
- duration;
- zones;
- participating entities;
- objectives;
- rewards;
- announcements;
- cooldown;
- recurrence.

---

# PHASE 26 — CRAFTING, MIXING, FORGE & RECIPE AUTHORING

## Objective

Deliver the flexible combination system requested for the project.

## Recipe definition

Each recipe supports:

- RecipeID;
- name;
- category;
- ingredients;
- quantities;
- output items;
- output quantities;
- success chance;
- failure behavior;
- required level;
- required skill;
- required station;
- required currency;
- cooldown;
- daily/weekly limit;
- recipe unlock condition;
- catalyst;
- protection item;
- guaranteed-result rules.

Example conceptual recipe:

`DragonFragment x10 + Crystal x2 + Gold 5000 -> DragonSword`

## Client workflow

The client presents:

- recipe list;
- search/filter;
- selected recipe;
- required materials;
- owned quantity;
- required quantity;
- missing quantity;
- success chance;
- cost;
- station;
- requirements;
- output preview;
- result preview;
- combine button.

The client never decides whether the combination succeeds.

## Server workflow

1. Receive RecipeID.
2. Resolve active character.
3. Load recipe definition.
4. Validate requirements.
5. Validate ownership.
6. Validate quantities.
7. Validate currency.
8. Validate capacity.
9. Consume inputs atomically.
10. Roll server-side result.
11. Create outputs.
12. Persist critical state.
13. Return authoritative result.
14. Replicate inventory changes.

---

# PHASE 27 — WORLD, MAP, INTERACTION & STREAMING CONTENT

## Objective

Connect definitions to actual Unreal world content.

Use Unreal World Partition where appropriate.

Support:

- zones;
- cities;
- safe zones;
- PvP zones;
- dungeons;
- instances;
- portals;
- spawn points;
- NPC placement;
- monster placement;
- resource nodes;
- interactive objects;
- world events;
- streaming;
- level-of-detail;
- HLOD;
- NavMesh;
- collision;
- Data Layers.

The server must retain logical world identity while the client streams visual content.

---

# PHASE 28 — CLIENT CORE ARCHITECTURE

## Objective

Build the complete client foundation only after the data/content contracts are stable.

Recommended client modules:

- `AetherClientCore`;
- `AetherUI`;
- `AetherInput`;
- `AetherPresentation`;
- `AetherAssets`;
- `AetherAudio`;
- `AetherCamera`;
- `AetherInteraction`;
- `AetherEffects`.

## Responsibilities

### Client Core

Owns:

- connection state;
- replicated state consumption;
- client lifecycle;
- error handling;
- session presentation.

### Input

Maps:

- movement;
- camera;
- attack;
- skills;
- inventory;
- interaction;
- dodge;
- jump;
- mount;
- UI shortcuts.

Input creates requests; it does not mutate authoritative state.

### Presentation

Converts replicated state into:

- actor visuals;
- animation state;
- effects;
- sounds;
- floating combat text;
- world indicators.

---

# PHASE 29 — COMPLETE MMORPG UI / UX SYSTEM

## Objective

Create a complete client interface architecture rather than isolated screens.

## Required screens

### Login

- account input;
- authentication state;
- connection state;
- error messages;
- reconnect;
- maintenance message.

### Character selection

- character list;
- class;
- level;
- appearance preview;
- create;
- delete where authorized;
- select;
- loading.

### HUD

- HP;
- MP;
- stamina/resource;
- level;
- XP;
- target;
- buffs/debuffs;
- party;
- minimap;
- quest tracker;
- notifications;
- chat;
- hotbar;
- interaction prompt.

### Character

- attributes;
- derived stats;
- equipment;
- cosmetics.

### Inventory

- grid;
- stack count;
- drag/drop;
- split;
- sort;
- filter;
- tooltip;
- context actions.

### Equipment

- slot layout;
- comparison;
- requirements;
- stats;
- enhancement.

### Skills

- learned skills;
- skill levels;
- cooldowns;
- descriptions;
- hotbar assignment.

### Craft

- recipe list;
- ingredient list;
- result preview;
- success chance;
- cost;
- requirements;
- combine action;
- result notification.

### NPC

- dialogue;
- shop;
- quest;
- services;
- crafting;
- teleport;
- repair.

### Quests

- active;
- available;
- completed;
- objectives;
- rewards;
- tracking.

### Social

- friends;
- party;
- guild;
- whisper;
- block;
- invitations.

### Map

- world map;
- zone map;
- player marker;
- party markers;
- NPC markers;
- quest markers;
- discovered locations.

### Settings

- graphics;
- audio;
- controls;
- accessibility;
- UI scale;
- language;
- network/debug information where appropriate.

---

# PHASE 30 — CLIENT PRESENTATION, PERFORMANCE & FINAL INTEGRATION

## Objective

Turn the client architecture into the actual playable Unreal client.

## 30.1 Character presentation

Implement:

- skeletal mesh;
- equipment visuals;
- weapon attachments;
- armor slots;
- animation blueprint;
- locomotion;
- combat animation states;
- hit reactions;
- death;
- emotes;
- mounts;
- cosmetics.

## 30.2 Asset resolution

Client receives stable IDs and resolves:

`DefinitionID -> AssetDefinition -> Unreal Asset -> Actor/Component presentation`

Missing assets must produce a safe placeholder and a diagnostic rather than crashing the client.

## 30.3 Network presentation

Replicated state must drive presentation through a controlled layer.

Do not scatter network callbacks directly through every widget.

Use presentation/view-model boundaries where practical.

## 30.4 UI performance

Requirements:

- avoid per-frame widget creation;
- virtualize long lists;
- cache icons;
- pool frequently spawned combat text/effects;
- avoid unnecessary bindings/ticks;
- asynchronous asset loading;
- controlled texture/material memory;
- predictable UI invalidation.

## 30.5 World performance

Measure:

- frame time;
- GPU;
- CPU;
- draw calls;
- memory;
- texture memory;
- animation cost;
- VFX cost;
- actor count;
- replication relevance;
- network bandwidth.

Do not optimize by guesswork.

## 30.6 Final client acceptance

A complete client acceptance pass must verify:

1. Launch.
2. Login.
3. Authentication.
4. Character list.
5. Character creation.
6. Character selection.
7. World loading.
8. Character spawn.
9. Movement.
10. Camera.
11. Interaction.
12. Targeting.
13. Combat.
14. Skills.
15. Inventory.
16. Equipment.
17. Loot.
18. Crafting.
19. NPC.
20. Shop.
21. Quests.
22. Party.
23. Guild.
24. Chat.
25. Map.
26. Persistence.
27. Reconnect.
28. Disconnect/recovery.
29. Security rejection.
30. Dedicated-server connection.

---

# CROSS-PHASE VALIDATION SYSTEM

Every phase must use the project's mandatory status format:

## STATUS
Current state.

## IMPLEMENTADO
Actual code/data/assets completed.

## COMPILADO
Whether the relevant build actually succeeded.

## TESTADO
Tests actually executed.

## VALIDADO
Runtime behavior actually verified.

## FALHAS
Known failures.

## PENDÊNCIAS
Remaining work.

## RISCOS
Known technical risks.

## PRÓXIMO PASSO
One concrete next action.

No phase is complete merely because files were created.

---

# CONTENT AUTHORING WORKFLOW — FINAL TARGET

To add a new monster:

1. Create MonsterDefinition.
2. Assign stable MonsterID.
3. Configure stats.
4. Configure attacks.
5. Assign skills.
6. Assign AI profile.
7. Assign loot table.
8. Assign XP/currency.
9. Assign respawn.
10. Assign mesh/animation/VFX/SFX.
11. Validate.
12. Import/register assets.
13. Run tests.
14. Place spawn.
15. Test on dedicated server + client.

To add a new item:

1. Create ItemDefinition.
2. Assign ItemID.
3. Configure category/slot.
4. Configure stats/options.
5. Configure requirements.
6. Configure stack/durability/binding.
7. Assign icon/mesh/equipped visual.
8. Add to loot/shop/recipe definitions as required.
9. Validate.
10. Test inventory/equipment/persistence.
11. Test in Unreal.

To add a new recipe:

1. Create RecipeDefinition.
2. Add ingredients.
3. Add quantities.
4. Add outputs.
5. Set chance.
6. Set failure behavior.
7. Set costs/requirements.
8. Assign station.
9. Validate.
10. Test atomic consumption.
11. Test success/failure.
12. Test UI.
13. Test persistence.

To add an NPC:

1. Create NPCDefinition.
2. Configure identity/faction.
3. Assign visual assets.
4. Configure interactions.
5. Attach shop/quest/craft/teleport services.
6. Place in world.
7. Validate.
8. Test interaction on client and server.

---

# FINAL ARCHITECTURAL CONTRACT

The final Age of AetheR client/server relationship is:

`Player Input
 -> Client Request
 -> Server Validation
 -> Authoritative Service
 -> State Mutation
 -> Persistence
 -> Replication
 -> Client Presentation
 -> UI/Animation/VFX`

For visual content:

`FBX/Source Asset
 -> Unreal Import
 -> UAsset
 -> Asset Registry
 -> Stable Asset/Definition ID
 -> Client Resolver
 -> Mesh/Material/Animation/VFX/SFX`

For gameplay content:

`Definition ID
 -> Data Registry
 -> Server Service
 -> Authoritative Runtime State
 -> Replicated Contract
 -> Client View Model
 -> UI/Presentation`

This separation is mandatory because it permits:

- thousands of items;
- thousands of monsters;
- hundreds of NPC types;
- large recipe libraries;
- reusable skills/effects;
- seasonal content;
- events;
- visual replacements;
- localization;
- future dedicated-server scaling;
- future launcher/patcher;
- future content hotfix strategy where supported;
- future database/backend replacement;
- future client optimization without rewriting server rules.

The goal is not to build a giant hardcoded MMORPG.

The goal is to build a **generic MMORPG engine/framework inside Age of AetheR capable of producing a large game through data and assets.**

---

# EXECUTION ORDER AFTER PHASE 18

1. Phase 19 — Universal Data Model & Content Registry.
2. Phase 20 — Asset Pipeline & Visual Asset Registry.
3. Phase 21 — Complete Item/Equipment/Enhancement Data.
4. Phase 22 — Monster/NPC/Boss/AI Content.
5. Phase 23 — Skills/Effects/Status.
6. Phase 24 — Loot/Drop/Spawn.
7. Phase 25 — Quest/Event/Dialogue/World Content.
8. Phase 26 — Craft/Mix/Forge.
9. Phase 27 — World/Map/Interaction/Streaming.
10. Phase 28 — Client Core.
11. Phase 29 — Complete Client UI/UX.
12. Phase 30 — Client Presentation/Performance/Final Integration.

**Important:** Phases 19–27 establish the content contract before serious client implementation. Phase 28 begins client development only after the definitions and server contracts are sufficiently stable. Phase 30 is the final client integration gate.

