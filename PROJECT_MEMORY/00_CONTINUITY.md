# AGE OF AETHER — CONTINUITY / HANDOFF

> Canonical handoff for continuing AGE OF AETHER. Read this before making project decisions.

## 1. Project identity

**Name:** AGE OF AETHER  
**Technical project:** AgeOfAether  
**Repository:** https://github.com/heldergaraujo2/Age-Of-Aether  
**Branch:** phase39-playable-base-character  
**Target Unreal:** 5.8.1  
**Current stage:** Phase 39 — Playable Base Character (repository implementation complete; Unreal 5.8.1 visual/runtime acceptance pending local validation). Next implementation target: Phase 40 — Input, Movement & Camera.  
**Roadmap:** ROADMAP.md

AGE OF AETHER is a persistent MMORPG built around Unreal Engine, C++, Blueprint and a server-authoritative architecture.

## 2. Non-negotiable architecture

- Client requests; server validates and decides.
- Persistent critical state is authoritative on the server.
- C++ implements core rules and critical systems.
- Blueprint is presentation/composition and safe designer-facing workflows.
- Data defines content.
- Gameplay code must not be rewritten for every new item/quest/monster/event.
- Persistence is separated from gameplay through services/repositories.
- Critical operations are transactional.
- Security validation is server-side.
- Testing starts from the foundation.
- Documentation and this continuity file are part of the project.
- Do not fake implementation or claim local Unreal validation that was not executed.

## 3. Completed phases

### Phase 0 — Repository foundation

Completed repository policies and architecture:
- .gitignore;
- .gitattributes;
- repository foundation;
- coding standards;
- Git/asset policy;
- test strategy;
- continuity/roadmap foundation.

### Phase 1 — Unreal project foundation

Repository baseline exists:
- AgeOfAether.uproject;
- Config;
- Source/AgeOfAether;
- C++ game/editor targets;
- Tests/README.md;
- Docs/UNREAL_ENGINE_HANDOFF.md.

Local-only gates remain unverified:
- Unreal Editor startup;
- Development Editor compilation;
- baseline map;
- Visual Studio C++ toolchain;
- Git LFS runtime behavior;
- clean-clone reproduction.

### Phase 2 — Core Runtime

Implemented:
- runtime lifecycle/health/result contracts;
- server clock;
- scheduler;
- runtime configuration;
- service registry;
- core logging;
- UAetherRuntimeSubsystem;
- core automation tests;
- graceful shutdown hardening.

Repository/source inspection passed. Unreal UHT/UBT/Editor/test execution remains local-only and unverified.

### Phase 3 — Networking Foundation

Implemented:
- protocol version contract;
- request/response contract;
- authoritative replicated GameState;
- server RPC request endpoint;
- client RPC response endpoint;
- request validation;
- request ordering/replay guard;
- network GameMode;
- networking automation tests.

Generic Unreal replication is used at foundation scale. Replication Graph/Iris scaling decisions remain later and must be driven by profiling.

Phase 3 repository implementation is complete. Unreal multiplayer runtime validation remains pending.

## 4. Phase 4 — Accounts and Sessions

Repository implementation is complete.

### Account

Implemented:
- FAetherAccountId;
- FAetherAccountRecord;
- username normalization;
- account status: Active, Disabled, Locked;
- permission flags: Player, VIP, Moderator, GM, Admin, Owner.

### Authentication

Implemented:
- authentication abstraction via injected FCredentialVerifier;
- opaque credential proof input;
- no plaintext password storage;
- invalid credentials;
- unavailable verifier;
- protocol compatibility validation;
- duplicate-login protection.

The current verifier is intentionally an abstraction. Persistent credential storage/external identity belongs to later backend/persistence/security work.

### Session

Implemented:
- FAetherSessionId;
- FAetherSessionRecord;
- active session tracking;
- login;
- reconnect;
- logout;
- heartbeat;
- timeout;
- account status enforcement.

Runtime session timeout defaults to 30 seconds and is checked by the account/session GameInstance subsystem ticker.

### Networking integration

AAetherNetworkPlayerController now exposes:
- AuthenticateAccount;
- ReconnectAccount;
- LogoutAccount;
- SendSessionHeartbeat;
- IsAccountAuthenticated;
- GetAuthenticatedAccountId;
- GetSessionId.

Account RPCs use separate request IDs from generic Phase 3 networking requests.

Server validates session/account ownership and never trusts client-provided authoritative account/session state.

### Runtime subsystem

UAetherAccountSessionSubsystem:
- derives from UGameInstanceSubsystem;
- owns the runtime account/session service;
- provides the service boundary to networking;
- expires timed-out sessions.

The subsystem is runtime-only. It is not a database.

### Phase 4 tests

Added:
Source/AgeOfAether/Private/Tests/AetherAccountSessionTests.cpp

Coverage:
- registration;
- username normalization;
- duplicate registration;
- bad credentials;
- successful authentication;
- permissions;
- duplicate simultaneous login;
- heartbeat;
- reconnect;
- logout;
- closed-session rejection;
- timeout;
- diagnostic inspection;
- disabled account;
- re-enable;
- incompatible protocol;
- no session created on protocol rejection.

These are Unreal Automation Framework tests.

## 5. Phase 5 — Character Foundation

Repository implementation is complete.

Implemented:
- FAetherCharacterId;
- FAetherCharacterRecord;
- account-to-character ownership;
- normalized globally unique character names;
- five-character account limit;
- Warrior/Mage/Archer/Cleric class contract;
- Created/Available/Selected/Active/Offline/Disabled/Deleted lifecycle;
- base stats;
- derived stats;
- runtime world location/rotation;
- character creation;
- account character listing;
- selection;
- deselection;
- server-side ownership checks;
- character GameInstance subsystem;
- replicated character PlayerState;
- replicated ACharacter pawn;
- spectator-before-selection flow;
- server-authoritative character spawn;
- logout character cleanup;
- server-side disabled/deleted lifecycle control and deleted-character listing exclusion;
- character automation tests.

Network integration:
- AAetherNetworkPlayerController exposes character list/create/select/deselect requests.
- All mutations are server RPCs.
- The server checks authenticated AccountID before character operations.
- The server spawns the character only after successful selection.
- The client never supplies authoritative ownership or lifecycle state.

Tests added:
- Source/AgeOfAether/Private/Tests/AetherCharacterTests.cpp
- creation;
- ID generation;
- name normalization;
- initial state;
- derived stats;
- ownership;
- cross-account selection rejection;
- account limit;
- name boundaries;
- duplicate names;
- selection lifecycle;
- one-character-active rule;
- deselection;
- location authority;
- account listing and deterministic ordering.

