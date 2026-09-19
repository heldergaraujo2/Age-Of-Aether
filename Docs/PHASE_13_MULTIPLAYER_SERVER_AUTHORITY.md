# PHASE 13 — MULTIPLAYER & SERVER AUTHORITY

## Status

Repository implementation: COMPLETE.
Repository/static validation: PASSED.
Unreal 5.8.1 build/runtime validation: NOT VERIFIED because this environment has no Unreal installation and the repository has no CI Unreal build.

## Purpose

Phase 13 hardens the existing networking foundation into a server-authority boundary for a real multiplayer runtime.

Unreal's networking model uses client/server authority: the server owns the authoritative game state and clients receive replicated state while requesting actions through RPCs.

## Implemented

### Multiplayer authority service

Added:
- FAetherMultiplayerConfig
- FAetherMultiplayerConnection
- FAetherMultiplayerService
- UAetherMultiplayerSubsystem
- UAetherMultiplayerConfigDataAsset

The runtime service owns:
- connection admission;
- maximum concurrent player count;
- connection lifecycle;
- authenticated connection state;
- heartbeat timestamps;
- request budget/rate limiting;
- timeout detection;
- clean shutdown.

### Admission control

The authoritative GameMode now:
- registers every accepted PlayerController connection;
- rejects connections after configured capacity;
- removes connections during GameMode logout;
- publishes authoritative player count;
- advances authoritative state revision when population changes.

Default runtime capacity is 1000 players and is configuration-driven.

### Authentication binding

Existing account authentication/reconnect paths synchronize authenticated state with the multiplayer authority registry.

Logout clears the connection's authenticated state.

Heartbeat is validated against the multiplayer authority registry before the account session heartbeat is accepted.

### Request rate limiting

The multiplayer authority service provides:
- configurable requests per second;
- configurable burst capacity;
- per-connection budget;
- server-side refill;
- unauthenticated request rejection;
- rate-limit result.

This infrastructure guard does not replace per-system gameplay validation.

### Replicated server state

AAetherNetworkGameState now replicates:
- protocol version;
- authoritative state revision;
- connected player count;
- whether the server is accepting connections.

GameState is a replicated representation; the service/GameMode remain authoritative.

### Dedicated-server readiness

No client-only authority was introduced into the gameplay foundation.

Unreal's dedicated-server model is server-authoritative and separates the server from player-facing rendering/input.

Actual dedicated-server compilation and launch remain an Unreal/local validation gate.

## Security boundary

The client cannot:
- decide admission;
- decide player capacity;
- mark its connection authenticated;
- reset its request budget;
- remove another connection;
- change server population state;
- advance authoritative state revision;
- declare its own server authority.

The server resolves connection identity from the PlayerController connection and authenticated session.

## Tests

Repository automation coverage includes:
1. valid/invalid multiplayer configuration;
2. admission;
3. maximum player capacity;
4. duplicate connections;
5. connection removal and slot reuse;
6. authentication state;
7. request burst capacity;
8. request rate limiting;
9. request budget refill;
10. heartbeat;
11. timeout removal;
12. service shutdown.

## Unreal validation required locally

- two or more PIE clients;
- dedicated server;
- replication inspection;
- disconnect/reconnect;
- packet loss;
- latency;
- jitter;
- out-of-order packets;
- simultaneous requests;
- server restart;
- Automation Framework execution.

Epic's UE 5.8 multiplayer testing tools support multiple players and a separate dedicated server. Network emulation supports latency, loss, jitter and packet ordering.

## Scope boundary

Deferred:
- production Replication Graph configuration;
- Iris production configuration;
- authoritative movement reconciliation;
- lag compensation;
- cross-server transfer;
- server discovery/matchmaking;
- distributed session state;
- database-backed connection state;
- anti-cheat;
- shard/world orchestration;
- dedicated-server deployment automation.

## Validation truth

Repository/static:
- source structure audited;
- delimiters balanced;
- no literal escaped-newline artifacts in audited Phase 13/modified files;
- authority service tests added;
- GameMode lifecycle hooks added;
- authentication/heartbeat integration added;
- replicated server admission state added;
- data-driven authority configuration added.

Unreal runtime: NOT VERIFIED.

No CI status is reported for the final repository commits, so no CI result is represented as a build/test pass.

## Next phase

PHASE 14 — PERSISTENCE & BACKEND
