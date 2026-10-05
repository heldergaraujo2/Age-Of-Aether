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
- isometric overview camera starts around 2,100 units and zooms to 250 units for close inspection (3,600-unit overview maximum); orbit mode reaches 120 units;
- mouse wheel zoom;
- overview pitch clamp -75 to +35 degrees; 3D orbit pitch now spans -82 to +10 degrees.

## Input
The first playable slice creates Enhanced Input actions at runtime, avoiding unnecessary manual Input Action/Mapping Context assets. Current controls:
- Left mouse click: move to the clicked ground position
- Right mouse click: basic attack
- F7: enable/disable 3D orbit-camera controls; the last view is retained
- Hold middle mouse button and drag horizontally/vertically: orbit and tilt the camera
- Page Up / Page Down: fine camera tilt adjustment while orbit mode is active
- F6: reset camera pitch, yaw and zoom to the original view
- Mouse wheel: zoom, including close inspection in 3D orbit mode
- Space: jump
- Left Shift: sprint

These can later be promoted to authored Input Action/Mapping Context assets for rebinding/localization without changing gameplay authority.

## Textured grass ground
`ArtSource/Environment/T_GrassGround_Source.png` is the tileable stylized grass source. In Unreal Editor, run `Scripts/import_fab_library_assets.py`; it imports the texture and builds `/Game/Aether/Environment/Ground/Materials/M_GrassGround`. The development-world actor applies that material to its ground cube automatically. The same script reimports the Fab foliage textures and assigns two-sided masked materials to the tree cards. If the grass asset has not been imported, the actor deliberately falls back to its plain green material. Verify foliage color in viewport **Lit** mode; **Shader Complexity** and other diagnostic views intentionally show false colors.

## Locomotion animation
The runtime Mage Walk/Run playback rate now follows planar movement speed (base multipliers are tunable on `AAetherCharacter`), and each ambient walker scales its Walk rate to its own movement speed. This keeps gait cadence closer to world travel speed without restarting the sequence every tick.

A visible snap exactly at the end of each cycle is an animation-source loop seam, not a per-frame restart. After importing the Mage clips, open `A_Walk_Anim` and `A_Run_Anim` in the Unreal Animation Sequence editor and use **Asset → Add Looping Interpolation**, then save. For a durable reimport workflow, make the first/last poses cyclic in the source FBX before rerunning `Scripts/import_mage_assets.py`.

## Safety
Movement remains server-authoritative through Unreal CharacterMovement/replication. Sprint transitions use a server RPC; the server selects the configured speed and never trusts a client-provided speed value. Input does not write authoritative position, class, inventory or progression.