Validation truth:
- Repository/static inspection passed.
- Unreal UHT/UBT/Editor/PIE/multiplayer/movement/automation runtime are NOT VERIFIED because Unreal 5.8.1 is not executable in this environment.
- No CI check exists that can substitute for Unreal validation.

Security/persistence boundary:
- Character data remains runtime-only.
- Persistence/database work belongs to later backend/persistence phases.
- Combat, inventory, equipment, progression and skills are not implemented in this phase.

## Phase 6 — Item and Inventory System

Repository implementation is complete.

Implemented:

- FAetherItemDefinition and stable definition identity;
- FAetherItemInstance with separate instance identity;
- CharacterID-owned 64-slot inventory;
- stacking and multi-stack grants;
- remove, move, split and merge;
- transactional AddItem with no partial mutation on failed capacity;
- UDataAsset item definition contract;
- UAetherItemSubsystem service boundary;
- server-authoritative inventory RPCs;
- authoritative inventory snapshots;
- automation coverage for definitions, stacking, mutations, ownership isolation and transactional capacity failure.

Network integration:

- AAetherNetworkPlayerController exposes inventory read and mutation requests;
- server derives the active CharacterID from authenticated controller/PlayerState;
- client cannot submit authoritative CharacterID or ItemInstance ownership;
- mutations execute only through the server-side inventory subsystem.

Validation truth:

- repository/static inspection passed;
- Unreal UHT/UBT/Editor/PIE/network runtime/Automation Framework remain NOT VERIFIED because Unreal 5.8.1 is not executable in this environment;
- no CI check exists that can substitute for Unreal validation.

Persistence boundary:

- item state is runtime-only in Phase 6;
- durable item repositories/database remain future work;
- equipment, options, enhancement, drops, shops, trade and crafting are deferred.

## Phase 7 — Progression System

Repository implementation is complete.

Implemented:

- FAetherProgressionConfig;
- FAetherProgressionResult;
- authoritative experience grants;
- bounded linear experience curve;
- default max level 4000;
- multi-level advancement with rollover XP;
- unspent stat points;
- Strength/Agility/Vitality/Energy/Command allocation;
- configurable stat cap;
- derived-stat recalculation;
- CharacterID/AccountID ownership validation;
- disabled/deleted lifecycle rejection;
- progression GameInstance subsystem;
- CharacterService progression commit boundary;
- progression Data Asset configuration contract;
- replicated level/XP/stat points/base stats/derived stats;
- server-authoritative stat allocation RPC;
- progression automation tests.

Security boundary:

- XP has no client RPC;
- stat allocation is only a server-validated request;
- AccountID comes from authenticated session state;
- CharacterID comes from active Character PlayerState;
- client cannot author XP, level, stat totals, derived stats or stat-point balance.

Validation truth:

- repository/static validation PASSED;
- Unreal UHT/UBT/Editor/PIE/replication/network emulation/Automation Framework are NOT VERIFIED because Unreal 5.8.1 is unavailable in this environment;
- no CI pipeline exists that can substitute for Unreal validation.

Persistence boundary:

- progression remains runtime-only;
- durable progression storage belongs to later persistence/backend phases.

Next implementation target:

**PHASE 8 — Combat Runtime**

Combat will consume authoritative Character and Progression state while keeping damage, cooldowns, targets and rewards server-authoritative.

## Phase 8 — Combat Runtime

Repository implementation is complete.

Implemented:
- server-authoritative basic attack request;
- attacker identity derived from authenticated Character PlayerState;
- target validation and self-target rejection;
- authoritative range validation;
- server-side per-character cooldown;
- deterministic accuracy and damage rolls;
- defense mitigation;
- derived resistance mitigation;
- critical hit calculation;
- shield absorption before health;
- authoritative health clamping;
- Dead combat state;
- replicated health/shield/combat state;
- combat configuration Data Asset contract;
- combat result/audit payload;
- CharacterService combat-state commit boundary;
- combat RPCs and client combat result delegate;
- automation tests for hit, defense, resistance, shield, critical, miss, cooldown, range, target validation, death and invalid configuration.

Security boundary:
- client cannot submit damage, attacker identity, attacker stats, target health, cooldown time or critical outcome;
- attacker is derived from authenticated session + active PlayerState;
- attacker/target positions and combat stats are read from authoritative CharacterService state;
- cooldown is server-time based;
- target state is committed only through CharacterService.

Validation truth:
- repository/static validation is complete and will be reported separately;
- Unreal UHT/UBT/Editor/PIE/replication/network emulation/Automation Framework are NOT VERIFIED because Unreal 5.8.1 is not executable in this environment;
- no CI pipeline exists that can substitute for Unreal validation.

Persistence/effects boundary:
- health/combat state remains runtime-only;
- skills, buffs/debuffs, AI combat, projectile collision, kill rewards, loot and respawn rules remain later systems;
- combat persistence/history belongs to later persistence/backend phases.

Deliverable:
- Docs/PHASE_8_COMBAT_RUNTIME.md

Next implementation target:
**PHASE 9 — World Runtime**

## Phase 9 — World Runtime

Repository implementation is complete.

Implemented:
- FAetherWorldZone and zone types Safe/PvP/Event/Dungeon/City;
- authoritative zone enable and level rules;
- FAetherWorldSpawnPoint and deterministic spawn resolution;
- FAetherWorldPortal and directed transition rules;
- FAetherWorldConfig and data-driven UAetherWorldConfigDataAsset;
- atomic world-definition loading;
- authoritative character initial spawn;
- authoritative portal transition;
- CurrentZoneId and CurrentZoneType on FAetherCharacterRecord;
- CharacterService world-state commit boundary;
- replicated zone identity on AAetherCharacterPlayerState;
- server-side pawn teleport after accepted transition;
- authoritative GameState revision increment;
- server RequestWorldTransition RPC;
- world transition result delegate;
- automation tests for configuration, definitions, deterministic spawns, level requirements, missing/disabled portals/zones, initial spawn and character transition.

Security boundary:
- client supplies only a target ZoneID request;
- server resolves source zone from authoritative character state;
- server resolves portal and target spawn;
- client cannot provide destination coordinates or rotation;
- world state is committed through CharacterService and replicated through PlayerState.

