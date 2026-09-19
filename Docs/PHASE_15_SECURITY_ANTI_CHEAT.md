# PHASE 15 — SECURITY & ANTI-CHEAT

## Status

**Repository implementation:** COMPLETE  
**Static/source validation:** PASS  
**Unreal 5.8.1 UHT/UBT/Editor/Automation runtime:** NOT VERIFIED in this environment

## Objective

Establish a reusable server-side security layer around every client-originated gameplay RPC while preserving the existing server-authoritative architecture.

Epic's UE 5.8 networking documentation states that the server owns authoritative gameplay state and that server RPC inputs must be verified against server rules. UE 5.8 also documents RPC validation and network emulation as important tools for identifying invalid behavior and exploits. citeturn1search0turn1search11turn0search0

## Implemented

### Security policy

FAetherSecurityConfig provides data-driven limits for:

- requests per second;
- burst capacity;
- invalid-request threshold;
- quarantine duration;
- maximum movement speed used by anomaly sampling;
- movement grace period.

A matching UAetherSecurityConfigDataAsset establishes the content/configuration boundary.

### Security service

FAetherSecurityService provides:

- per-connection request token bucket;
- category-aware request replay protection;
- authentication gating;
- quarantine state;
- suspicion score;
- suspicious-event recording;
- authoritative movement anomaly sampling;
- bounded in-memory security audit trail.

### RPC protection

All 43 server implementations in AAetherNetworkPlayerController are now gated through the security policy.

The existing per-category LastProcessed request guards remain in place. The new security layer additionally tracks request IDs independently per action category, so numeric ID 1 in Combat does not incorrectly collide with numeric ID 1 in Inventory.

Protected categories:

- Authentication
- Session
- Character
- Inventory
- Progression
- Combat
- World
- Quest
- Social
- Economy
- Generic networking

### Quarantine

Repeated invalid behavior can move a connection into a temporary quarantine.

While quarantined, gameplay requests are rejected until the configured quarantine period expires.

The system intentionally does not permanently ban a player based solely on a local suspicion score. Permanent moderation/ban decisions belong to a future administrative/security backend with persistent evidence.

### Movement anomaly detection

The security service samples the server-authoritative pawn location during authenticated session heartbeats.

It calculates the maximum permitted distance from the previous authoritative sample using:

MaxMovementSpeedUnitsPerSecond * (DeltaTime + MovementGraceSeconds)

An impossible displacement produces a suspicious audit event and increases the connection suspicion score.

This is an anomaly signal, not the movement simulation itself. Unreal's Character Movement system remains responsible for authoritative movement correction; Epic documents the server-side movement reproduction and correction model separately. citeturn1search10

### Multiplayer limiter hardening

Phase 13's token bucket contained a subtle refill weakness: request refill was anchored to heartbeat time, so a heartbeat could influence the refill basis.

It is now split into:

- LastHeartbeatAt
- LastRequestRefillAt

Heartbeat activity no longer creates additional request capacity.

## Tests

Added:

Source/AgeOfAether/Private/Tests/AetherSecurityTests.cpp

Coverage:

1. security configuration validation;
2. rate limiting;
3. authentication gate;
4. temporary quarantine;
5. movement anomaly detection;
6. suspicion scoring;
7. audit trail;
8. per-action replay protection.

Extended multiplayer tests:

- heartbeat cannot refill request budget;
- elapsed request time is the only refill source.

## Static validation

Performed:

- all security source delimiter balance: PASS;
- controller delimiter balance after 43 RPC gate injections: PASS;
- escaped-newline audit: PASS;
- server RPC security-gate coverage: 43/43: PASS;
- per-action replay-key audit: PASS;
- multiplayer token refill separation audit: PASS;
- test file presence/coverage inspection: PASS.

## Unreal validation limitation

This environment does not contain an executable Unreal Engine 5.8.1 toolchain.

Therefore these remain NOT VERIFIED:

- UHT;
- UBT;
- C++ compilation against UE 5.8.1;
- Editor startup;
- Automation Framework execution;
- PIE multiplayer runtime;
- real network-emulation execution;
- dedicated-server attack simulation;
- runtime disconnect/quarantine behavior.

Epic documents PIE multiplayer and Network Emulation for exactly these runtime tests, including simulated latency, packet loss, jitter and packet reordering. citeturn0search1turn0search0

## Security boundaries

The phase does not trust:

- client-authored CharacterID;
- client-authored AccountID;
- client-authored damage;
- client-authored currency;
- client-authored inventory ownership;
- client-authored quest completion;
- client-authored world coordinates;
- client-authored critical-hit outcomes.

Existing gameplay services continue to be the authoritative decision points.

The security service adds a cross-cutting defense layer; it does not replace gameplay validation.

## Production anti-cheat boundary

The following are deliberately deferred:

- persistent ban database;
- hardware fingerprinting;
- kernel-level anti-cheat;
- external anti-cheat SDK;
- machine-learning detection;
- cross-server reputation;
- GM moderation UI;
- evidence export pipeline;
- distributed security event ingestion.

Those features require production infrastructure and legal/privacy decisions and must not be fabricated inside the gameplay repository.

## Runtime attack test plan

When Unreal is available:

1. run dedicated server;
2. connect multiple clients;
3. spam reliable RPCs;
4. repeat old request IDs;
5. send requests before authentication;
6. attempt cross-account character operations;
7. attempt inventory ownership manipulation;
8. attempt invalid economy quantities;
9. attempt impossible movement;
10. enable network emulation with high latency/loss/jitter;
11. inspect security audit events;
12. verify quarantine;
13. verify normal operation resumes after quarantine;
14. verify legitimate traffic is not permanently penalized.

Epic recommends harsh network conditions during multiplayer testing; its UE 5.8 documentation specifically describes latency, packet loss, jitter and packet ordering controls. citeturn0search0turn0search3

## Acceptance

### Repository
- security service: PASS
- security subsystem: PASS
- 43/43 server RPC security gates: PASS
- replay protection: PASS
- rate limiting: PASS
- quarantine: PASS
- anomaly scoring: PASS
- movement anomaly sampling: IMPLEMENTED
- bounded audit trail: PASS
- multiplayer limiter hardening: PASS
- automation coverage: PASS

### Unreal
- UHT: NOT VERIFIED
- UBT: NOT VERIFIED
- Automation Framework: NOT VERIFIED
- PIE attack tests: NOT VERIFIED
- network emulation: NOT VERIFIED
- dedicated-server attack simulation: NOT VERIFIED

## Next phase

**PHASE 16 — MMORPG SCALE & DEDICATED SERVER**
