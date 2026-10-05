# PHASE 40 — INPUT, MOVEMENT & CAMERA

## STATUS
**Implemented at repository/source level.** Unreal 5.8.1 compile and PIE acceptance remain a local gate.

## IMPLEMENTED
- Added optional `UAetherMovementCameraProfile` Data Asset for movement/camera tuning.
- Added validation for finite, safe movement and camera ranges.
- Added configurable walk/sprint speed, jump velocity, rotation rate, camera distance range, zoom step and pitch limits.
- Extended the runtime Enhanced Input fallback with:
  - left-click-to-move using the cursor hit point (with a ground-plane fallback);
  - right-click basic attack;
  - F5 toggles 3D orbit-camera control without discarding the last orbit/zoom view;
  - while orbit control is active, hold the middle mouse button and drag to rotate the camera through 360 degrees;
  - F6 resets pitch, yaw and zoom to the original camera view;
  - Mouse Wheel zoom, including close inspection in orbit mode;
  - Space jump;
  - Left Shift sprint.
- Removed W/A/S/D movement bindings. CharacterMovement still performs and replicates movement; the client does not send a trusted speed or raw position.
- Shows the mouse cursor and uses a Game-and-UI input mode so the player can select destinations during play.
- Added server-authoritative sprint transition through a Server RPC. The server chooses the configured speed; the client never supplies a speed value.
- Added camera pitch clamping.
- Preserved Unreal CharacterMovement replication/authority.
- Kept the first-playable workflow asset-light: no mandatory hand-authored Input Action or Mapping Context assets.
- Updated the in-game debug HUD control hint.
- Added Automation coverage for movement/camera profile validation.
- Updated continuity and roadmap documentation.

## SIMPLE WORKFLOW
For the first playable slice:
1. Import the real FBX.
2. Create one visual profile and assign the Skeletal Mesh.
3. Optionally create one movement/camera profile and tune values.
4. Put the character in the development map.
5. Press Play.

No per-class or per-character input C++ is required.

## DEFAULT CONTROLS
- Left mouse click = move to the clicked ground position
- Right mouse click = basic attack
- F5 = toggle 3D orbit-camera control on/off; the last view is retained
- Hold middle mouse button and drag = orbit 360 degrees while F5 orbit control is active
- F6 = reset camera to its original pitch, yaw and zoom
- Mouse wheel = zoom, down to a close inspection distance during orbit mode
- Space = jump
- Left Shift = sprint

## SECURITY
- Position remains governed by Unreal CharacterMovement/server replication.
- Sprint RPC accepts only a boolean state; speed is selected server-side.
- Camera and input presentation never become gameplay authority.
- No client-provided class, item, progression or movement speed is trusted.

## COMPILATION / RUNTIME TRUTH
Repository/source implementation is complete and statically structured for Unreal 5.8.1. Actual UHT/UBT, PIE, two-client PIE and Dedicated Server runtime execution are **NOT VERIFIED in this environment**.

## ACCEPTANCE GATE
Local Unreal verification must confirm:
- project opens without critical errors;
- profile Data Asset can be created;
- character spawns;
- left click moves to a ground destination;
- right click triggers the basic attack;
- F5 enables/disables orbit control without resetting the view;
- middle-button drag rotates the camera smoothly through a full yaw revolution;
- F6 resets the original camera view;
- orbit zoom can inspect the character closely and remains stable when orbit control is toggled off;
- pitch clamp works;
- Space jumps;
- Shift sprint reaches configured server-authoritative speed;
- mouse wheel zoom stays inside configured range;
- 2-client PIE replicates movement;
- Dedicated Server + client works;
- Automation passes;
- Output Log has no new critical errors.

## NEXT
**PHASE 41 — ANIMATION SYSTEM.**
