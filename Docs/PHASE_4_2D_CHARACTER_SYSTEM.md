# AGE OF AETHER — PHASE 4 — 2D CHARACTER SYSTEM

## Objective

Adapt the existing playable-character presentation root to support a premium 2D isometric representation without rebuilding gameplay, character identity, progression, combat, networking or input.

## Architectural result

The existing AAetherCharacter remains the gameplay authority.

Phase 4 adds a dedicated 2D presentation layer:

- UAether2DCharacterVisualProfile — data-driven 2D visual contract.
- UAether2DCharacterVisualComponent — runtime presentation component.
- EAether2DCharacterVisualState — stable visual state vocabulary.
- Paper2D is added as the presentation dependency.
- AAetherCharacter owns the new 2D component alongside the existing visual systems.

The original skeletal visual component remains intact. The new system is additive and can become the primary presentation only when a 2D profile explicitly enables it.

## Supported states

The canonical state vocabulary is:

- Idle
- Walk
- Run
- Attack
- Hit
- Death
- Cast
- Interaction

This state layer presents existing gameplay state; it does not replace gameplay authority.

## Movement presentation

When enabled, the 2D component derives Idle/Walk/Run from the existing character velocity.

- below the movement threshold → Idle;
- normal movement → Walk;
- high movement speed → Run.

The gameplay movement system remains unchanged.

## Combat presentation

Basic attack now forwards to the 2D visual component in addition to the existing animation path.

Attack is treated as a non-looping visual state and returns to Idle after the authored Flipbook finishes.

This does not mutate combat authority, damage, targeting, networking or server-side combat logic.

## Data-driven visual contract

A profile contains:

- stable VisualProfileID;
- state → Flipbook mapping;
- visual scale;
- world offset;
- explicit primary-presentation switch.

Idle is mandatory for a valid profile.

Null references and invalid scale are rejected.

## Quality requirements

The system is designed for:

- extremely fluid 2D animation;
- stable anchors;
- consistent scale;
- clean state transitions;
- responsive attack presentation;
- isometric readability;
- future shadow/light/VFX layering;
- deterministic fallback to the existing visual root when no valid 2D profile is assigned.

The project-wide maximum-quality directive remains mandatory.

## Class and evolution compatibility

Existing class/evolution presentation systems remain authoritative and are not recreated.

Future class/evolution-specific 2D profiles can be selected through the existing presentation/catalog layer without changing gameplay contracts.

## Equipment compatibility

The existing equipment visual component remains untouched.

2D equipment overlays can be introduced later as presentation extensions using the same stable identity and profile approach. Phase 4 does not duplicate inventory/equipment authority.

## Networking and multiplayer

The new component does not introduce replicated gameplay state.

The visual state is presentation-only. Gameplay replication remains owned by existing character/network systems.

Dedicated-server behavior is safe at the presentation boundary because the 2D component refuses runtime application on dedicated server.

## Unreal materialization boundary

This phase intentionally does not fabricate or commit Unreal binary assets.

The following are source-level contracts only until the Unreal validation window:

- real Texture2D assets;
- real Paper2D Sprite assets;
- real Paper2D Flipbooks;
- real materials;
- real visual profile Data Assets;
- final imported character art.

Those must be created through the legitimate Unreal Editor/toolchain and then validated in runtime.

## Source files introduced/changed

- Source/AgeOfAether.Build.cs
- Source/AgeOfAether/Public/Characters/Aether2DCharacterVisualProfile.h
- Source/AgeOfAether/Private/Characters/Aether2DCharacterVisualProfile.cpp
- Source/AgeOfAether/Public/Characters/Aether2DCharacterVisualComponent.h
- Source/AgeOfAether/Private/Characters/Aether2DCharacterVisualComponent.cpp
- Source/AgeOfAether/Public/Characters/AetherCharacter.h
- Source/AgeOfAether/Private/Characters/AetherCharacter.cpp

## Validation status

Repository/source validation:

- architecture inspected before adaptation;
- existing character gameplay root preserved;
- existing skeletal visual path preserved;
- 2D profile validation implemented;
- state vocabulary implemented;
- movement state mapping implemented;
- attack state routing implemented;
- non-looping attack recovery implemented;
- Paper2D dependency declared;
- no Unreal binary fabricated.

Unreal validation is intentionally deferred as requested.

## Completion boundary

**PHASE 4 — SOURCE/ARCHITECTURE: COMPLETE**

**PHASE 4 — REAL UNREAL MATERIALIZATION/RUNTIME: DEFERRED**

This means the repository now contains the 2D character presentation foundation. It does not falsely claim that final production character art or Unreal runtime assets already exist.

## Exit criterion

The phase is closed at the repository/source level because the reusable 2D character presentation contract and integration point now exist without replacing the existing gameplay architecture.

