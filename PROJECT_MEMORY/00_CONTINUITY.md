# AGE OF AETHER — CONTINUITY / HANDOFF

> This file is the canonical handoff document for continuing AGE OF AETHER in a new chat/agent.
> Read this file before making project decisions.

## 1. Project identity

**Name:** AGE OF AETHER  
**Technical project:** AgeOfAether  
**Repository:** https://github.com/heldergaraujo2/Age-Of-Aether  
**Branch:** main  
**Current stage:** Phase 0 — Repository and architecture foundation COMPLETE  
**Roadmap:** `ROADMAP.md`

AGE OF AETHER is intended to become a persistent MMORPG built with Unreal Engine, C++, Blueprint and a server-authoritative architecture.

## 2. Non-negotiable architecture

- Client requests; server validates and decides.
- Persistent critical state is authoritative on the server.
- C++ implements core rules and critical systems.
- Blueprint is used for presentation/composition and safe designer-facing workflows.
- Data defines content.
- Gameplay code must not be rewritten for every new item/quest/monster/event.
- Persistence is separated from gameplay through services/repositories.
- Critical operations are transactional.
- Security validation is server-side.
- Testing starts from the foundation.
- Documentation and this continuity file are part of the project.

## 3. Current status

Repository is at the initial foundation stage.

Completed as planning/documentation:

- project identity;
- master MMORPG architecture;
- C++ + Blueprint strategy;
- data-driven content strategy;
- ItemDefinition / ItemInstance concept;
- inventory/equipment architecture concept;
- item creation pipeline concept;
- research of public MU item configuration patterns;
- master roadmap;
- continuity protocol.

Not yet implemented:

- Unreal project baseline;
- C++ modules;
- runtime;
- networking;
- accounts;
- characters;
- item registry;
- inventory;
- equipment;
- database;
- combat;
- world;
- events;
- production server.

## 4. Supplied Item.txt

The user supplied `Item.txt`.

The file is 2,221 text lines and contains 16 numbered sections (0–15), each ending with `end`.

It identifies itself as part of 3DServers MuOnline Server files and contains a 3DServers copyright header.

The file is a reference only.

Observed section record counts:

- 0: 96
- 1: 11
- 2: 33
- 3: 19
- 4: 52
- 5: 69
- 6: 43
- 7: 161
- 8: 182
- 9: 182
- 10: 162
- 11: 182
- 12: 240
- 13: 275
- 14: 278
- 15: 38

The schemas vary by category. Common fields include Type, Slot, Skill, Width, Height, serial/option/drop flags and Name. Weapon sections include damage/speed/durability and requirements. Defensive sections include defense/magic defense variants. Other sections contain BuyMoney, resistances, Value and skill/scroll-specific requirements.

The architecture must therefore support a common item definition plus specialized/modular properties rather than one rigid record.

## 5. MU research conclusions

Public research confirms that MU item addition can involve multiple synchronized concerns:

- server item definition;
- client item definition;
- item options;
- drop configuration;
- shops;
- item visual/model/texture;
- tooltip/presentation;
- version-specific constraints.

AGE OF AETHER will use the architectural lesson, not copy MU implementation.

## 6. Target item architecture

### ItemDefinition
Defines the item type.

Examples:

- ItemDefinitionID
- name/description
- category/subcategory
- slot
- dimensions
- base stats
- requirements
- class restrictions
- flags
- stack rules
- option references
- effect references
- visual references
- economy/drop rules

### ItemInstance
Defines the actual persistent object.

Examples:

- ItemInstanceID
- DefinitionID
- OwnerCharacterID
- location
- quantity
- level
- durability
- rarity
- quality
- enhancement
- options
- sockets
- binding
- custom data

## 7. Inventory target

Must support:

- grid;
- item dimensions;
- add/remove;
- move/swap;
- stack/split/merge;
- pickup/drop;
- use;
- equip/unequip;
- destroy;
- trade;
- sell/buy;
- quest/crafting/event consumption.

## 8. Equipment target

Target slots:

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

All equip operations must be server validated.

## 9. Item creation target

Adding a normal new item should require data/content creation, not a new inventory engine implementation.

