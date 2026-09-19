# PHASE 20 — ASSET PIPELINE & VISUAL ASSET REGISTRY

## STATUS
**Repository implementation: COMPLETE at source level.**
**Static validation: PASS.**
**Unreal 5.8.1 UHT/UBT/Editor/Automation runtime: NOT VERIFIED in this environment.**

## IMPLEMENTADO
- Stable \`AssetID\` identity independent of Unreal asset paths.
- Asset types for skeletal/static meshes, animation, texture, material/material instance, Niagara, sound, widget, icon and map.
- Asset descriptor carrying:
  - Unreal asset path;
  - fallback asset;
  - skeleton;
  - animation set;
  - materials;
  - icon;
  - VFX;
  - SFX;
  - scale;
  - socket names;
  - content version.
- Source/import metadata supporting the FBX -> Unreal -> UAsset workflow.
- Structural validation for required IDs, asset type, positive content version, finite positive scale, required skeleton and declared fallback.
- Central visual asset registry with register, batch register, resolve, contains, remove, reset and deterministic enumeration.
- Cross-reference validation for fallback, skeleton, animation, material, VFX and SFX assets.
- Normalized IDs and duplicate detection.
- Structured validation diagnostics.
- Automation tests covering registration, normalization, duplicates, fallback, skeleton, numeric constraints, dependency graph and deterministic ordering.

## ARCHITECTURE CONTRACT

External authoring:
\`FBX/source -> Unreal import -> UAsset\`

Runtime identity:
\`Stable AssetID -> Asset Registry -> Unreal Asset Path -> Client Presentation\`

Server/client rule:
- Server transmits stable gameplay/content IDs.
- Client resolves IDs to local Unreal presentation assets.
- Server must not depend on client Unreal package paths.

Example:
\`Monster.Goblin.001 -> AssetID Monster.Goblin.Mesh.001 -> local Unreal SkeletalMesh\`

## VALIDATION

### Static/source checks
- braces/parentheses/brackets reviewed;
- generated-header names and locations reviewed;
- include paths reviewed;
- ID normalization reviewed;
- duplicate rejection reviewed;
- cross-reference validation reviewed;
- missing fallback/skeleton/dependency behavior reviewed;
- positive finite scale validation reviewed;
- deterministic ordering reviewed;
- Automation coverage reviewed.

### Unreal checks

The following could **not** be executed here:
- Unreal Header Tool (UHT);
- Unreal Build Tool (UBT);
- Development Editor compile;
- Automation Framework execution;
- Editor Asset Registry integration;
- actual FBX import;
- cooked build/package;
- PIE visual rendering.

Reason: Unreal Engine 5.8.1 is not executable in the current environment.

No compile/runtime success is claimed without that local validation.

## TEST MATRIX

| Check | Source/static | UE 5.8.1 runtime |
|---|---|---|
| Stable AssetID | PASS | NOT VERIFIED |
| Duplicate AssetID | PASS | NOT VERIFIED |
| ID normalization | PASS | NOT VERIFIED |
| Asset type validation | PASS | NOT VERIFIED |
| Positive finite scale | PASS | NOT VERIFIED |
| Required skeleton | PASS | NOT VERIFIED |
| Fallback validation | PASS | NOT VERIFIED |
| Skeleton reference | PASS | NOT VERIFIED |
| Animation references | PASS | NOT VERIFIED |
| Material references | PASS | NOT VERIFIED |
| VFX references | PASS | NOT VERIFIED |
| SFX references | PASS | NOT VERIFIED |
| Deterministic ordering | PASS | NOT VERIFIED |
| UHT | NOT RUN | NOT VERIFIED |
| UBT | NOT RUN | NOT VERIFIED |
| Automation Framework | NOT RUN | NOT VERIFIED |
| Real FBX import | NOT RUN | NOT VERIFIED |
| Runtime rendering | NOT RUN | NOT VERIFIED |

## FALHAS

No repository-level implementation defect identified during static review.

## PENDÊNCIAS

These are environment/runtime validation gates, not silently marked as complete:
1. Run UHT/UBT on Unreal 5.8.1.
2. Execute \`AgeOfAether.AssetRegistry.*\` Automation tests.
3. Create/register actual Unreal assets and verify paths through the real Editor Asset Registry.
4. Import representative FBX skeletal/static meshes.
5. Validate skeleton/animation compatibility in Editor.
6. Validate fallback resolution at runtime.
7. Validate cooked/package behavior.

## PRÓXIMO PASSO

**Phase 21 — Complete Item, Equipment & Enhancement Data.**

The stable AssetID contract created here is the presentation bridge for the next content phases.