Unreal/content boundary:
- no .umap or fake World Partition assets are generated without Unreal;
- World Partition is the intended large-world technology;
- actual maps, Data Layers, HLOD, terrain, collision/NavMesh and streaming-source authoring remain Unreal content work.

Validation truth:
- repository/static validation is performed after implementation;
- Unreal 5.8.1 UHT/UBT/Editor/PIE/network emulation/Automation Framework remain NOT VERIFIED in this environment;
- no CI pipeline exists to substitute for local Unreal validation.

Next implementation target:
**PHASE 10 — Quest/Mission**


## Phase 10 — Quest/Mission

Repository implementation is complete.

Implemented:
- data-driven quest contracts and `UAetherQuestDefinitionDataAsset`;
- quest lifecycle and objective contracts;
- minimum-level and prerequisite validation;
- character-owned runtime quest state;
- active quest capacity;
- authoritative accept/abandon/complete operations;
- objective progression by ID and by authoritative type/target;
- required/optional objective semantics;
- reward contract and duplicate-reward guard;
- `UAetherQuestSubsystem` service boundary;
- quest list/accept/abandon/complete server RPCs;
- AccountID/CharacterID ownership enforcement;
- dedicated quest request replay/ordering guard;
- Blueprint-assignable quest operation/list delegates;
- automation tests for definition validation, registration, lifecycle, progression, prerequisites, security, capacity and optional objectives.

Security boundary:
- client does not provide authoritative AccountID or CharacterID;
- client cannot directly grant objective progress;
- server resolves active character from authenticated PlayerState;
- completion requires all required objectives to be complete;
- completed state prevents a second reward grant.

Integration boundary:
- Combat, Inventory, World and Progression can call server-side objective advancement APIs when their authoritative events are implemented;
- no cross-system client event is trusted;
- reward data is returned as an authoritative contract; durable reward/persistence transactions remain later work.

Validation truth:
- repository/static validation passed;
- Unreal 5.8.1 UHT/UBT/Editor/PIE/network emulation/Automation Framework remain NOT VERIFIED because Unreal is unavailable in this environment;
- no CI pipeline exists to substitute for local Unreal validation.

Documentation:
- `Docs/PHASE_10_QUEST_MISSION.md`
- `ROADMAP.md`

Next implementation target:
**PHASE 11 — Social**

## Phase 11 — Social

Repository implementation is complete.

Implemented:
- account-level friends and pending requests;
- accept/reject/remove/block/unblock;
- party creation, invitation, acceptance, leave, leader handoff and kick;
- party capacity and account/character membership indexes;
- guild creation, unique normalized names, invitations and role management;
- guild leader/officer/member roles and leave rules;
- Local/Party/Guild/Whisper/System chat contracts;
- server-side message validation, rate limit and recipient routing;
- social service/subsystem/config Data Asset;
- dedicated social request sequence and PlayerController RPCs/delegates;
- automation tests for friends, blocking, party, party capacity, guild, security and chat.

Security boundary:
- AccountID is derived from authenticated session state;
- CharacterID is derived from active Character PlayerState;
- social role and membership state are server-authoritative;
- chat routing is server-authoritative;
- external Online Services/EOS remains an adapter concern rather than core gameplay authority.

Validation truth:
- repository/static validation passed;
- Unreal 5.8.1 UHT/UBT/Editor/PIE/network emulation/Automation Framework remain NOT VERIFIED because Unreal is unavailable in this environment;
- no CI pipeline exists to substitute for local Unreal validation.

Documentation:
- `Docs/PHASE_11_SOCIAL.md`
- `ROADMAP.md`

Next implementation target:
**PHASE 12 — Economy & Crafting**

## 6. Validation truth

### Repository/static validation

Performed:
- repository state inspected before Phase 5;
- Phase 5 source/contracts/tests added;
- character/account architecture boundaries reviewed;
- CharacterID ownership remains tied to AccountID;
- character operations are server RPCs;
- PlayerState carries replicated character identity;
- character pawn is spawned only after server-side selection;
- no persistence placeholder was presented as durable storage;
- logout cleanup removes the active character pawn and deselects the character.

### Not executable in this environment

Cannot truthfully mark PASS:
- Unreal Header Tool;
- Unreal Build Tool;
- C++ compilation against Unreal 5.8.1;
- Editor startup;
- PIE multiplayer;
- character creation/select runtime;
- possession and replication runtime;
- movement/network correction;
- Unreal Automation Framework execution;
- network emulation.

No CI checks are configured for Unreal, so GitHub cannot provide an Unreal build/test PASS.

## 7. Local Phase 5 acceptance procedure

When local Unreal validation is available:

1. Generate/update project files.
2. Run UHT.
3. Compile AgeOfAetherEditor.
4. Open Editor.
5. Start listen/server PIE.
6. Authenticate a test account.
7. Create two characters.
8. Request the character list.
9. Select one character.
10. Verify AAetherCharacter is spawned by the server.
11. Verify AAetherCharacterPlayerState replicates CharacterID/name/class/level.
12. Verify CharacterID replicates on the character actor.
13. Move the character from the client and verify server authority.
14. Test latency/loss/jitter with Unreal network emulation.
15. Attempt cross-account character selection and verify rejection.
16. Attempt simultaneous selection of two characters for one account and verify rejection.
17. Logout and verify pawn destruction and character deselection.
18. Re-authenticate and select the character again.
19. Run AgeOfAether.Character.* automation tests.

## 8. Important security/persistence boundaries

Do not:
- let the client choose CharacterID authority;
- let the client choose AccountID ownership;
- accept client lifecycle state as authoritative;
- allow selection of another account's character;
- implement database logic directly inside the Character Actor;
- treat runtime character state as persistent storage;
- mark Unreal validation complete without actually running it.

Later phases must add:
- durable character repository/backend;
- transactional persistence;
- crash recovery;
- data-driven class definitions;
- progression;
- inventory/equipment;
- combat and skills.

## 9. IP/source rule

Do not copy MU Online proprietary code, assets, maps, protocols, client binaries or protected content.

AGE OF AETHER must use original implementation, original content and original networking/persistence.

## 10. Phase 4 final hardening

Repeated login/reconnect on an already authenticated controller is explicitly rejected. This prevents a second successful authentication from replacing controller-local identity while leaving the previous session active.

## 11. Next implementation target

**PHASE 12 — Economy & Crafting**

Phase 12 is now complete at repository level. It establishes authoritative Gold wallets, shop buy/sell flows, data-driven crafting recipes, complete-output inventory preflight, economy transaction results, controller RPCs and automation coverage. Persistence remains deferred to Phase 14.

