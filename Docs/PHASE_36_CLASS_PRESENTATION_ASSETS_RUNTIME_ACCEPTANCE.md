# PHASE 36 — Class Presentation, Assets & Runtime Acceptance

## STATUS
Repository implementation COMPLETE. Unreal 5.8.1 runtime gate remains NOT VERIFIED.

## IMPLEMENTADO
- Data-driven presentation contract for ClassID/EvolutionID.
- 25 canonical class/evolution presentation definitions generated from Phase 32.
- Stable bindings for skeletal mesh, animation set, icon, selection widget, transition VFX/SFX and equipment tags.
- Client-local class presentation subsystem with explicit load states.
- Deterministic presentation registry and safe same-class placeholder fallback.
- No presentation API is authoritative for class, evolution or balance.
- Real Unreal assets are not fabricated; AssetIDs remain the integration contract.

## TESTADO
- Registration, normalization, duplicate rejection and structural validation.
- Missing-presentation fallback behavior.
- Subsystem resolution, reset and load-state transitions.
- 25-entry catalog count, identity, registry validation and isolation.
- Phase 35 simulation fixture now registers base plus evolution rows used by its cases.
- Repository delimiter/source invariant checks and required-deliverable checks.

## COMPILADO
Repository CI/static validation is the available automated gate. Unreal Engine 5.8.1 UHT/UBT/Automation/PIE/Dedicated Server cannot be executed in this environment, so runtime compilation is NOT VERIFIED.

## VALIDADO
- Five base classes remain canonical.
- 25 evolutions map one-to-one to 25 presentation entries.
- Class presentation is client-facing and cannot change server authority.
- Missing assets have an explicit safe fallback model.
- No fake .uasset/.umap content was committed.

## FALHAS
No known repository-level defect after source/static audit.

## PENDÊNCIAS
1. Import the real FBX/skeletal meshes in Unreal 5.8.1.
2. Create/retarget animation blueprints and montages.
3. Create real materials, icons, VFX and SFX.
4. Validate async loading with Unreal Asset Manager/StreamableManager.
5. Run UHT/UBT and Unreal Automation.
6. Run multiplayer PIE, reconnect/persistence and dedicated-server acceptance.
7. Measure real performance budgets on target hardware.

## RISCOS
- Placeholder AssetIDs are contracts, not runtime assets.
- Skeleton/socket/retarget compatibility requires real Unreal content.
- Performance cannot be certified without real maps, actors and hardware.

## ACCEPTANCE MATRIX
Repository-level checks:
- 5 classes.
- 25 evolutions.
- 25 presentation definitions.
- Stable normalized IDs.
- Duplicate/invalid presentation rejection.
- Safe fallback.
- Explicit loading states.
- Client-only presentation boundary.

Unreal runtime gate:
- Class selection.
- Evolution selection.
- Mesh/animation/icon loading.
- Equipment attachments.
- Transition VFX/SFX.
- Skill presentation.
- Multiplayer PIE replication.
- Dedicated-server authority isolation.
- Persistence/reconnect.
- Missing-asset fallback.
- Performance budget validation.


Repository CI validation branch created for final Phase 36 verification.