Target:

ItemDefinition
→ validation
→ registry
→ server load
→ ItemInstance generation
→ inventory/equipment
→ persistence
→ replication
→ client visual/UI

## 10. Full planned systems

- Core
- Runtime
- Configuration
- Networking
- Accounts
- Sessions
- Characters
- Stats
- Progression
- Items
- ItemInstances
- Inventory
- Equipment
- Options
- Sockets
- Enhancement
- Drops
- World
- Maps
- NPCs
- Monsters
- AI
- Combat
- Skills
- Effects
- Quests
- Rewards
- Parties
- Guilds
- Economy
- Trade
- Shops
- Marketplace
- Crafting
- Events
- Bosses
- Instances
- Achievements
- Titles
- Rankings
- Pets
- Mounts
- Social
- Mail
- Security
- Anti-cheat
- Persistence
- Administration
- Observability
- Testing
- Dedicated server
- Load testing
- Optimization
- Production readiness

## 11. Development rule

Never jump directly from planning to a fake full implementation.

For each phase:

1. inspect;
2. implement;
3. compile;
4. test;
5. validate;
6. document;
7. update this file;
8. commit;
9. continue.

## 12. Mandatory status after major work

STATUS  
IMPLEMENTADO  
COMPILADO  
TESTADO  
VALIDADO  
FALHAS  
PENDÊNCIAS  
RISCOS  
PRÓXIMO PASSO

## 13. Immediate next step

Create the clean Unreal project baseline and repository structure.

Before implementation, determine:

- exact Unreal Engine version installed by the user;
- Windows/toolchain environment;
- desired project template;
- whether Git LFS will be enabled;
- whether the repository should remain public or be made private.

Then establish:

- `.uproject`;
- Config;
- Source;
- Content;
- Plugins;
- Docs;
- PROJECT_MEMORY;
- tests;
- Git ignore/LFS policy.

Do not implement all gameplay systems in this step.

## 14. Important continuity instruction

When another chat starts:

1. Read this file.
2. Read `ROADMAP.md`.
3. Inspect the repository.
4. Determine the latest completed phase from Git history/files.
5. Never assume a phase is complete only because documentation says it should exist.
6. Verify code, build and tests.
7. Continue from the actual repository state.
8. Update this file after every major change.

## 15. Master Technical Blueprint

The canonical detailed architecture document is:

Docs/MASTER_TECHNICAL_BLUEPRINT.md

It defines the target architecture and implementation model for:
- Unreal Engine;
- C++;
- Blueprint;
- data-driven content;
- server authority;
- Core Runtime;
- accounts/sessions;
- characters/stats/progression;
- ItemDefinition and ItemInstance;
- inventory/equipment;
- options/sockets/enhancement;
- drops;
- world/NPCs/monsters/AI;
- combat/skills/effects;
- quests/rewards;
- party/guild;
- economy/trade/shops;
- crafting;
- events/instances/bosses;
- social/mail;
- persistence;
- networking;
- security/anti-cheat;
- audit/logging/observability;
- testing;
- dedicated server;
- future multi-server boundaries;
- content/editor pipeline.

This document describes the target architecture, not proof of implementation. Actual implementation status must always be verified against source code, tests and Git history.

## 16. IP/source rule

Do not copy MU Online proprietary code, assets, maps, protocols, client binaries or protected content.

Use the supplied Item.txt and public documentation only as reference for data modeling and architectural lessons.

AGE OF AETHER must use original implementation and original content.


## 16. Phase 0 evidence

- Repository tree inspected at the pre-change commit and after Phase 0 changes.
- Recent Git history inspected; prior work is documentation/architecture only.
- Required source-of-truth documents verified.
- Git ignore and LFS policy added.
- Repository foundation, coding standards, test strategy and asset/IP policy added.
- No Unreal runtime or gameplay implementation was falsely declared complete.

## 17. Next phase

PHASE 1 — Unreal Project Foundation.

First verify Unreal Engine 5.8.1 locally, verify the C++ toolchain and Git LFS, then create/validate the real Unreal project baseline. Do not create a fake .uproject in Phase 0.