Next implementation target:
**PHASE 13 — Multiplayer & Server Authority**

## 12. Mandatory workflow

For every phase:

AUDITAR → IMPLEMENTAR → COMPILAR → TESTAR → VALIDAR → DOCUMENTAR → COMMITAR → ATUALIZAR CONTINUIDADE → PROSSEGUIR.

If a local/editor step is unavailable, record it as NOT VERIFIED rather than inventing a PASS.


## Phase 12 — Economy & Crafting

Repository implementation is complete.

Implemented:
- FAetherEconomyService;
- UAetherEconomySubsystem;
- FAetherEconomyConfigDataAsset;
- FAetherCurrency and character-owned wallet balances;
- server-authoritative Gold mutations;
- shop definitions and buy/sell transactions;
- crafting recipe definitions and transactional crafting validation;
- inventory capacity simulation before crafting consumption;
- economy transaction result/delegate;
- economy PlayerController RPCs and replay guards;
- economy automation tests.

Hardening completed during implementation:
- repaired Item Data Asset registration boundary;
- removed a literal escaped-newline artifact from AetherItemSubsystem.h;
- made economy config registration atomic;
- checked sell currency overflow before removing inventory items;
- simulated all crafting outputs together before consuming ingredients;
- restricted the currently implemented currency mutation path to Gold.

Validation truth:
- repository/static validation PASSED;
- balanced delimiters and escaped-newline audit PASSED for Phase 12 and modified source;
- Unreal 5.8.1 UHT/UBT/Editor/PIE/multiplayer/Automation Framework/network emulation NOT VERIFIED;
- no CI pipeline exists to substitute for Unreal validation.

Deliverable:
- Docs/PHASE_12_ECONOMY_CRAFTING.md

Next implementation target:
**PHASE 13 — Multiplayer & Server Authority**


## Phase 13 — Multiplayer & Server Authority

Repository implementation is complete. Added connection admission, lifecycle tracking, authentication binding, heartbeat authority, request rate limiting, timeout handling, replicated player population state and automation coverage. Unreal build/runtime/network emulation remain NOT VERIFIED locally.

Deliverable: Docs/PHASE_13_MULTIPLAYER_SERVER_AUTHORITY.md

Next implementation target: **PHASE 14 — PERSISTENCE & BACKEND**


## Phase 14 — Persistence & Backend

Repository implementation is COMPLETE at the source/repository level.

Implemented:
- FAetherPersistenceResult and versioned persistence contract;
- FAetherCharacterPersistenceSnapshot;
- revision-based optimistic concurrency;
- checksum integrity;
- snapshot validation;
- schema migration boundary;
- rollback-safe restore;
- UAetherPersistenceSaveGame;
- UAetherPersistenceSubsystem;
- alternating durable SaveGame slots;
- recovery using the highest valid storage revision;
- character identity restoration preserving CharacterID;
- inventory restore boundary and preflight;
- economy wallet restore boundary and preflight;
- quest state restore boundary and preflight;
- runtime save/load orchestration across Character, Item, Economy and Quest services;
- persistence automation tests.

Important files:
- Source/AgeOfAether/Public/Persistence/AetherPersistenceTypes.h
- Source/AgeOfAether/Public/Persistence/AetherPersistenceService.h
- Source/AgeOfAether/Private/Persistence/AetherPersistenceService.cpp
- Source/AgeOfAether/Public/Persistence/AetherPersistenceSaveGame.h
- Source/AgeOfAether/Public/Persistence/AetherPersistenceSubsystem.h
- Source/AgeOfAether/Private/Persistence/AetherPersistenceSubsystem.cpp
- Source/AgeOfAether/Private/Tests/AetherPersistenceTests.cpp
- Docs/PHASE_14_PERSISTENCE_BACKEND.md

Persistence slots:
- AetherServerPersistence_A
- AetherServerPersistence_B

Consistency model:
- save requires ExpectedRevision == CurrentRevision;
- accepted save increments the character revision;
- stale save is rejected with Conflict;
- checksum covers persistent identity, class/status, progression, derived stats, combat state, health/shield, world transform/zone, inventory, wallet and quest state;
- restore validates all dependent runtime definitions before mutation.

Validation truth:
- source/static validation PASSED;
- changed-source delimiter audit PASSED after final correction;
- literal escaped-newline audit PASSED;
- rollback-variable audit PASSED;
- Unreal 5.8.1 UHT/UBT/Editor/Automation/disk round-trip/dedicated-server persistence remain NOT VERIFIED because Unreal is unavailable in this environment;
- no CI pipeline exists to substitute for local Unreal validation.

Design boundary:
- local SaveGame is a real durable development/server adapter;
- production external database/API is not fabricated;
- FAetherPersistenceService remains the backend boundary for a future PostgreSQL/MySQL/service implementation;
- synchronous SaveGame writes are used for the current authoritative transactional foundation; Epic recommends asynchronous save/load for larger active-session saves to avoid hitches, so async production adaptation remains a deliberate future optimization. citeturn1view0

Next implementation target:
**PHASE 15 — SECURITY & ANTI-CHEAT**


## Phase 15 — Security & Anti-Cheat

Repository implementation is COMPLETE at source/repository level.

Implemented:
- FAetherSecurityTypes.h;
- FAetherSecurityService;
- FAetherSecuritySubsystem;
- FAetherSecurityConfigDataAsset;
- per-connection request token bucket;
- per-action request replay protection;
- authentication gating;
- temporary quarantine;
- suspicion score;
- bounded security audit trail;
- authoritative movement anomaly sampling during session heartbeat;
- security authorization helper in AAetherNetworkPlayerController;
- security gates on all 43 Server*_Implementation RPCs;
- multiplayer token-bucket hardening with independent LastRequestRefillAt;
- security and multiplayer automation tests.

Security model:
- all client-originated server gameplay RPCs pass through the security policy before gameplay logic;
- category-local request IDs remain independently guarded by the PlayerController;
- security replay tracking uses EAetherSecurityAction as a byte-keyed category;
- unauthenticated gameplay requests are rejected;
- authentication/session requests remain available before authentication;
- rate exhaustion returns RateLimited;
- repeated invalid behavior can trigger temporary Quarantined state;
- impossible authoritative movement samples increase suspicion and generate audit events.

