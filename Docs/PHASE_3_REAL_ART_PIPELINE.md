# PHASE 3 — REAL ART PIPELINE

## STATUS

**SOURCE/DOCUMENTATION: COMPLETE.**  
**UNREAL RUNTIME / REAL ASSET ACCEPTANCE: PENDING LOCAL EXECUTION.**

This phase formalizes the production pipeline for real visual assets without fabricating Unreal binary assets. It complements the existing repository implementation in `Docs/PHASE_38_REAL_ASSET_PIPELINE.md` and turns that technical contract into the production workflow required by the Open World + Isometric + Painterly direction.

The phase is closed at source/documentation level only. Real FBX import, material inspection, skeleton/Physics Asset validation, animation import, sockets, LOD/Nanite decisions, Asset Manager loading and runtime presentation still require the user's installed Unreal Engine environment.

---

## 1. OBJECTIVE

Establish one reproducible pipeline:

**Concept → Source Asset → Cleanup/Validation → Import Profile → Unreal Asset → Materials/Skeleton/Collision → Asset Registry → Stable AssetID → Presentation Consumer → Runtime Validation**

The pipeline must support:

- characters;
- creatures;
- NPCs;
- weapons;
- armor/equipment;
- architecture;
- props;
- vegetation;
- terrain/environment assets;
- materials/textures;
- animations;
- VFX;
- UI icons;
- audio references where applicable.

The pipeline must scale toward one continuous world with approximately 40 cities and large jurisdictions without creating a separate workflow for each region.

---

## 2. NON-NEGOTIABLE RULES

1. Never fabricate `.uasset`, `.umap`, FBX or other binary content.
2. Every real asset has a purpose and a consumer before production expands.
3. Every runtime-facing visual asset resolves through stable identity/registry contracts.
4. Server-authoritative gameplay never depends on client-created visual state.
5. Real source provenance and license/origin must be recorded.
6. Missing dependencies and invalid references fail validation instead of silently propagating.
7. Fallbacks are explicit where a consumer requires them.
8. Import settings are deterministic and documented.
9. LOD/Nanite/material decisions are made according to asset class and measured need, not by blanket rule.
10. The first accepted asset is used to prove the pipeline before mass import.
11. Runtime acceptance is never inferred from repository/source correctness.
12. The pipeline must remain compatible with the existing Asset Manager, registries and presentation components.

---

## 3. CANONICAL ASSET LIFECYCLE

### Stage A — Concept and specification

Before acquiring or creating an asset, define:

- asset purpose;
- gameplay/presentation consumer;
- asset type;
- required silhouette/readability;
- target platform/quality;
- expected scale;
- attachment points/sockets when applicable;
- animation requirements;
- collision requirements;
- material requirements;
- performance class;
- fallback requirement;
- AssetID.

### Stage B — Source acquisition

Record:

- source location;
- creator/vendor;
- license;
- permitted modifications;
- version;
- revision/date;
- source format;
- dependencies;
- attribution requirements.

No asset enters the production registry without provenance.

### Stage C — Source cleanup

Validate as applicable:

- transform/origin;
- scale;
- orientation;
- naming;
- topology;
- normals/tangents;
- UVs;
- material slots;
- texture references;
- skeleton hierarchy;
- bone naming;
- animation ranges;
- mesh intersections;
- hidden/unwanted geometry;
- excessive geometry;
- texture dimensions;
- collision intent.

### Stage D — Import

Use the existing import profile contract.

Default skeletal/static workflow:

**Source → Import Profile → Unreal Mesh/Material/Skeleton/Animation → inspect → validate**

Do not enable broad automatic content generation merely to reduce manual work. Predictability is preferred during the first accepted slice.

### Stage E — Unreal asset assembly

Depending on asset class:

- Static Mesh;
- Skeletal Mesh;
- Skeleton;
- Physics Asset;
- Animation Sequence;
- Animation Montage;
- Animation Blueprint;
- Material/Material Instance;
- Texture;
- Niagara/VFX asset;
- Sound asset;
- Widget/icon asset;
- Blueprint composition asset.

### Stage F — Registry and identity

Register:

- stable AssetID;
- Unreal asset path;
- type;
- version;
- source metadata;
- dependencies;
- fallback;
- skeleton AssetID where relevant;
- socket requirements;
- consumer/presentation role;
- validation status.

The runtime contract uses stable IDs rather than exposing editor-specific paths as gameplay authority.

### Stage G — Integration

Connect the asset to the existing consumer:

- character visual component;
- equipment visual component;
- creature presentation;
- world/prop composition;
- UI catalog;
- VFX profile;
- audio catalog;
- content package;
- map/world definition.

Do not create a bespoke manager for every asset.

### Stage H — Runtime acceptance

The real Unreal environment must prove:

