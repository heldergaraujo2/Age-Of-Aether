# Phase 34 — Class/Combat Integration & Server Authority

## STATUS
COMPLETE at repository/source level.

## IMPLEMENTADO
- Added stable ClassID and EvolutionID to the authoritative character record while retaining the legacy enum for compatibility.
- Character creation initializes stable IDs for the five current archetypes; persisted characters preserve the IDs and safely default legacy records to warrior / warrior.01 when the fields are absent.
- Added EAetherCombatMode (PvE/PvP) and FAetherCombatBalanceContext.
- Added an authoritative combat-resolution entry point that rejects non-authoritative balance contexts.
- Existing deterministic combat now applies attacker Damage * OutgoingDamage, critical-damage scaling, target Defense, and target IncomingDamage modifiers.
- Added finite-result protection before combat state mutation.
- Added FAetherClassCombatIntegration to resolve class/evolution identity and active/fallback balance server-side.
- Added authoritative evolution-transition validation: same class, later stage, required level and declared prerequisite.
- Added client-claim validation; mismatched class/evolution claims are rejected rather than trusted.
- Character class/evolution state is part of the replicated/persisted FAetherCharacterRecord contract.
- No client-supplied multiplier is accepted as authoritative.

## TESTADO
Added repository-level Unreal Automation coverage for authoritative class/evolution resolution, PvE/PvP separation, client class/evolution spoof rejection, invalid class rejection, evolution transition authorization, insufficient-level rejection, cross-class rejection, class/evolution damage application and finite-result protection.

Existing combat tests remain in place for range, cooldown, accuracy, criticals, defense/resistance, shields and deterministic damage.

## VALIDADO
- Stable class IDs match Phase 32: archer, warrior, mage, tank, healer.
- Evolution IDs use class.01 through class.05.
- Combat resolution uses the server-side balance registry; the client has no balance-setting API in this integration.
- PvE/PvP values are selected by server context.
- Class/evolution identity is resolved against FAetherClassRegistry.
- Balance fallback remains delegated to Phase 33 ResolveActive.
- State mutation happens only after validation and finite-damage checks.
- Extreme fantasy mechanics remain data/configuration concerns; this phase does not introduce unconditional map-wide attacks or permanent group immortality.

## COMPILADO
Not verified. Unreal Engine 5.8.1 UHT/UBT/Editor/Automation/Dedicated Server is not executable in the current environment.

## FALHAS
No known repository-level defect identified during source/static review.

## PENDÊNCIAS
- Unreal 5.8.1 compile and runtime acceptance.
- Full network RPC wiring of class-aware combat into a live dedicated-server session.
- Equipment/skill/effect execution integration can consume the same authoritative class identity contract; those systems remain governed by their existing data registries.
- Phase 35 deterministic PvE/PvP simulation and regression validation.

## RISCOS
- Unreal compiler/API integration is unverified in this environment.
- Existing legacy character enum and new stable IDs must remain synchronized until the enum is fully retired.
- Balance composition is multiplicative; Phase 35 must exercise extreme combinations.
- Runtime configuration loading/hot reload remains a separate Unreal deployment concern.

## PRÓXIMO PASSO
Phase 35 — PvE/PvP Balance Simulation & Automated Validation.