Important files:
- Source/AgeOfAether/Public/Security/AetherSecurityTypes.h
- Source/AgeOfAether/Public/Security/AetherSecurityService.h
- Source/AgeOfAether/Private/Security/AetherSecurityService.cpp
- Source/AgeOfAether/Public/Security/AetherSecuritySubsystem.h
- Source/AgeOfAether/Private/Security/AetherSecuritySubsystem.cpp
- Source/AgeOfAether/Public/Security/AetherSecurityConfigDataAsset.h
- Source/AgeOfAether/Public/Networking/AetherNetworkPlayerController.h
- Source/AgeOfAether/Private/Networking/AetherNetworkPlayerController.cpp
- Source/AgeOfAether/Private/Tests/AetherSecurityTests.cpp
- Source/AgeOfAether/Private/Tests/AetherMultiplayerTests.cpp
- Docs/PHASE_15_SECURITY_ANTI_CHEAT.md

Validation truth:
- source/static delimiter audit PASSED;
- escaped-newline audit PASSED;
- all 43 server RPC security-gate coverage PASSED;
- per-action replay audit PASSED;
- multiplayer heartbeat/request-refill isolation PASSED;
- Unreal 5.8.1 UHT/UBT/Editor/Automation/PIE/network-emulation/dedicated-server attack runtime remains NOT VERIFIED because Unreal is unavailable in this environment;
- no CI pipeline exists to substitute for Unreal validation.

Design boundary:
- security service is defense-in-depth and does not replace gameplay-service authority;
- permanent bans and persistent moderation evidence are intentionally deferred to production backend/admin infrastructure;
- no external anti-cheat SDK or kernel component is fabricated;
- future production security can consume the bounded audit events and suspicion signals through a backend adapter.

Next implementation target:
**PHASE 16 — MMORPG SCALE & DEDICATED SERVER**


## Phase 16 — MMORPG Scale & Dedicated Server

Repository implementation is COMPLETE at source/repository level.

Implemented:
- dedicated server UBT target;
- restored/verified Unreal project/module foundation required for server builds;
- FAetherScaleConfig;
- FAetherServerNodeId;
- FAetherServerNodeDescriptor;
- FAetherServerLoadSnapshot;
- FAetherServerTransferRequest;
- FAetherScaleService;
- UAetherScaleSubsystem;
- UAetherScaleConfigDataAsset;
- server accepting/draining lifecycle;
- node health heartbeat;
- capacity and load scoring;
- hard-load automatic drain;
- expiring server-to-server transfer contract;
- scale automation tests.

Validation:
- repository/static validation PASSED;
- source delimiter audit PASSED;
- escaped-newline audit PASSED;
- target naming and TargetType.Server audit PASSED;
- no fake .uasset/.umap content added;
- Unreal 5.8.1 UHT/UBT/Editor/Automation/dedicated-server runtime/multi-client load/Replication Graph/World Partition remain NOT VERIFIED because Unreal is unavailable;
- no CI pipeline exists to substitute for local Unreal validation.

Design boundary:
- Phase 13 remains the connection-level admission/rate authority;
- Phase 16 adds server-node admission and lifecycle above it;
- cross-server transfer is a server-side contract only;
- production gateway/cloud/database infrastructure is not fabricated;
- Replication Graph remains a validated Unreal-stage integration point rather than an unverified dependency.

Next implementation target:
**PHASE 17 — AI/GPT INTEGRATION**


## Phase 17 — AI/GPT Integration

Repository implementation is COMPLETE at source/repository level.

Implemented:
- FAetherAIConfig;
- FAetherAIContext;
- FAetherAIRequest/Response;
- FAetherAIToolProposal;
- IAetherAIProvider;
- FAetherAIService;
- UAetherAISubsystem;
- UAetherAIConfigDataAsset;
- AI automation tests.

Security/authority:
- AI remains advisory;
- per-account rate limiting and replay protection;
- bounded context and memory;
- output safety checks;
- tool proposals cannot directly execute gameplay;
- no client API key or provider secret;
- authoritative gameplay subsystems remain the only state mutation boundary.

Validation:
- repository/static validation PASSED;
- per-account request ID isolation tested;
- no escaped-newline artifacts;
- no credentials added;
- Unreal 5.8.1 UHT/UBT/Editor/Automation/live provider/network validation NOT VERIFIED because Unreal is unavailable;
- no CI exists to substitute for local Unreal validation.

Next implementation target:
**PHASE 18 — Production & Live MMORPG**


## Phase 18 — Production & Live MMORPG

Repository implementation is COMPLETE at source/repository level.

Implemented:
- production configuration;
- liveness/readiness health;
- startup checks;
- graceful drain;
- bounded metrics;
- bounded audit events;
- production subsystem;
- Data Asset configuration;
- production automation tests;
- release/operations gate documentation.

Validation:
- repository/static validation PASSED;
- production test source corrected and re-audited;
- Unreal 5.8.1 UHT/UBT/Editor/Automation/dedicated-server/live runtime remains NOT VERIFIED because Unreal is unavailable;
- no CI pipeline exists to substitute for Unreal validation.

**NEXT:** Phase 19 — Universal Data Model & Content Registry. Detailed Phases 19–30 are defined in `ROADMAP_CONTENT_AND_CLIENT.md`; implementation must preserve server authority, data-driven content and the mandatory validation status format.
## 36. DATA-DRIVEN CONTENT & CLIENT ROADMAP — ADDED 2026-09-19

The repository roadmap phases 0–18 are complete at source/repository level. The next implementation program is defined in:

**`ROADMAP_CONTENT_AND_CLIENT.md`**

This document is now the canonical detailed plan for Phases 19–30.

### Phase 19 — Universal Data Model & Content Registry
Establish stable IDs, schema versions, metadata, registry, dependency resolution and content validation. New content must not require gameplay C++ rewrites.

### Phase 20 — Asset Pipeline & Visual Asset Registry
Establish the complete source-asset pipeline:
**FBX/source -> Unreal import -> UAsset -> asset registry -> stable Asset/Definition ID -> client presentation.**
The server sends stable gameplay/content identities rather than Unreal visual paths. Client owns visual assets and resolves IDs locally.

### Phase 21 — Complete Item, Equipment & Enhancement Data
Expand item definitions, equipment slots, restrictions, durability, binding, visuals, options and data-driven enhancement paths.

### Phase 22 — Monster, NPC, Boss & AI Content
Create configurable monster/NPC/boss definitions including stats, attacks, skills, AI profiles, loot, rewards, respawn, visuals, dialogue and services.

