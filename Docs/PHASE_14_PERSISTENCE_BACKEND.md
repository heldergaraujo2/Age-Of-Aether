# PHASE 14 — PERSISTENCE & BACKEND

## Status

**Repository implementation:** COMPLETE  
**Static/source validation:** PASS  
**Unreal 5.8.1 UHT/UBT/Editor/Automation runtime:** NOT VERIFIED in this environment  
**External production database:** NOT CONNECTED; provider credentials/infrastructure are intentionally not invented

## Objective

Establish a real server-side persistence boundary so authoritative character state can survive process/world restarts without putting database logic inside gameplay actors.

The implementation uses Unreal's native USaveGame serialization as the durable development/server adapter. Epic documents custom USaveGame classes as the native save/load mechanism and provides synchronous and asynchronous slot APIs; Epic recommends asynchronous save/load for larger active-session saves to avoid hitches. AGE OF AETHER keeps the repository boundary independent so this local adapter can later be replaced by an external backend without rewriting gameplay rules. citeturn1view0

## Implemented

### Persistence contract

- EAetherPersistenceResult
- schema version 1
- FAetherCharacterPersistenceSnapshot
- revision number per character
- saved-at timestamp
- account/character identity binding
- character progression/combat/world state
- inventory snapshot
- economy wallet snapshot
- quest state snapshot
- checksum

### Persistence service

FAetherPersistenceService provides:

- optimistic-concurrency save
- revision conflict rejection
- snapshot import
- snapshot restore for rollback
- load
- delete
- deterministic snapshot enumeration
- schema migration hook
- snapshot validation
- checksum generation/verification

### Durable server adapter

UAetherPersistenceSubsystem uses two alternating save slots:

- AetherServerPersistence_A
- AetherServerPersistence_B

The active slot is selected by storage revision. A failed write does not replace the previous in-memory state. This gives a basic crash-tolerant write pattern without pretending it is a distributed database.

### Runtime integration

The persistence boundary can capture and restore:

- CharacterService
- ItemService
- EconomyService
- QuestService

Restore is preflighted before mutating runtime state:

1. snapshot integrity;
2. inventory definitions/ownership/stack validation;
3. wallet validation;
4. quest definition/objective validation;
5. character identity restoration;
6. inventory restoration;
7. wallet restoration;
8. quest restoration.

Character identity restoration preserves the persistent CharacterID instead of generating a new identity.

## Security and consistency

The server remains authoritative.

The persistence layer rejects:

- invalid account/character identity;
- cross-account character ownership;
- duplicate inventory instance IDs;
- invalid inventory slots;
- item ownership mismatch;
- invalid item quantities;
- negative currency;
- duplicate currency entries;
- duplicate quest IDs;
- invalid quest objective progress;
- invalid schema;
- invalid checksums;
- stale revision writes.

The client has no persistence RPC that can directly author a saved snapshot.

## Transaction model

Character saves use optimistic concurrency:

ExpectedRevision == CurrentRevision

If the revision is stale, the save returns Conflict and leaves the stored snapshot unchanged.

Durable writes use alternating slots:

current slot → write other slot → promote other slot

If the write fails, the runtime snapshot is restored to its previous version.

## Schema evolution

AETHER_PERSISTENCE_SCHEMA_VERSION is the canonical version.

MigrateSnapshot() is the migration boundary. Version 0 is normalized to version 1; unsupported versions are rejected.

Future migrations must explicitly transform old fields. Persistent data must never be silently reinterpreted.

## Tests added

Source/AgeOfAether/Private/Tests/AetherPersistenceTests.cpp

Coverage:

1. save/load round trip;
2. revision creation;
3. optimistic-concurrency conflict;
4. second revision commit;
5. checksum verification;
6. tampered snapshot rejection;
7. ownership tamper rejection;
8. slot-range validation;
9. negative-currency rejection;
10. cross-account snapshot rejection;
11. schema migration;
12. delete;
13. repeated delete rejection;
14. deterministic snapshot enumeration.

## Static validation performed

- changed-source delimiter audit: PASS;
- changed-source literal escaped-newline audit: PASS;
- persistence subsystem rollback-variable audit: PASS;
- repository diff inspection: PASS;
- persistence service brace/parenthesis/bracket balance: PASS after final correction;
- no fake database connection or fake Unreal asset was introduced.

## Unreal validation limitation

This environment does not contain an executable Unreal Engine 5.8.1 installation/toolchain, so the following cannot truthfully be marked PASS here:

- UHT;
- UBT;
- C++ compilation against UE 5.8.1;
- Editor startup;
- Unreal Automation Framework execution;
- SaveGame disk round-trip in a running server;
- multiplayer persistence/reconnect runtime;
- dedicated-server persistence runtime.

These remain explicit local acceptance gates.

## Production backend boundary

The repository is now backend-ready through a persistence service boundary, but this phase does not invent a cloud/database provider.

A future external repository can implement the same persistence contract for:

- PostgreSQL/MySQL;
- a service API;
- transactional database writes;
- connection pooling;
- retries;
- backup/restore;
- migrations;
- distributed locks;
- shard-aware storage.

The local USaveGame adapter is therefore a real durable development/server implementation, not a claim that production database infrastructure already exists.

## Acceptance criteria

### Repository
- implementation exists: PASS
- persistence is durable through SaveGame slots: IMPLEMENTED
- optimistic concurrency: PASS by source/tests
- integrity validation: PASS by source/tests
- rollback boundary: IMPLEMENTED
- schema versioning: IMPLEMENTED
- runtime save/load adapters: IMPLEMENTED

### Local Unreal
- UHT: NOT VERIFIED
- UBT: NOT VERIFIED
- Automation Framework: NOT VERIFIED
- actual disk save/load: NOT VERIFIED
- dedicated-server restart recovery: NOT VERIFIED

## Next phase

**PHASE 15 — SECURITY & ANTI-CHEAT**
