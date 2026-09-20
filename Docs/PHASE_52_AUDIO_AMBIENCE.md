# Phase 52 — Audio & Ambience

## Status
Complete at repository/source level. Unreal 5.8.1 runtime acceptance remains a local gate.

Implemented:
- Stable AudioID definitions and normalized registry.
- Audio buses: Music, Ambience, UI, Combat, Creature and Voice.
- One-shot and loop playback contracts.
- Bounded volume, pitch and spatial-distance settings.
- Data Asset catalog for audio authoring.
- GameInstance audio subsystem with reusable Blueprint-callable playback.
- Dedicated Server guard: presentation audio is never loaded/played there.
- Automation coverage.
- No fake audio assets were committed.

## Low-bureaucracy workflow
1. Import one real audio asset.
2. Create one UAetherAudioCatalog Data Asset.
3. Add ambience/music/UI/combat rows.
4. Call the generic subsystem from Blueprint/UI/gameplay presentation.
5. Add more sounds as data rows; no C++ per sound.

## Runtime acceptance
- Compile Editor/Game/Dedicated Server.
- Create one catalog with a real ambience loop and one UI one-shot.
- Play/stop the loop.
- Play the one-shot.
- Verify no audio loading on Dedicated Server.
- Test PIE and 2-client PIE.
- Verify no leaked loop components after stop/map teardown.
