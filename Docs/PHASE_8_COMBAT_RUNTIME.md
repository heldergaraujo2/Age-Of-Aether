# AGE OF AETHER — PHASE 8: COMBAT RUNTIME

## Status

**Repository implementation:** COMPLETE  
**Repository/static validation:** PASSED  
**Unreal 5.8.1 compile/runtime validation:** NOT VERIFIED in this environment  
**Persistence:** intentionally deferred

Phase 8 establishes the server-authoritative basic combat runtime without introducing skills, equipment modifiers, loot rewards or persistence prematurely.

## Architecture

Combat follows:

`Request → Validate → Target → Range → Cooldown → Accuracy → Damage → Defense → Resistance → Critical → Shield/HP → Death → Replication → Audit Result`

The client sends only a target CharacterID. The server derives the attacker CharacterID from the authenticated PlayerState and derives attacker/target stats and positions from authoritative character state.

Implemented boundaries:

- `FAetherCombatService`: deterministic combat rules and cooldown state.
- `UAetherCombatSubsystem`: GameInstance service boundary and CharacterService commit boundary.
- `FAetherCombatConfig`: data-driven combat parameters.
- `UAetherCombatConfigDataAsset`: authoring contract for combat configuration.
- `FAetherCombatResult`: authoritative result/audit payload.
- `AAetherNetworkPlayerController`: server attack RPC and client result event.
- `AAetherCharacterPlayerState`: replicated health, shield and combat state.
- `FAetherCharacterRecord`: authoritative health/shield/combat state.
- `FAetherCharacterService::ApplyCombatState`: authoritative state commit.

## Rules

### Target validation

The server rejects:

- invalid attacker/target IDs;
- self-targeting;
- inactive attackers;
- inactive targets;
- dead attackers;
- dead targets;
- attackers not owned by the authenticated account.

The client cannot submit damage, attack stats, health, defense, resistance, cooldown state or attacker identity.

### Range

Default maximum basic-attack range is 250 Unreal units.

Distance is calculated from authoritative server-side character locations.

### Cooldown

Default basic-attack cooldown is 1 second.

Cooldown state is maintained server-side per CharacterID. Invalid requests do not consume the cooldown; a processed miss does.

### Accuracy

Accuracy is configured from 0.0 to 1.0.

The server derives a deterministic roll from attacker ID, target ID and server-validated request sequence. No client-provided random result is trusted.

### Damage

Base damage is sampled deterministically between authoritative AttackMin and AttackMax.

Defense uses:

`DamageAfterDefense = RawDamage × DefenseMitigationScale / (DefenseMitigationScale + Defense)`

### Resistance

Resistance is an authoritative derived stat, currently derived from Energy and clamped to 75%.

`DamageAfterResistance = DamageAfterDefense × (1 - Resistance / 100)`

### Critical

Critical chance and multiplier are server configuration.

Critical results are derived server-side and cannot be forced by the client.

### Shield and health

Damage consumes shield before health.

Health is clamped to [0, MaxHealth].

When health reaches zero:

- combat state becomes Dead;
- further attacks against the target are rejected as TargetDead.

No automatic respawn is introduced in Phase 8.

### Replication

Health, shield and combat state replicate through Character PlayerState.

The server updates the affected PlayerState from authoritative CharacterService state. This keeps critical state in replicated properties rather than using multicast RPCs for persistent health.

Unreal's multiplayer model is server-authoritative, and replicated properties are the appropriate mechanism for persistent state synchronization; RPCs are used here only for the client request and request-specific result.

## Files

New:

- `Source/AgeOfAether/Public/Combat/AetherCombatTypes.h`
- `Source/AgeOfAether/Public/Combat/AetherCombatService.h`
- `Source/AgeOfAether/Private/Combat/AetherCombatService.cpp`
- `Source/AgeOfAether/Public/Combat/AetherCombatSubsystem.h`
- `Source/AgeOfAether/Private/Combat/AetherCombatSubsystem.cpp`
- `Source/AgeOfAether/Public/Combat/AetherCombatConfigDataAsset.h`
- `Source/AgeOfAether/Private/Tests/AetherCombatTests.cpp`

Updated:

- Character types/service/subsystem;
- Character PlayerState;
- Network PlayerController.

## Automated test coverage

Added tests for:

1. basic hit;
2. defense mitigation;
3. resistance mitigation;
4. shield absorption;
5. critical hit;
6. accuracy miss;
7. cooldown rejection and recovery;
8. range rejection;
9. self-target rejection;
10. dead-target rejection;
11. lethal damage/death transition;
12. invalid combat configuration.

These are Unreal Automation Framework tests. They were statically inspected but could not be executed without a local Unreal 5.8.1 environment.

## Local acceptance procedure

When Unreal 5.8.1 is available:

1. Generate/update Visual Studio project files.
2. Run UHT/build Development Editor.
3. Open the project.
4. Create a `UAetherCombatConfigDataAsset` and configure it if desired.
5. Inject the config through `UAetherCombatSubsystem::ConfigureFromDataAsset`.
6. Run `AgeOfAether.Combat.*` automation tests.
7. Authenticate two accounts.
8. Create/select two active characters.
9. Place them within range.
10. Call `BasicAttack(TargetCharacterID)`.
11. Verify the server calculates damage.
12. Verify health/shield/combat state replicate.
13. Verify repeated attacks before cooldown are rejected.
14. Verify out-of-range attacks are rejected.
15. Verify dead targets cannot be attacked.
16. Enable network emulation for latency/loss/jitter.
17. Verify clients cannot alter damage, attacker identity, target health, cooldown or stats.
18. Run the full Automation Framework suite.

Unreal documents that server RPCs execute on the server when called by the owning client, while replicated properties synchronize authoritative state to clients. Unreal's multiplayer programming examples also use replicated health for server-side damage state.

## Explicitly deferred

- equipment-derived modifiers;
- weapon attack types;
- skills;
- buffs/debuffs and periodic effects;
- crowd control;
- elemental resistance types;
- hit reactions/animations/VFX;
- projectile and hit-scan collision;
- AI combat;
- XP/reward generation from kills;
- loot/drop generation;
- combat persistence;
- combat history database;
- respawn rules.

These belong to later phases and will consume the Phase 8 contracts rather than replacing them.
