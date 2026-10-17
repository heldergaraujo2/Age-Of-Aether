# Phase 57 — Sprite-sheet animation slice

**Status:** implemented in the local browser preview and source-art pipeline; Unreal runtime acceptance pending.
**Scope:** animate the playable Mage, frontier NPC Elara and Wild Hound in the existing first-region Canvas preview.

## Implemented

- Generated separate transparent key-pose sources for Mage Walk/Cast, Elara Idle/Talk, and Wild Hound Run/Attack.
- Prepared the poses as RGBA PNGs and retained the original generated images under `ArtSource/Generated/Animations/`.
- Added deterministic row-based atlas packing in `Tools/Art/build_sprite_sheets.py` and per-row metadata for cell size, FPS, loop mode and sources.
- Built three sheets: Mage 4×3 (Idle/Walk/Cast), Elara 4×2 (Idle/Talk), Wild Hound 4×3 (Idle/Run/Attack).
- Wired `Preview/game.js` to select animation rows based on movement, spell/attack actions, Elara interaction and hound AI state. Existing standalone sprites and vector NPC drawing remain load-failure fallbacks.
- Added generated-frame, sheet and row records to `Content/Aether/Art/2D_ASSET_MANIFEST.json` and source prompts/provenance to `Content/Aether/Art/GENERATED_ART_PROVENANCE.md`.
- Expanded CI to rebuild the sheets, validate atlas dimensions/alpha/metadata, and run the preparation/packing unit tests.

## Validation performed

- `node --check Preview/game.js` — passed.
- Automated DOM/canvas-stub smoke flow — passed: verifies sprite-sheet rows for Mage Idle/Walk/Cast, Elara Talk, Wild Hound Run/Attack and completes the mission/loot/turn-in path.
- `Tools/Art/build_sprite_sheets.py` — produced all three atlases and manifests.
- `Tools/Art/validate_manifest.py` — validates the generated source/import pairs and animation metadata.
- `python -m unittest Tools.Art.test_prepare_generated_sprite Tools.Art.test_build_sprite_sheets` — passed.
- Workspace HTTP preview serves all sprite sheets locally; no CDN is used.

## Acceptance boundary

This is a first key-pose sprite-animation pass: some idle/hold cells are repeated with tiny anchor/scale changes, and action rows use generated poses with held frames. It does not yet include all eight directions, complete Hit/Death states, or frame-by-frame production animation. The browser plays the sheets, but the PNGs are **not** Unreal runtime assets. Import as Paper2D sprites, create Flipbooks, map rows to the real presentation profiles, and validate scale/sorting/transitions in UE 5.8 PIE before recording any Unreal PASS.