### Phase 23 — Skills, Effects, Buffs, Debuffs & Status
Create a reusable ability/effect system shared by players, monsters and bosses. Damage, healing, control effects, durations, stacks and cooldowns remain server-authoritative.

### Phase 24 — Loot, Drop, Reward & World Spawn
Create data-driven loot tables, weighted/guaranteed/conditional drops, spawn groups, respawn and event/world population rules.

### Phase 25 — Quest, Event, Dialogue & World Content Authoring
Expand quests into chains, branches, dialogue, timed/world objectives, rewards, reputation and configurable world events.

### Phase 26 — Crafting, Mixing, Forge & Recipe Authoring
Create the fully configurable recipe system requested by the project:
- ingredients and quantities;
- outputs and quantities;
- success percentage;
- failure behavior;
- level/skill/station requirements;
- currency;
- catalysts/protection;
- unlocks/limits.
The client displays recipes and requirements; the server validates, consumes inputs atomically, rolls the result and creates the output.

### Phase 27 — World, Map, Interaction & Streaming Content
Connect definitions to Unreal World Partition, zones, cities, PvP/safe areas, dungeons, portals, spawns, NPCs, resources, Data Layers, HLOD, NavMesh and streaming.

### Phase 28 — Client Core Architecture
Begin the actual client architecture only after the data/content contracts are stable. Planned modules include ClientCore, UI, Input, Presentation, Assets, Audio, Camera, Interaction and Effects.

### Phase 29 — Complete MMORPG UI/UX
Implement the complete client interface contract:
- Login;
- character selection/creation;
- HUD;
- inventory;
- equipment;
- skills;
- crafting;
- NPC/shop;
- quests;
- social;
- map;
- settings;
- notifications/chat.

### Phase 30 — Client Presentation, Performance & Final Integration
Connect replicated state to meshes, equipment visuals, animation, camera, VFX/SFX and UI. Validate launch through dedicated-server gameplay, combat, inventory, equipment, loot, crafting, NPCs, quests, social, persistence, reconnect and security.

### Final client/server contract

`Input -> Client Request -> Server Validation -> Authoritative Service -> State Mutation -> Persistence -> Replication -> Client Presentation -> UI/Animation/VFX`

### Final visual asset contract

`FBX/Source -> Unreal Import -> UAsset -> Asset Registry -> Stable Asset/Definition ID -> Client Resolver -> Mesh/Material/Animation/VFX/SFX`

### Final content-authoring principle

A normal new item, monster, NPC or recipe should be addable through **data + assets + validation**, without rewriting the underlying gameplay engine.

### Continuity rule for these phases

No phase may be marked complete merely because files were created. The project must continue using the mandatory status format:

- STATUS
- IMPLEMENTADO
- COMPILADO
- TESTADO
- VALIDADO
- FALHAS
- PENDÊNCIAS
- RISCOS
- PRÓXIMO PASSO

The repository still has the existing Unreal limitation: Unreal 5.8.1 UHT/UBT/Editor/PIE/Automation/dedicated-server runtime is not executable in this environment. Repository/static validation must not be represented as Unreal runtime validation.

**PHASE 19 STATUS: COMPLETE AT REPOSITORY/SOURCE LEVEL. NEXT IMPLEMENTATION TARGET: PHASE 20 — ASSET PIPELINE & VISUAL ASSET REGISTRY.**


## 37. PHASE 19 — UNIVERSAL DATA MODEL & CONTENT REGISTRY — COMPLETED 2026-09-19

Phase 19 is complete at repository/source level.

Implemented:
- `FAetherContentMetadata` stable definition identity and shared metadata;
- `FAetherContentDefinition` generic content contract;
- `FAetherContentRegistry` registration, resolution, removal, reset and deterministic enumeration;
- duplicate ID rejection;
- explicit finite/probability/non-negative numeric validation;
- missing-reference validation;
- circular dependency and self-reference detection;
- structured diagnostics;
- Unreal Automation tests for registration, normalization, duplicates, missing references, cycles, numeric validation and deterministic ordering.

Important files:
- Source/AgeOfAether/Public/Data/AetherContentTypes.h

## Phase 31 implementation status — COMPLETE at repository level
- Implemented `FAetherClassDefinition`, `FAetherClassEvolutionDefinition` and `FAetherClassRegistry`.
- Added stable normalized ClassID/EvolutionID handling, progression prerequisites, role metadata, stat modifiers, starting/granted skills/effects, equipment tags and asset references.
- Added cross-registry validation boundaries for Items, Skills/Effects, Quests, Content and Assets.
- Added seven Automation test cases covering registration, invalid definitions, duplicates, deterministic enumeration, prerequisite graph validation, cross-reference validation and reset.
- Unreal 5.8.1 UHT/UBT/Editor/PIE/Automation/Dedicated Server remains unverified.
- Next implementation target: Phase 33 — Configurable PvE/PvP Class Balance System.


## Phase 32 implementation status — COMPLETE at repository level
- Added FAetherClassCatalog with exactly 5 base classes and exactly 25 evolutions.
- Preserved requested player-facing names, path titles and evolution order.
- Stable IDs: archer, warrior, mage, tank, healer; evolutions class.01 through class.05.
- Added role identity, baseline stat metadata, descriptions and mechanic tags.
- Added eight Automation tests for counts, membership, names, progression/prerequisites, normalization, determinism, validation and isolation.
- No undefined skill/effect/item/asset references were introduced.
- Unreal 5.8.1 UHT/UBT/Editor/PIE/Automation/Dedicated Server remains unverified.
- Next implementation target: Phase 33 — Configurable PvE/PvP Class Balance System.


## Phase 33 implementation status — COMPLETE at repository level
- Added versioned external class balance format at Config/Balance/ClassBalanceProfiles.aetherbalance.
- Added Development, Testing and Production profiles with explicit versions, active profile and Production fallback.
- Added 20 PvE and 20 PvP multipliers covering damage, critical damage, defense, healing, shields, area damage, crowd control, resistance, movement/attack/cast speed, threat, lifesteal, regeneration, buff/debuff effectiveness, outgoing/incoming damage and resource economy.
- Added class/evolution balance resolution with deterministic multiplicative composition.
- Added finite-value and [0,10] safety validation and exact 45-field external row schema.
- Added eight Automation tests for parsing, PvE/PvP separation, composition, invalid values, safety, profile activation/fallback, serialization and validation.
- External config audit: 90 rows, 30 per profile, 5 base class rows + 25 evolution rows per profile, zero malformed rows.
- Initial committed values are neutral 1.0 and are not a production balance claim.
- Unreal 5.8.1 UHT/UBT/Editor/PIE/Automation/Dedicated Server remains unverified.
- Next implementation target: Phase 34 — Class/Combat Integration & Server Authority.


