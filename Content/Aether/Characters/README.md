# Playable Characters

## Minimal visual workflow
1. Import one real skeletal FBX.
2. Create one AetherPlayableCharacterVisualProfile.
3. Assign Skeletal Mesh.
4. Assign Animation Blueprint when available.
5. Assign optional materials.
6. On the character, assign the visual profile.
7. Press Play.

## Movement/camera workflow
Phase 40 adds an optional AetherMovementCameraProfile Data Asset. Use one profile for the first playable slice. Movement and camera tuning does not require new C++.

Runtime fallback remains available when no profile is assigned:
- walk 420;
- sprint 630;
- jump uses Unreal CharacterMovement default;
- camera distance 250–650;
- mouse wheel zoom;
- pitch clamp -75 to +35 degrees.

## Input
The first playable slice creates Enhanced Input actions at runtime, avoiding unnecessary manual Input Action/Mapping Context assets. Current controls:
- W/A/S/D: movement
- Mouse: camera
- Space: jump
- Left Shift: sprint
- Mouse wheel: zoom

These can later be promoted to authored Input Action/Mapping Context assets for rebinding/localization without changing gameplay authority.

## Safety
Movement remains server-authoritative through Unreal CharacterMovement/replication. Sprint transitions use a server RPC; the server selects the configured speed and never trusts a client-provided speed value. Input does not write authoritative position, class, inventory or progression.
