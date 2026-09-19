# AGE OF AETHER — PHASE 7: PROGRESSION SYSTEM

## Status

**Repository implementation:** COMPLETE
**Repository/static validation:** PASSED
**Unreal 5.8.1 compilation/runtime:** NOT VERIFIED in this environment
**Persistence:** INTENTIONALLY DEFERRED to later backend/persistence phases

## Objective

Build the authoritative character progression layer on top of the Phase 5 Character Foundation:

- experience;
- level advancement;
- level cap;
- deterministic experience curve;
- stat points;
- server-authoritative stat allocation;
- derived-stat recalculation;
- data-driven progression configuration;
- replicated progression state;
- automation coverage.

## Implemented

### Progression contracts

FAetherProgressionConfig defines:

- maximum level: 4000 by default;
- base XP requirement: 100;
- bounded linear XP growth: 25% of base requirement per additional level;
- stat points per level: 5;
- maximum value for each base stat: 30000.

FAetherProgressionResult reports:

- result code;
- previous/new level;
- previous/new XP;
- levels gained;
- stat points granted;
- remaining unspent stat points.

### Experience

The progression service:

- rejects zero/negative XP;
- rejects invalid character identity;
- rejects disabled/deleted characters;
- rejects XP beyond the level cap;
- protects against signed 64-bit overflow;
- supports multiple level-ups in one grant;
- preserves rollover XP;
- awards stat points for each level gained;
- recalculates derived stats after progression.

XP is a server-side gameplay operation. The client has no RPC that grants itself XP.

### Level curve

The default curve is intentionally bounded:

RequiredXP(level) = BaseXP × (1 + Growth × (level - 1))

With defaults:

- level 1 → 100 XP;
- level 2 → 125 XP;
- level 3 → 150 XP;
- ...
- level 4000 has no next-level requirement.

This avoids exponential overflow at a 4000-level cap while keeping the rule data-driven.

### Stat allocation

Supported authoritative stats:

- Strength;
- Agility;
- Vitality;
- Energy;
- Command.

Allocation validates:

- positive amount;
- valid stat;
- character lifecycle;
- available unspent points;
- configured stat cap.

Derived values are recalculated immediately after a successful allocation.

### Character integration

FAetherCharacterRecord now stores:

- Level;
- Experience;
- UnspentStatPoints;
- BaseStats;
- DerivedStats.

FAetherCharacterService::ApplyProgressionState is the authoritative commit boundary.

The progression subsystem first works on a copy, validates the entire operation, then commits the resulting progression state to the Character subsystem.

### Replication

AAetherCharacterPlayerState now replicates:

- CharacterLevel;
- CharacterExperience;
- UnspentStatPoints;
- BaseStats;
- DerivedStats.

The server updates PlayerState from the authoritative Character record.

### Networking

The PlayerController exposes one client-originated progression mutation:

- AllocateStatPoints.

The server derives AccountID from the authenticated controller and CharacterID from the active Character PlayerState.

The client cannot provide authoritative:

- CharacterID;
- AccountID;
- XP;
- level;
- stat totals;
- derived stats;
- stat-point balance.

Experience is intentionally not a client RPC.

### Data-driven configuration

UAetherProgressionConfigDataAsset provides an Unreal UDataAsset representation of progression configuration.

UAetherProgressionSubsystem::ConfigureFromDataAsset validates and injects the configuration into the progression service.

This keeps progression rules out of Blueprint presentation code while allowing content/design configuration through Unreal data assets.

## Tests

Source/AgeOfAether/Private/Tests/AetherProgressionTests.cpp

Coverage:

1. default experience curve;
2. injected progression configuration;
3. invalid configuration rejection;
4. single level-up;
5. rollover XP;
6. multi-level advancement;
7. stat point generation;
8. stat allocation;
9. derived-stat recalculation;
10. overspending rejection;
11. stat-cap rejection;
12. zero/negative XP rejection;
13. zero/negative stat allocation rejection;
14. maximum-level rejection;
15. disabled-character rejection;
16. deleted-character rejection.

## Static validation

Phase 7 source audit confirmed:

- balanced braces;
- balanced parentheses;
- balanced brackets;
- no literal escaped-newline artifacts in audited files;
- CharacterID ownership remains enforced;
- progression mutation is separated from the Character service commit boundary;
- client does not grant XP;
- stat allocation is server-authoritative;
- persistence is not falsely represented as implemented.

## Unreal validation truth

The following remain NOT VERIFIED because Unreal Engine 5.8.1/UHT/UBT/Editor are unavailable in the current execution environment:

- UHT;
- C++ compilation;
- Editor startup;
- PIE;
- replication runtime;
- automation execution;
- network emulation;
- actual Data Asset creation/loading.

No CI pipeline exists that can substitute for those checks.

## Local acceptance procedure

When Unreal 5.8.1 is available:

1. Generate/update project files.
2. Run UHT.
3. Compile AgeOfAetherEditor.
4. Open the Editor.
5. Create a progression configuration Data Asset.
6. Configure the progression subsystem from that asset.
7. Run AgeOfAether.Progression.* automation tests.
8. Authenticate and select a character.
9. Grant XP from authoritative server-side gameplay/test code.
10. Verify level advancement and rollover XP.
11. Verify stat points are granted.
12. Allocate stats through the client request.
13. Verify the server rejects overspending and cap violations.
14. Verify PlayerState replication of level/XP/stat points/base/derived stats.
15. Test network latency/loss/jitter.
16. Confirm the client has no path to self-grant XP.

## Explicit boundaries

Not implemented in Phase 7:

- persistent progression database;
- death/XP-loss rules;
- quests and mission rewards;
- combat XP sources;
- party XP distribution;
- rested XP;
- XP boosters;
- prestige/rebirth;
- class-specific stat formulas;
- equipment stat modifiers;
- skills;
- achievements.

Those systems will consume the progression foundation in later phases.

## Next phase

PHASE 8 — Combat Runtime

Combat must consume Character stats and Progression state without allowing clients to author damage, cooldowns, targets or rewards.