## Phase 34 implementation status — COMPLETE at repository level
- Added stable ClassID/EvolutionID fields to authoritative character state while retaining legacy class enum compatibility.
- Character creation and restoration preserve stable class/evolution IDs.
- Added PvE/PvP combat context and authoritative class balance application to the existing deterministic combat service.
- Added server-side class/evolution/balance resolution through FAetherClassCombatIntegration.
- Added client class/evolution claim rejection and authoritative evolution-transition validation.
- Added automation coverage for authority, PvE/PvP separation, spoof rejection, evolution transition rules and finite combat results.
- Unreal 5.8.1 UHT/UBT/Editor/PIE/Automation/Dedicated Server remains unverified.
- Next implementation target: Phase 35 — PvE/PvP Balance Simulation & Automated Validation.


## Phase 35 implementation status — COMPLETE at repository level
- Added deterministic non-mutating PvE/PvP class balance simulation.
- Simulation resolves attacker and target class/evolution modifiers independently from the server-side balance registry.
- Added finite-result and diagnostic safety ceiling validation.
- Added deterministic repeatability and extreme multiplier checks.
- Added automation coverage for neutral baseline, both contexts, deterministic output, invalid inputs, missing definitions and extreme safety.
- Simulation never mutates or promotes production balance.
- Unreal 5.8.1 compile/runtime remains unverified.
- Next implementation target: Phase 36 — Class Presentation, Assets & Runtime Acceptance.


## Phase 36 — Class Presentation, Assets & Runtime Acceptance — COMPLETE at repository level
- Added data-driven class/evolution presentation contract.
- Added deterministic presentation registry and 25-entry catalog generated from canonical evolutions.
- Added client-local class presentation subsystem with explicit load states.
- Added safe same-class fallback behavior and validation.
- Added automation tests for registry, fallback, subsystem and 25-entry catalog.
- Corrected Phase 35 simulation fixture to register the evolution rows used by its target cases.
- Extended repository validation for Phase 36 deliverables.
- Unreal 5.8.1 UHT/UBT/Automation/PIE/multiplayer/dedicated-server runtime remains NOT VERIFIED.
- Real FBX import, animations, VFX/SFX, materials, icons, sockets and performance acceptance remain local Unreal-content work.
- Next gate: execute Unreal 5.8.1 runtime acceptance and integrate real class assets.


## PHASES 37–56 — VISUAL & PLAYABLE IMPLEMENTATION ROADMAP ADDED 2026-09-20

A dedicated roadmap was added for the post-runtime implementation of the actual visual and playable Unreal game:

**ROADMAP_VISUAL_AND_PLAYABLE.md**

Sequence:
37 Unreal Visual Foundation
38 Real Asset Pipeline
39 Playable Base Character
40 Input, Movement & Camera
41 Animation
42 Equipment & Item Visuals
43 Five Classes & 25 Evolutions Presentation
44 Playable Combat
45 Skills, Buffs, Debuffs & VFX
46 Monsters, NPCs & Bosses
47 World, Maps & Streaming
48 Interaction, NPC, Quest & Events
49 Inventory, Loot & Equipment Gameplay
50 Crafting, Forge, Economy & Shops
51 Complete MMORPG UI/UX
52 Audio & Ambience
53 Multiplayer, Dedicated Server & Persistence
54 Performance, Streaming & Scale
55 Initial Complete Content Package
56 Alpha, Beta & Release Candidate

Important execution rule:
- Phase 37 starts only after the Unreal Engine 5.8.1 foundation/runtime gate is locally validated.
- A vertical slice is required before mass asset/content production.
- Each phase must be implemented, compiled, tested, validated, documented and reflected in ROADMAP.md and this continuity file.
- Visual assets never become gameplay authority.
- C++ remains the rules/authority layer, Data remains content, Blueprint/Unreal remains presentation/composition, and the server remains authoritative.
- Real FBX assets, maps, animations, VFX, SFX, UI widgets, multiplayer runtime and hardware performance must be validated in Unreal and cannot be inferred from GitHub/static CI.

Current visual/playable target after the Unreal gate:
**PHASE 37 — UNREAL VISUAL FOUNDATION**


## Phase 37 — Unreal Visual Foundation — IMPLEMENTATION COMPLETE AT REPOSITORY LEVEL

Implemented:
- explicit Enhanced Input plugin/dependency;
- UAetherGameInstance;
- UAetherAssetManager;
- UAetherVisualFoundationDataAsset and AetherVisual primary asset scan;
- AAetherCharacter camera, movement and runtime Enhanced Input bootstrap;
- AAetherDevelopmentWorldActor temporary ground;
- automatic PlayerStart for the development test world;
- AAetherFoundationHUD;
- custom trace channel names for interaction, targeting and movement;
- Phase 37 Automation tests;
- repository validation for all Phase 37 contracts.

Simplification:
- the first visual test does not require manually creating Character Blueprint, Input Actions, Mapping Context, floor mesh, PlayerStart or debug widget;
- Unreal's built-in BasicShapes is used only as temporary development ground;
- authored assets can replace the bootstrap path later without changing server authority.

Validation:
- repository/static validation: PASS when GitHub Actions completes;
- no fake .uasset/.umap binaries committed;
- Unreal 5.8.1 UHT/UBT/Editor/PIE/Automation/Dedicated Server runtime: NOT VERIFIED in this environment.

Remaining local Unreal gate:
1. open AgeOfAether.uproject in Unreal 5.8.1;
2. compile modules;
3. create Content/Aether/Maps/Aether_Development.umap;
4. set it as Game Default Map;
5. PIE single client;
6. verify WASD/mouse/Space/HUD;
7. run 2-client PIE;
8. run Dedicated Server + client;
9. run Phase 37 Automation tests;
10. check Output Log for critical errors.

Detailed report:
Docs/PHASE_37_UNREAL_VISUAL_FOUNDATION.md

Next target after the runtime gate:
PHASE 38 — REAL ASSET PIPELINE.


## Phase 38 — Real Asset Pipeline — IMPLEMENTATION COMPLETE AT REPOSITORY LEVEL

