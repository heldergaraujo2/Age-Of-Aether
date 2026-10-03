# PHASE 0 — OPEN WORLD ISOMETRIC RPG BASE AUDIT

## STATUS

**COMPLETE at repository/source audit level.**

This phase establishes the transformation boundary for AGE OF AETHER without replacing the existing MMORPG foundation.

Important truth:
- This is a repository/source audit.
- It does not claim Unreal Editor runtime acceptance.
- Unreal runtime remains the mandatory gate for Phase 1.

## OBJECTIVE

Transform the existing AGE OF AETHER into a high-quality open-world isometric 2.5D RPG with a stylized painterly diorama identity while preserving the mature server-authoritative architecture.

Target product:
- open world;
- isometric 2.5D presentation;
- hand-painted / painterly visual language;
- premium-quality environments, characters, materials, lighting and VFX;
- MMORPG progression and multiplayer foundation;
- data-driven content expansion;
- Unreal Engine 5.8 runtime.

## AUDIT RESULT

The repository already contains a broad MMORPG/gameplay foundation and a complete source-level visual/playable framework. The transformation therefore **must not be a rewrite**.

The correct strategy is:

**PRESERVE the gameplay architecture → ADAPT presentation/camera/world → CORRECT known repository/runtime gates → REPLACE only temporary/bootstrap presentation → CREATE the real art/content layer.**

## CLASSIFICATION MATRIX

| Area | Decision | Finding | Transformation |
|---|---|---|---|
| Unreal project | PRESERVE | UE 5.8 project exists and Enhanced Input is enabled | Keep project/module structure |
| Server authority | PRESERVE | Existing gameplay architecture is server-authoritative | Never move gameplay authority into presentation |
| Networking | PRESERVE | Request/validation/replication foundations already exist | Adapt movement/camera presentation only |
| Accounts/sessions | PRESERVE | Existing account/session foundation | No redesign for visual transformation |
| Persistence | PRESERVE | Persistence contracts and SaveGame-backed implementation exist | Reuse for open-world state |
| Progression | PRESERVE | Level/XP/class/evolution foundation exists | Present through new UI/art |
| Inventory/equipment | PRESERVE | Data-driven inventory/equipment exists | Adapt visual attachment and UI |
| Economy/crafting | PRESERVE | Economy, shops and recipes exist | Build world-facing presentation |
| Quests/events | PRESERVE | Quest/event contracts exist | Integrate with living-world presentation |
| Combat | PRESERVE | Server-authoritative combat exists | Adapt targeting/camera/animation/VFX |
| Skills/effects | PRESERVE | Skill/effect/status foundation exists | Create premium VFX/animation layer |
| Creatures/AI contracts | PRESERVE | Generic creature/catalog foundation exists | Create real monsters/NPCs/bosses |
| World registry | PRESERVE | World/map identity and streaming contracts exist | Build real World Partition maps |
| Streaming/scale | PRESERVE | Streaming/performance abstractions exist | Validate and tune with real profiling |
| UI architecture | PRESERVE | Presentation/controller/catalog foundation exists | Replace placeholder visuals with final UI |
| Audio architecture | PRESERVE | Data-driven audio subsystem exists | Create real audio content |
| Asset Manager | PRESERVE | Primary asset scanning/stable identity foundation exists | Extend to production art |
| Visual profiles | PRESERVE | Character/equipment/class visual bridges exist | Use as reusable presentation layer |
| Temporary BasicShapes ground | REPLACE | Development ground is explicitly a bootstrap placeholder | Replace with authored terrain |
| Runtime fallback input | ADAPT | Useful for foundation but not final UX | Move toward authored mappings/settings |
| Native debug HUD | REPLACE | Debug-only presentation | Replace/augment with final HUD |
| Spring-arm third-person camera | ADAPT | Existing camera proves character foundation | Transform into controlled isometric camera |
| Placeholder/no real art | REPLACE | Repository deliberately avoids fake binary assets | Import production assets through real pipeline |
| Empty/placeholder maps | CREATE | Real world maps are still local Unreal content | Create first production diorama |
| Production environment art | CREATE | No production art library is committed | Build/import coherent art set |
| Production characters | CREATE | Source contracts exist; real meshes are pending | Create/import hero, NPC and creature assets |
| Production animation | CREATE | Contracts exist; real animation assets pending | Build locomotion/combat animation set |
| Production VFX | CREATE | Contracts exist; real VFX pending | Build painterly combat/world VFX |
| Production lighting | CREATE | No final map lighting exists | Establish art-directed lighting recipe |
| Isometric camera UX | CREATE | Not yet the product-defining camera | Build final camera/controller behavior |
| World composition | CREATE | World contracts exist, content does not | Build village/forest/dungeon composition |
| Painterly material language | CREATE | No final material library | Establish master materials and texture rules |
| Visual QA/performance budget | CREATE | Performance contracts exist but visual target is not locked | Establish measurable visual budgets |

## ARCHITECTURAL DECISION

The existing architecture is suitable for the new product.

No second game architecture should be introduced.

