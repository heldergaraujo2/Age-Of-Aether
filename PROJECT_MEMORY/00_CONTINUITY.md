# AGE OF AETHER — CONTINUITY / HANDOFF

> Canonical handoff for continuing AGE OF AETHER. Read this before making project decisions.

## 1. Project identity

**Name:** AGE OF AETHER  
**Technical project:** AgeOfAether  
**Repository:** https://github.com/heldergaraujo2/Age-Of-Aether  
**Branch:** main  
**Target Unreal:** 5.8.1  
**Current stage:** Phase 6 — Item and Inventory System (repository implementation complete; Unreal compilation/network runtime validation pending local validation)  
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

**PHASE 7 — Progression System**

The next system will build authoritative experience, levels and stat allocation on the existing Character foundation and integrate progression requirements with the item foundation without moving authority to the client.

## 12. Mandatory workflow

For every phase:

AUDITAR → IMPLEMENTAR → COMPILAR → TESTAR → VALIDAR → DOCUMENTAR → COMMITAR → ATUALIZAR CONTINUIDADE → PROSSEGUIR.

If a local/editor step is unavailable, record it as NOT VERIFIED rather than inventing a PASS.
