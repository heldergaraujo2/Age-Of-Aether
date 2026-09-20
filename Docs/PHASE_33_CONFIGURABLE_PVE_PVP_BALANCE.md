# Phase 33 — Configurable PvE/PvP Class Balance System

## STATUS
COMPLETE at repository/source level.

## IMPLEMENTADO
- Added a versioned external balance format: `Config/Balance/ClassBalanceProfiles.aetherbalance`.
- Added Development, Testing and Production profiles.
- Added profile versions and config version.
- Added explicit active profile and production fallback profile.
- Added 20 independent combat multipliers for PvE and 20 for PvP:
  damage, critical damage, defense, healing, shields, area damage, crowd control, crowd-control resistance, movement speed, attack speed, cast speed, threat, lifesteal, regeneration, buff effectiveness, debuff effectiveness, outgoing damage, incoming damage, resource cost and resource recovery.
- Added class-level and evolution-level definitions.
- Resolution composes class and evolution multipliers deterministically.
- Stable IDs are used; clients do not provide authoritative balance values.
- Numeric values are finite and constrained to [0,10].
- Profile/version duplication and malformed external rows are rejected.
- Exact external row schema is enforced: 45 pipe-delimited fields per balance row.
- External config can be edited and reloaded by the loader without recompiling combat rules; Unreal/runtime file-loading integration is intentionally reserved for the runtime validation gate.
- Initial committed values are neutral (1.0) so this phase does not silently choose production balance numbers.

## FILES
- `Source/AgeOfAether/Public/Data/AetherClassBalanceTypes.h`
- `Source/AgeOfAether/Private/Data/AetherClassBalanceTypes.cpp`
- `Source/AgeOfAether/Public/Data/AetherClassBalanceRegistry.h`
- `Source/AgeOfAether/Private/Data/AetherClassBalanceRegistry.cpp`
- `Source/AgeOfAether/Public/Data/AetherClassBalanceConfig.h`
- `Source/AgeOfAether/Private/Data/AetherClassBalanceConfig.cpp`
- `Source/AgeOfAether/Private/Tests/AetherClassBalanceTests.cpp`
- `Config/Balance/ClassBalanceProfiles.aetherbalance`

## CONFIGURATION SAFETY
- All multipliers must be finite.
- All multipliers must be in [0,10].
- Negative, NaN and infinite values are rejected by registry validation.
- Profiles require positive versions.
- Duplicate profile IDs are rejected.
- Duplicate definition IDs inside a profile are rejected.
- Active and fallback profiles must exist before activation.
- Evolution references must belong to the declared class when validated against Phase 31.
- PvE and PvP values are resolved separately.
- Explicit profile names are normalized.
- Production is the committed fallback profile.

## TESTADO
Thirteen Unreal Automation test cases:
1. external-format parsing;
2. PvE/PvP resolution separation;
3. class + evolution composition;
4. invalid multiplier rejection;
5. numeric safety bounds;
6. profile duplication/activation/fallback safety;
7. serialization round trip;
8. zero registry validation issues.
9. cross-registry class/evolution validation;
10. active-profile resolution with fallback;
11. exact external schema rejection;
12. class/evolution reference mismatch rejection;
13. external-file load failure safety.

Static external-config audit:
- 90 balance rows;
- 30 rows per profile;
- 5 class rows + 25 evolution rows per profile;
- 30 unique definition IDs per profile;
- 0 malformed rows;
- profiles: development, testing, production.

## VALIDADO
- The external file uses the exact parser schema.
- All committed initial multipliers are 1.0.
- The production profile is neutral and is configured as fallback.
- No balance values are hard-coded into the combat calculation pipeline by this phase.
- Phase 31 class/evolution architecture remains the identity source; Phase 33 only tunes modifiers.

## COMPILADO
Not verified. Unreal Engine 5.8.1 UHT/UBT/Editor is not executable in the current environment.

## FALHAS
No known repository-level defect after source review and external-file structural audit.

## PENDÊNCIAS
- Phase 34: connect resolved balance to authoritative combat calculations.
- Phase 35: deterministic PvE/PvP simulation and regression validation.
- Runtime loading of the external file in the Unreal process and hot/reload policy must be verified with Unreal 5.8.1.
- Production activation must later be guarded by the server/configuration deployment workflow.

## RISCOS
- Unreal-specific compiler/API integration remains unverified.
- The initial values are neutral placeholders, not a claim that the five classes are already balanced.
- Multipliers up to 10 are a safety envelope, not recommended production values.
- Combining class and evolution multipliers multiplicatively can amplify values; Phase 35 must detect undesirable combinations.

## PRÓXIMO PASSO
Phase 34 — Class/Combat Integration & Server Authority.
