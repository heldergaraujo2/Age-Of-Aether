# Phase 59 — Class Balance Blocker: Static Triage

**Date:** 2026-10-17
**Status:** Two stale automation expectations corrected at source level; Unreal runtime verification pending. This does **not** close the primary ClassBalance blocker.

## Findings and source changes

### `AgeOfAether.BalanceSimulation.Neutral`

The test case uses base damage `100`, target defense `100`, and resistance `20%`. The simulation formula applies the default defense scale of `100`:

```text
100 × (100 / (100 + 100)) × (1 - 20 / 100) = 40
```

`FAetherCombatConfig::DefenseMitigationScale` also defaults to `100`, and the deterministic combat-test config explicitly sets it to `100`. The previous expected value `26.6666667` was inconsistent with both the inputs and the current combat mitigation formula. The test now asserts `40` with a descriptive explanation; no combat formula or safety contract was changed.

### `AgeOfAether.ClassCombat.PvPSwitch`

The fixture defines Archer base damage as `2.0` in both modes, with evolution damage `1.5` in PvE and `1.25` in PvP. The balance registry composes class and evolution modifiers multiplicatively, as required by Phase 33 and already checked by the neighboring authoritative-resolution test. Therefore the correct resolved values are:

- PvE: `2.0 × 1.5 = 3.0`
- PvP: `2.0 × 1.25 = 2.5`

The test now checks those composed results and still asserts that PvE and PvP remain distinct. Production code and test coverage were not weakened.

## Validation boundary

- A targeted Python source/arithmetic check passed for the default scale, both formula implementations, and the corrected expectations; `git diff --check` passed.
- These checks are static only; they do not execute or compile the C++ automation tests.
- Unreal Editor, UnrealEditor-Cmd, and UnrealBuildTool are not available in this workspace. No UHT/UBT build or Unreal Automation test was run for these edits.
- The historical Unreal result remains **280 total / 266 PASS / 14 FAIL / 0 WARN**. It is not a result from this session and must not be treated as refreshed.
- The primary ClassBalance failures remain unresolved until their actual failing test names/logs are identified and the relevant Unreal tests are rerun.

## Required next gate

On the Unreal 5.8.2 environment, build the editor target, then run the `AgeOfAether.ClassBalance`, `AgeOfAether.BalanceSimulation`, and `AgeOfAether.ClassCombat` Automation groups. Preserve the complete log and update the Roadmap only from that runtime evidence. These source-level expectation fixes are candidates to remove two historical failures; they are not yet verified as passing.