- import succeeds;
- asset opens correctly;
- scale/orientation are correct;
- materials/textures resolve;
- skeleton is valid;
- Physics Asset is valid where required;
- sockets resolve;
- animations play where required;
- LOD/Nanite behavior is appropriate;
- Asset Manager/registry resolves the asset;
- fallback resolves when configured;
- runtime consumer displays it;
- multiplayer presentation remains correct;
- Dedicated Server does not unnecessarily load presentation-only content;
- Output Log has no new critical errors.

---

## 4. ASSET CLASSES

### Characters / Creatures / NPCs

Required contract:

**Skeletal Mesh + Skeleton + Physics Asset + Materials + Animation Set + Presentation Profile + Stable AssetID**

Validate silhouette under the isometric camera, readable combat pose, equipment attachment, animation compatibility and expected distance readability.

### Weapons / Equipment

Required contract:

**Mesh + Materials + Socket/Attachment Definition + Visual Definition + Stable AssetID**

The server validates equipment state; the client presents the corresponding asset.

### Architecture / Props

Required contract:

**Static Mesh + Materials + Collision + LOD/Nanite Policy + Stable AssetID**

Assets must support modular assembly and continuous-world reuse.

### Environment / Vegetation

Required contract:

**Static Mesh/Foliage Asset + Materials + Collision/Interaction Policy + Performance Class + Stable AssetID**

Environmental density must support World Partition/streaming and future profiling.

### Materials / Textures

Validate:

- intended shader/material class;
- texture references;
- UV assumptions;
- material instance parameters;
- roughness/specular/normal usage;
- painterly surface language;
- region-specific variation;
- memory/performance implications.

### Animation

Validate:

- skeleton compatibility;
- root motion policy;
- frame rate;
- looping;
- start/stop transitions;
- locomotion/combat use;
- montage/notify requirements;
- multiplayer presentation behavior.

Animation Notifies may synchronize presentation but never become gameplay authority.

### VFX

Validate:

- spawn/attachment contract;
- lifetime;
- scalability;
- camera readability;
- gameplay-result correspondence;
- network relevance;
- performance class.

VFX represents authoritative results; it does not decide them.

---

## 5. PAINTERLY PREMIUM REQUIREMENTS

The pipeline must preserve the official visual direction:

**Stylized Painterly Isometric / 2.5D Hand-Painted Diorama / 2.5D Isometric Cutaway**

Assets should be reviewed for:

- strong silhouette;
- readable shape language;
- painterly material response;
- controlled roughness variation;
- intentional color/value grouping;
- visual hierarchy;
- contact with the environment;
- readable shadows;
- isometric distance readability;
- regional identity;
- consistency between characters, creatures, architecture and props.

Do not accept technically valid assets that visibly break the established art direction merely because they import successfully.

---

## 6. LOD / NANITE / PERFORMANCE POLICY

No universal setting is mandated.

Decision sequence:

1. identify asset class;
2. identify expected camera distance;
3. identify geometry density;
4. identify repetition/density;
5. identify animation requirements;
6. identify platform/runtime constraints;
7. select LOD/Nanite strategy;
8. validate visual quality;
9. profile real cost in Unreal;
10. document the accepted policy.

Performance decisions must be evidence-based.

For repeated world assets, evaluate:

- instance count;
- draw/instance cost;
- memory;
- streaming;
- shadow cost;
- material complexity;
- foliage density;
- skeletal animation cost.

---

## 7. COLLISION POLICY

Collision is gameplay-relevant and must not be treated as decoration.

Each asset declares the intended collision behavior:

- blocking;
- overlap;
- trace-only;
- interaction;
- no collision;
- custom gameplay collision.

Characters, props, architecture and world traversal must use collision appropriate to the actual gameplay need.

Physics Assets for skeletal assets must be checked in real Unreal before runtime acceptance.

---

## 8. SOCKET POLICY

Sockets are stable integration points for:

- weapons;
- shields;
- back items;
- wings/capes;
- VFX attachment;
- interaction presentation;
- creature effects.

Socket names must be intentional and documented in the asset contract. Consumers must not depend on arbitrary bone indices.

---

## 9. ASSETID / DEPENDENCY POLICY

Every production asset should resolve to:

**AssetID → registry record → Unreal asset → dependencies → fallback when required**

Validation must reject:

- duplicate IDs;
- malformed IDs;
- missing dependencies;
- dependency cycles;
- missing required fallbacks;
- invalid source metadata.

This reuses the existing `FAetherAssetPipelineRecord`, `FAetherAssetPipelineRegistry` and related validation already implemented in Phase 38.

---

## 10. LOW-BUREAUCRACY AUTHORING RULE

Do not create:

- one C++ manager per asset;
- one Blueprint class per trivial variation;
- one registry per map;
- hard-coded asset paths throughout gameplay;
- duplicated import workflows.

Prefer:

