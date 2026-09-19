# AGE OF AETHER — CONTINUITY / HANDOFF

> Canonical handoff for continuing AGE OF AETHER. Read this before making project decisions.

## 1. Project identity

**Name:** AGE OF AETHER  
**Technical project:** AgeOfAether  
**Repository:** https://github.com/heldergaraujo2/Age-Of-Aether  
**Branch:** main  
**Target Unreal:** 5.8.1  
**Current stage:** Phase 4 — Accounts and Sessions (repository implementation complete; Unreal compilation/network runtime validation pending local validation)  
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

## 5. Phase 4 tests

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

## 6. Validation truth

### Repository/static validation

Performed:
- repository state inspected;
- Phase 3 source inspected before Phase 4;
- Phase 4 source/contracts/tests added;
- architecture boundaries reviewed;
- no plaintext password storage introduced;
- no database placeholder presented as persistence;
- request IDs are separated between generic networking and account/session operations.
- repeated authentication/reconnect requests are rejected while the controller already owns an authenticated session, preventing accidental session replacement.

### Not executable in this environment

Cannot truthfully mark PASS:
- Unreal Header Tool;
- Unreal Build Tool;
- C++ compilation against Unreal 5.8.1;
- Editor startup;
- PIE multiplayer;
- client/server authentication;
- reconnect runtime;
- heartbeat timeout runtime;
- Unreal Automation Framework execution;
- network emulation.

No CI checks are configured for Unreal, so GitHub cannot provide an Unreal build/test PASS.

## 7. Local Phase 4 acceptance procedure

When local Unreal validation is available:

1. Generate/update project files.
2. Run UHT.
3. Compile AgeOfAetherEditor.
4. Open Editor.
5. Start listen/server PIE.
6. Configure a development credential verifier from test/bootstrap code.
7. Register a test account on the authoritative server.
8. Authenticate from a client.
9. Verify accepted response/session ID.
10. Verify duplicate login rejection.
11. Heartbeat the active session.
12. Stop heartbeats and verify timeout.
13. Reconnect before timeout and verify acceptance.
14. Reconnect after timeout and verify SessionExpired.
15. Disable account and verify authentication rejection.
16. Logout and verify Closed.
17. Attempt operations with mismatched session IDs and verify rejection.
18. Run AgeOfAether.Accounts.* automation tests.
19. Repeat critical flows with Unreal network emulation.

## 8. Important security/persistence boundaries

Do not:
- store plaintext passwords;
- let the client choose AccountID authority;
- let the client choose SessionID authority;
- accept client permission values as authoritative;
- implement database logic directly inside gameplay/controller classes;
- treat runtime account registry as persistent storage;
- mark Unreal validation complete without actually running it.

Later phases must add:
- persistent account repository/backend;
- secure credential storage/provider;
- rate limiting;
- brute-force/abuse controls;
- audit/security logging;
- dedicated server;
- crash recovery;
- production persistence.

## 9. IP/source rule

Do not copy MU Online proprietary code, assets, maps, protocols, client binaries or protected content.

The supplied Item.txt and public documentation are references for data modeling/architecture only.

AGE OF AETHER must use original implementation, original content and original networking/persistence.

## 10. Final Phase 4 hardening

Repeated login/reconnect on an already authenticated controller is explicitly rejected. This prevents a second successful authentication from replacing controller-local identity while leaving the previous session active.

## 11. Next implementation target

**PHASE 5 — Character Foundation**

Implement:
- CharacterID;
- account-to-character relation;
- character name;
- class;
- location;
- status;
- base stats;
- derived-stat foundation;
- character lifecycle;
- server-authoritative character ownership and validation.

The character system must consume the authenticated account/session boundary created in Phase 4. Do not move character authority to the client.

## 12. Mandatory workflow

For every phase:

AUDITAR → IMPLEMENTAR → COMPILAR → TESTAR → VALIDAR → DOCUMENTAR → COMMITAR → ATUALIZAR CONTINUIDADE → PROSSEGUIR.

If a local/editor step is unavailable, record it as NOT VERIFIED rather than inventing a PASS.
