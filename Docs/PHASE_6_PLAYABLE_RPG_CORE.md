# PHASE 6 — NÚCLEO RPG JOGÁVEL

**SOURCE/DOCUMENTATION: COMPLETE.**
**UNREAL RUNTIME: PENDING LOCAL EXECUTION.**

This phase defines the canonical playable RPG loop: character → level/XP → attributes → HP/resource → basic attack → damage → death/respawn → equipment → inventory → loot.

No fake Unreal binary assets are introduced.

## Scope

Reuse existing architecture. Do not create duplicate inventory, equipment, progression, combat, item, or persistence systems.

The player state includes identity, level, XP, attributes, HP/resource, alive/dead state, respawn state, equipment, and inventory. XP and level are authoritative and deterministic. Attributes and derived values have one authoritative calculation path. HP/resources remain within valid bounds.

Basic combat follows attacker intent → server validation → damage calculation → target state update → death. The client cannot authoritatively mutate health. Death and respawn are idempotent and leave valid player state.

Equipment follows inventory item → validation → equipped slot/state → derived contribution. Inventory requires authoritative add/remove, ownership validation, valid quantities, and no duplication. Loot follows authoritative reward event → validated generation → inventory transaction → observable result, with duplicate-claim prevention.

XP, level-up, damage, death, respawn, item grant/removal, equip/unequip and loot acquisition are authoritative state transitions. Failed transactions must not leave partial state.

## Phase 5 integration

The intended loop is: spawn → village/plaza → services → road → exploration → combat-ready area → basic combat → XP/loot → inventory/equipment → continue.

The contract remains compatible with the future enemy/AI phase, dungeon phase, class/evolution progression, multiplayer, and persistence. No final balance curve or death penalty is fixed here.

## Automation contract

Acceptance should cover XP, level thresholds, invalid progression, attribute calculations, HP/resource bounds, valid/invalid damage, death/respawn, item grant/removal, equip/unequip consistency, loot atomicity, duplicate reward prevention, authority boundaries, persistence compatibility, and invalid-state rejection. Documenting a test is not evidence that it passed.

## Runtime acceptance

The following remain pending real Unreal execution: player state, level/XP, attributes, HP/resource, attack/damage, death/respawn, equipment, inventory, loot, persistence, 1-client PIE, 2-client PIE, Dedicated Server, Automation, and Output Log inspection.

The unresolved Phase 1 real Unreal foundation/runtime gate remains mandatory. Source/documentation completion is not runtime PASS.

## Definition of done

The canonical RPG loop, player state, progression, attributes/resources, combat, death/respawn, equipment, inventory, loot, authority boundaries, persistence compatibility, multiplayer contract, automation matrix, Phase 5 integration, roadmap update, and continuity update are defined without introducing duplicate architecture.

## Next phase

**PHASE 7 — PRIMEIRO INIMIGO E COMBATE**: enemy → AI/aggro → pursuit → attack → hit reaction → death → XP → drop → respawn.
