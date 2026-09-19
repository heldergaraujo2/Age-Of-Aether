# PHASE 23 — SKILLS, EFFECTS, BUFFS, DEBUFFS & STATUS

## Status
**Repository implementation: COMPLETE.**

## Scope
This phase establishes the data contract and registry for combat abilities and their persistent/transient effects. It deliberately separates definitions from future runtime execution.

## Skill model
A SkillDefinition supports:
- stable ID and display data;
- required level;
- target mode: self, ally, enemy, area, party or ground;
- delivery mode: instant, projectile, beam, area, melee or ground;
- cast time;
- cooldown;
- range and radius;
- mana/health/custom-resource costs;
- ordered effect IDs;
- required and blocked target tags;
- animation and visual asset IDs;
- critical-hit eligibility.

## Effect model
An EffectDefinition supports:
- damage, heal, mana restore;
- stun, root, silence, slow and haste;
- shield;
- damage-over-time and heal-over-time;
- stat modification;
- crowd-control immunity;
- taunt and dispel;
- custom effects for future extensions;
- duration and tick interval;
- primary/secondary magnitude;
- maximum stacks;
- stack policy: replace, refresh, add stacks or independent;
- dispel policy;
- tags and conflicting effects;
- visual asset ID.

## Status model
A StatusDefinition groups effects into a stable status identity and can add/remove gameplay tags and client visibility metadata.

## Validation
The registry validates:
- IDs/names;
- positive required level;
- finite non-negative timings/ranges;
- valid skill resource costs;
- area skills requiring a positive radius;
- ticking effects requiring a positive duration;
- positive stack limits;
- statuses requiring at least one effect;
- referenced effects existing;
- conflicting effects existing;
- skill content references against Phase 19;
- animation/visual asset references against Phase 20;
- global ID uniqueness across skills/effects/statuses;
- deterministic ID enumeration.

## Security/authority boundary
This phase does not trust the client to execute effects or decide damage/healing. The client may request a skill action later; server runtime must validate ownership, cooldown, resource cost, target legality and effect application. Runtime execution belongs to a later combat/runtime phase.

## Tests
Automation tests cover basic registration, invalid definitions, missing effect references, cross-type duplicate IDs and deterministic ordering.

## Validation truth
Repository/static validation is performed. Unreal 5.8.1 UHT, UBT, Editor, Automation Framework and PIE remain **NOT VERIFIED** because the Unreal toolchain is unavailable in this environment. No compilation success is claimed.

## Non-goals
Live skill casting, damage calculation, effect ticking, dispel execution, replication, animation playback, VFX execution and client UI are later runtime/content work.