**one pipeline + one registry + data-driven profiles + reusable presentation consumers.**

Adding a normal new asset should normally be:

**source + metadata + registry record + real Unreal asset + consumer reference**

rather than a C++ change.

---

## 11. FIRST REAL ACCEPTANCE ASSET

The first runtime proof must remain deliberately small.

Recommended order:

1. one simple skeletal character FBX;
2. one skeleton;
3. one Physics Asset;
4. one material;
5. one basic animation;
6. one stable AssetID;
7. one character visual profile;
8. one playable character consumer.

Acceptance then expands to:

9. one weapon;
10. one environment prop;
11. one creature;
12. one VFX;
13. one UI/icon asset.

Only after this chain is stable should mass asset production begin.

---

## 12. FIRST PRODUCTION SLICE

The art pipeline must feed the first real visual benchmark:

**city/village → road → forest → combat area → dungeon entrance**

The benchmark should use real assets through the same production pipeline that will later serve the complete world.

It must not become a disposable prototype disconnected from the continuous-world architecture.

---

## 13. VALIDATION MATRIX

| Area | Source/Repository | Unreal Runtime |
|---|---|---|
| AssetID/registry | Required | Resolve |
| Source metadata | Required | Inspect where applicable |
| Import profile | Required | Real import |
| Static/Skeletal Mesh | Contract | Open/inspect |
| Materials/textures | References/contracts | Visual validation |
| Skeleton | Contract | Real skeleton validation |
| Physics Asset | Contract | Real asset validation |
| Animation | Contract | Real playback |
| Sockets | Contract | Real attachment |
| Collision | Contract | Real traversal/interaction |
| LOD/Nanite | Policy | Visual + profiling |
| Asset Manager | Contract | Real load |
| Fallback | Validation | Real resolution |
| Character/creature presentation | Integration | PIE |
| Multiplayer | Architecture | 2-client PIE |
| Dedicated Server | Guard/contract | Real server test |
| Output Log | Static only | Real log inspection |

---

## 14. TESTING STATUS

Existing Phase 38 repository tests cover:

- registration;
- deterministic ID normalization;
- duplicate rejection;
- missing dependency detection;
- missing fallback detection;
- dependency cycle detection;
- import-profile validation.

Phase 3 does not duplicate those tests. It defines the production acceptance layer above them.

No new runtime PASS is claimed here because the real Unreal environment has not been audited.

---

## 15. RISKS

- Source assets may have incompatible licenses.
- FBX imports can expose scale, normals, skeleton, material or animation problems.
- Assets may be technically valid but visually inconsistent with the painterly target.
- High-density assets can exceed memory/streaming/performance budgets.
- Wrong collision can break navigation or gameplay.
- Socket inconsistencies can break equipment/VFX.
- Asset dependency growth can create hidden coupling.
- Unreal binary assets must be produced and verified in the real local Unreal environment, not generated through repository text APIs.

---

## 16. DEFINITION OF DONE — PHASE 3 SOURCE/DOCUMENTATION

The phase is considered complete at source/documentation level when:

- the real asset lifecycle is explicit;
- existing Phase 38 contracts are reused rather than duplicated;
- asset classes have acceptance requirements;
- provenance/license is mandatory;
- stable AssetID/dependency/fallback rules are explicit;
- painterly/isometric quality requirements are explicit;
- LOD/Nanite/collision/socket policy is explicit;
- first-asset acceptance order is explicit;
- runtime validation is separated from source validation;
- the roadmap is updated;
- `PROJECT_MEMORY/00_CONTINUITY.md` is updated;
- no fake Unreal binaries are introduced.

All source/documentation requirements above are satisfied by this phase.

---

## 17. RUNTIME GATE REMAINS OPEN

The following still require execution in the user's actual Unreal installation:

- UHT/UBT;
- Editor startup;
- real map;
- real FBX import;
- real material/skeleton/Physics Asset;
- real animation;
- sockets;
- Asset Manager load;
- PIE;
- 2-client PIE;
- Dedicated Server;
- Output Log;
- performance profiling.

Therefore:

**PHASE 3 = SOURCE/DOCUMENTATION COMPLETE / UNREAL RUNTIME PENDING.**

The unresolved **PHASE 1 — REAL UNREAL FOUNDATION / AUDIT OF THE REAL UNREAL ENVIRONMENT** remains an explicit project gate.

---

## 18. NEXT PHASE

**PHASE 4 — CAMERA AND ISOMETRIC EXPLORATION**

Next documentary/runtime preparation should define the integration of the already-existing camera/movement foundation with:

- isometric camera behavior;
- zoom;
- pan/rotation policy;
- camera collision;
- movement relative to camera;
- selection;
- targeting;
- interaction;
- multiplayer consistency.

No runtime acceptance should be declared until the real Unreal audit is executed.