### Keep

- C++ gameplay/domain rules;
- server authority;
- data-driven definitions;
- registries;
- stable IDs;
- persistence;
- networking;
- security;
- generic reusable presentation components;
- Asset Manager;
- Automation contracts.

### Change

- camera model;
- world presentation;
- visual asset quality;
- map composition;
- materials;
- lighting;
- animation presentation;
- VFX;
- UI art direction;
- environmental storytelling.

### Replace

Only explicitly temporary/bootstrap pieces:
- BasicShapes development floor;
- debug HUD;
- placeholder visual assets;
- development-only map composition.

## PREMIUM VISUAL QUALITY TARGET

The project now adopts the following non-negotiable visual target:

**Stylized Painterly Isometric / 2.5D Hand-Painted Diorama at premium game quality.**

The target is not low-poly placeholder art and not a generic orthographic camera placed above a conventional 3D level.

### Environment

- dense, authored composition;
- strong silhouettes;
- layered depth;
- high-quality hand-painted or painterly textures;
- detailed architecture;
- rich vegetation;
- rocks, roads, props and clutter;
- believable material separation;
- deliberate color/value hierarchy;
- environmental storytelling.

### Characters

- distinctive silhouettes;
- high-quality sculpt/model;
- coherent stylized proportions;
- detailed materials;
- clean deformation;
- polished locomotion and combat animation;
- equipment that visibly changes the character;
- readable at the gameplay camera distance.

### Lighting

- art-directed key/fill/rim relationships;
- high-quality shadows;
- ambient occlusion/contact grounding where appropriate;
- atmospheric depth;
- day/night or lighting-state support when justified;
- controlled post-processing;
- no dependence on excessive bloom or effects to hide weak assets.

### Materials

- coherent master-material family;
- painterly albedo/value treatment;
- controlled roughness/specular response;
- detail layers;
- weather/variation support where useful;
- consistent visual language across environment, characters and props.

### VFX

- readable silhouettes;
- impact feedback;
- spell identity;
- restrained but high-quality particles;
- ground effects;
- boss telegraphs;
- lighting interaction;
- scalability tiers.

### Camera

- deliberate isometric composition;
- stable readable framing;
- controlled zoom;
- occlusion handling;
- target readability;
- comfortable traversal;
- no camera behavior that destroys the diorama composition.

## FIRST VISUAL QUALITY BENCHMARK

The first production benchmark is one small but polished playable diorama:

**Village → road → forest → combat clearing → dungeon entrance.**

It must contain:
- playable character;
- one class/evolution presentation;
- one weapon;
- one armor set;
- NPC;
- interaction;
- quest;
- three creature presentations;
- combat;
- three skills;
- loot;
- dungeon entrance;
- boss presentation;
- essential HUD;
- lighting;
- ambient audio;
- VFX.

The benchmark is intentionally small in geographic scope but high in visual quality.

No mass production should begin before this benchmark establishes the final visual language.

## QUALITY RULE

A new asset is not accepted merely because it technically imports.

It must satisfy:
1. silhouette/readability at gameplay camera distance;
2. material consistency;
3. lighting compatibility;
4. animation/interaction compatibility where applicable;
5. performance budget;
6. stable asset identity;
7. fallback behavior when applicable;
8. runtime proof.

## RUNTIME TRUTH

The repository contains many source-level implementations, but the following remain runtime gates:
- Unreal 5.8 editor startup;
- UHT/UBT builds;
- real map creation;
- real asset import;
- PIE;
- 2-client PIE;
- Dedicated Server;
- Automation Framework;
- replication;
- runtime visual presentation;
- performance profiling.

Therefore **source completeness must never be reported as runtime completion**.

## KNOWN REPOSITORY VALIDATION STATE

The project continuity records:
- previous real C++ Development Editor build: PASS;
- latest recorded automation: 280 total / 266 PASS / 14 FAIL / 0 WARN;
- ClassBalance remains the principal recorded failure block;
- additional recorded failures include BalanceSimulation.Neutral, ClassCombat.PvPSwitch, QuestDialogueEvent.CrossReferences, Multiplayer heartbeat/rate-limit, Quests.Security and Security.Replay.

These are technical gates and are not caused by the isometric art direction. They must be handled before declaring the runtime foundation closed.

## PHASE 0 GATE

Phase 0 is complete when:
- existing systems are classified;
- no duplicate architecture is planned;
- temporary presentation is identified;
- production visual target is explicit;
- first benchmark is defined;
- runtime/source distinction is explicit;
- transformation risks are documented.

All repository-level criteria above are now satisfied.

## NEXT PHASE

**PHASE 1 — REAL UNREAL FOUNDATION**

Priority order:
1. synchronize local clone with main;
2. execute Unreal 5.8 runtime gate;
3. close known build/Automation blockers;
4. create the real development map;
5. prove PIE;
6. prove 2-client PIE;
7. prove Dedicated Server smoke;
8. only then begin the final isometric camera and production visual implementation.

The first real art asset should not be used as a substitute for a broken runtime foundation.
