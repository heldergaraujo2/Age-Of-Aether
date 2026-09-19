# AGE OF AETHER — Repository Foundation

## Purpose
This document records the completed Phase 0 repository and architecture baseline.

## Source of truth
The Git repository is the canonical persisted state of AGE OF AETHER.

Required continuity sources:
1. PROJECT_MEMORY/START_NEW_CHAT.md
2. PROJECT_MEMORY/00_CONTINUITY.md
3. ROADMAP.md
4. Docs/MASTER_TECHNICAL_BLUEPRINT.md
5. Docs/ITEM_SYSTEM_REFERENCE.md

Documentation describes intent. A system is implemented only when its source/data exists and, where applicable, it compiles and passes validation.

## Unreal decision
Target Unreal Engine version: 5.8.1.
Phase 0 records the target version only. Local installation/toolchain verification and creation of the .uproject are Phase 1 responsibilities.

## Architecture contract
- C++ owns authoritative rules, critical systems and infrastructure.
- Data owns game content definitions.
- Blueprint owns presentation, composition and designer-facing visual workflows.
- The server is the final authority over mutable gameplay state.
- Persistence owns durable state through service/repository boundaries.
- Tests protect invariants and regressions.
- Audit/logging provides traceability for critical operations.

## Repository organization
Current Phase 0 intentionally contains planning, architecture and project-memory material only:
- Docs/ — technical architecture and design references.
- PROJECT_MEMORY/ — continuity and agent operating instructions.
- ROADMAP.md — execution sequence.

Unreal runtime directories (Config/, Source/, Content/, Plugins/) are not created in Phase 0 because doing so would falsely imply an Unreal project already exists. They belong to Phase 1.

## Git strategy
- main is the source-of-truth branch during the foundation stage.
- Commits describe the actual change.
- No force-push/rewrite of shared history.
- Significant changes update continuity.
- Feature branches may be introduced when parallel development becomes useful.

## Naming
- Repository: Age-Of-Aether
- Technical project: AgeOfAether
- C++ types: PascalCase.
- C++ functions: PascalCase for Unreal-style APIs.
- C++ variables: descriptive camelCase or established Unreal conventions.
- Unreal assets use prefixes such as BP_, WBP_, DA_, DT_, MI_, M_, T_, SK_, AN_ where applicable.
- Stable data identifiers must not depend on display text.
- Names must be stable, descriptive and searchable.

## IP / licensing
AGE OF AETHER uses original implementation and original content.
MU Online material is reference-only. Do not copy proprietary code, assets, maps, models, textures, animations, sounds, protocols, client files or binaries.
Third-party dependencies require license review before inclusion. Never commit secrets.

## Phase 0 boundary
Phase 0 establishes the rules for development; it does not implement gameplay or the Unreal runtime.
Phase 0 is complete when a new agent can understand project identity, authority, repository layout, Git/asset policy, naming/coding expectations, testing expectations, IP constraints and the next executable phase.

## Phase 1 handoff
Phase 1 begins by verifying the local Unreal 5.8.1 installation and toolchain, then creating/validating the real Unreal project and reproducible repository structure.
