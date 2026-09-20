# Phase 52 — Minimal Unreal Setup

## First vertical slice

Create one UAetherAudioCatalog Data Asset.

Add:
- world.ambience — one real looping ambience sound;
- ui.confirm — one real one-shot sound;
- optionally combat.hit — one real one-shot sound.

The subsystem is UAetherAudioSubsystem (GameInstance Subsystem), so no Audio Manager Blueprint is required.

## Blueprint usage

From any client-side presentation/UI Blueprint:
- Get Game Instance Subsystem and select Aether Audio Subsystem.
- call PlayLoop with world.ambience;
- call StopLoop with world.ambience;
- call PlayOneShot with ui.confirm.

Do not use these calls as authoritative gameplay state.

## Simplicity rule

No per-sound C++ classes.
No per-map audio manager.
No mandatory MetaSound graph for the first slice.

Start with normal Unreal sound assets. MetaSounds can be adopted later where they add real value.

## Multiplayer

Audio is client presentation. The server may authorize an event, but clients render the approved presentation. Dedicated Server does not load/play presentation audio through this subsystem.
