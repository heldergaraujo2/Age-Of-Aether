# Phase 48 — Minimal Unreal Setup

1. Create one UAetherInteractionCatalog Data Asset.
2. Add one Talk interaction targeting an NPC ID.
3. Add one Quest with a simple objective count such as 3.
4. Add one Shop interaction gated by that quest if desired.
5. Add one timed Event definition.
6. Register the catalog through the world-content bootstrap when available, or a test Level Blueprint during local validation.
7. Use Blueprint only for widgets/buttons and presentation; keep state changes in the subsystem/server path.

No per-NPC Quest Blueprint and no per-event C++ class is required.

Visual minimum:
- Reuse the existing NPC skeletal mesh and Animation Blueprint.
- Dialogue UI can be one reusable widget.
- Event presentation can reuse one VFX/SFX profile.

Acceptance:
- insufficient level rejected;
- missing prerequisite quest rejected;
- quest starts Active;
- objective progress is bounded;
- completion requires required count;
- non-repeatable quest cannot restart;
- invalid event duration rejected;
- 2-client PIE and Dedicated Server checked locally.