Implemented:
- asset import profile and canonical FBX pipeline contract;
- source provenance/license/version/revision metadata;
- stable AssetID normalization;
- UnrealAssetPath;
- skeleton/fallback/dependency contracts;
- dependency cycle detection;
- missing dependency/fallback validation;
- validation lifecycle: Draft, Imported, Validated, Approved, Rejected;
- /Game/Aether content structure guidance;
- Phase 38 Automation tests;
- CI validation.

Simplification for Unreal:
1. import one real skeletal FBX;
2. validate scale/orientation;
3. validate skeleton;
4. generate/check Physics Asset;
5. verify material;
6. register one stable AssetID;
7. connect it to the existing presentation system;
8. only then multiply assets.

Validation:
- repository/static CI: PASS;
- Unreal 5.8.1 UHT/UBT/Editor/import/runtime: NOT VERIFIED here;
- no fake .uasset/.umap assets added.

Detailed report:
Docs/PHASE_38_REAL_ASSET_PIPELINE.md

Next target:
**PHASE 39 — PLAYABLE BASE CHARACTER**


## Phase 39 — Playable Base Character — COMPLETE at repository/source level

Implemented:
- added UAetherPlayableCharacterVisualProfile Data Asset for skeletal mesh, animation class, material overrides and mesh transform;
- added visual profile validation with stable VisualProfileID and required skeletal mesh;
- added UAetherPlayableCharacterVisualComponent as the reusable visual bridge attached to AAetherCharacter;
- visual application is presentation-only and does not mutate authoritative gameplay state;
- Dedicated Server avoids visual asset loading;
- integrated the component directly into AAetherCharacter so ordinary character appearance changes do not require new gameplay C++;
- added Phase 39 Automation tests for profile validation and character/component integration;
- added Docs/PHASE_39_PLAYABLE_BASE_CHARACTER.md;
- updated ROADMAP.md and ROADMAP_VISUAL_AND_PLAYABLE.md.

Simplified visual workflow:
1. import one real skeletal FBX;
2. create one AetherPlayableCharacterVisualProfile;
3. assign mesh;
4. assign Animation Blueprint when available;
5. optionally assign materials;
6. assign the profile to the character Visual Component;
7. Play.

Validation truth:
- repository/source implementation complete;
- no fake .uasset/.umap/FBX/proprietary visual asset added;
- Unreal 5.8.1 UHT/UBT/Editor/PIE/Automation/Dedicated Server runtime remains NOT VERIFIED here;
- real FBX import, skeleton, Physics Asset, Character Blueprint instance, PIE, 2-client PIE, Dedicated Server + client and Output Log remain local acceptance gates.

Next implementation target:
**PHASE 40 — INPUT, MOVIMENTO E CÂMERA**



## Phase 40 — Input, Movement & Camera — COMPLETE at repository/source level

Implemented:
- UAetherMovementCameraProfile optional Data Asset;
- finite/safe validation for movement and camera tuning;
- configurable walk, sprint, jump, rotation, zoom and pitch limits;
- Enhanced Input controls: W/A/S/D, mouse, Space, Left Shift and Mouse Wheel;
- server-authoritative sprint transition using a boolean-only Server RPC;
- server-side speed selection from trusted profile/fallback;
- camera pitch clamping;
- Phase 40 Automation test;
- Docs/PHASE_40_INPUT_MOVEMENT_CAMERA.md;
- ROADMAP.md and ROADMAP_VISUAL_AND_PLAYABLE.md updated.

Low-bureaucracy visual/playable workflow:
- no mandatory authored Input Action or Mapping Context assets for the first slice;
- one optional movement/camera profile can tune the whole character;
- ordinary visual/animation assets remain data-driven and can be swapped without gameplay C++ changes.

Validation truth:
- repository/source implementation complete;
- Unreal 5.8.1 UHT/UBT/PIE/2-client/Dedicated Server runtime remains NOT VERIFIED here;
- no fake .uasset/.umap/FBX assets added.

Next implementation target:
**PHASE 41 — ANIMATION SYSTEM**


## Phase 41 — Animation System — COMPLETE at repository/source level

Implemented:
- UAetherCharacterAnimationProfile Data Asset with stable ID, optional Animation Blueprint class, thresholds and optional animation sequence references;
- UAetherBaseAnimInstance reusable across the playable character, five classes and 25 evolutions;
- Idle, Walk, Run, Jump and Fall movement states derived from CharacterMovement;
- Blueprint-facing GroundSpeed, Direction, VerticalSpeed, bIsInAir, bIsSprinting and MovementState;
- existing visual profile now accepts an AnimationProfile and automatically applies its Animation Blueprint class and profile to the base AnimInstance;
- Automation tests for identity, threshold ordering and finite numeric safety;
- animation configuration file and Phase 41 documentation;
- no fake .uasset/.umap/FBX/animation binaries.

Low-bureaucracy Unreal workflow:
1. import one skeletal FBX;
2. create one AetherCharacterAnimationProfile;
3. create one Animation Blueprint based on UAetherBaseAnimInstance;
4. create a tiny five-state locomotion graph;
5. assign the AnimationProfile to the existing AetherPlayableCharacterVisualProfile;
6. Play and reuse the same system for all classes/evolutions.

Validation truth:
- repository/source changes complete;
- GitHub Actions is the available automated gate;
- Unreal 5.8.1 UHT/UBT/Editor/PIE/Automation/2-client/Dedicated Server animation runtime remains NOT VERIFIED here;
- real FBX/skeleton/animation assets and Animation Blueprint graph require local Unreal acceptance.

Next implementation target:
**PHASE 42 — EQUIPMENT & ITEM VISUALS**


## Phase 42 implementation status — COMPLETE at repository/source level
- Added UAetherEquipmentVisualProfile with stable visual/item IDs, equipment slot, visual type, mesh references, socket, transform and material overrides.
- Added UAetherEquipmentVisualComponent with apply, remove, clear and lookup operations.
- Integrated the component into AAetherCharacter.
- Presentation is client-side and Dedicated Server avoids visual asset loading.
- Existing item definitions remain the source of item identity; gameplay authority/equipment state remains separate and will be integrated in Phase 49.
- Added Automation tests and phase documentation.
- Unreal 5.8.1 UHT/UBT/Editor/PIE/2-client/Dedicated Server runtime remains NOT VERIFIED here.
- No fake .uasset/.umap/FBX/mesh binaries.
- Next implementation target: **PHASE 43 — FIVE CLASSES & 25 EVOLUTIONS PRESENTATION**.
