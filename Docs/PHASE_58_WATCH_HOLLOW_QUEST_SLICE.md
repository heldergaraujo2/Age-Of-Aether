# Phase 58 — Oca da Vigília quest slice

**Status:** implemented in the local Canvas browser preview; Unreal runtime acceptance remains pending.
**Preview:** WEB SLICE 0.3.
**Scope:** extend the first-region browser slice with a second quest and an animated ranged enemy, while retaining the original Elara / Wild Hound mission.

## Implemented

- Added a generated Aether Wisp with transparent Idle, Glide and Pulse poses, prepared RGBA source frames, a deterministic 4×3 atlas, manifest entries and provenance prompts.
- Added the follow-up mission **O sussurro da Vigília**: inspect the Oca, defeat the Wisp, collect its Núcleo de Vigília, then activate the seal at the gate.
- Added a distinct Wisp encounter with hover/glide behavior, spacing AI, an animated pulse attack and traveling aether projectiles. Projectiles can miss if the player moves away from their marked impact point.
- Added Wisp targeting and health presentation, a second minimap pin, an active objective marker, an unstable-seal zone label, and an updated milestone/quest journal.
- Added key-item inventory handling: the Wisp drops one Núcleo de Vigília; activating the seal consumes it. Existing first-quest shard, rewards, combat, and turn-in flow remain intact.
- Kept the Canvas preview and all source art local. No account, backend, CDN, Unreal binary or fabricated Unreal content was introduced.

## Validation performed

- `node --check Preview/game.js` — passed.
- `git diff --check` — passed.
- `python Tools/Art/build_sprite_sheets.py` — built Mage, Elara, Wild Hound and Aether Wisp atlases.
- `python Tools/Art/validate_manifest.py` — passed: 18 unique art IDs and 16 generated RGBA source assets.
- `python -m unittest Tools.Art.test_prepare_generated_sprite Tools.Art.test_build_sprite_sheets -v` — passed: 6 tests.
- DOM/Canvas-stub smoke test — passed: original quest and Wisp quest; Mage Idle/Walk/Cast, Elara Talk, Hound Run/Attack and Wisp Idle/Glide/Pulse row selection; Wisp projectile dodge; core pickup and seal activation.
- HTTP smoke check serves the page, JavaScript, CSS and all four local sprite sheets from the Workspace preview.

The automated smoke uses a simulated Canvas and DOM. It is not visual-browser acceptance. UE 5.8 import, Paper2D Sprite/Flipbook creation, profile binding, PIE, multiplayer and real graphical inspection remain **NOT VERIFIED**. The required next acceptance gate is still local Unreal execution; this browser slice does not replace it.

## Source and reproducibility

- Browser atlas and metadata: `Preview/assets/spritesheets/aether-wisp.png` and `Preview/assets/spritesheets/manifest.json`.
- Prepared frames: `Content/Aether/Art/ImportReady/Animations/AetherWisp_*.png`.
- Generated source poses: `ArtSource/Generated/Animations/AetherWisp_*.png`.
- Art manifest and prompts: `Content/Aether/Art/2D_ASSET_MANIFEST.json` and `Content/Aether/Art/GENERATED_ART_PROVENANCE.md`.
- Rebuild: `python Tools/Art/build_sprite_sheets.py`.
