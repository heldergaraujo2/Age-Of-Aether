# AGE OF AETHER — PHASE 7: 2D DEPTH, LIGHTING AND SHADOWS

## Objective
Create the repository/source foundation for convincing depth in the premium 2D isometric presentation without requiring a 3D world.

## Architecture

### UAether2DDepthLightingProfile
Data-driven depth/presentation policy with stable ProfileID, semantic DepthLayer, RenderLayer, ParallaxFactor, DepthOffset, optional shadow casting and optional parallax.

ParallaxFactor is normalized from 0 to 1: 1.0 is world-locked and 0.0 is maximum camera-relative movement for this policy.

### UAether2DDepthLightingComponent
Reusable presentation component that validates the profile, resolves a no-collision primitive presentation component, applies render ordering and shadow policy, optionally applies camera-relative parallax, and skips presentation behavior on Dedicated Server.

## Depth model
The intended composition uses background, far layers, world layer, midground, foreground and overlay. Render ordering is data-driven so trees, buildings, characters, props, VFX and foreground occluders can be layered deliberately.

## Parallax
Parallax is an optional presentation effect controlled by profile data. It is not a replacement for world geometry, collision, navigation or gameplay positions.

## Lighting and shadows
The source contract supports shadow policy at the presentation layer. Real Unreal lighting, material response and shadow quality remain runtime materialization concerns. No fake lighting system or binary asset is fabricated.

## Future extension points
The depth layer is designed to support foreground occlusion, atmospheric layers, fog, weather, particles, magic VFX, environmental animation and additional material-driven lighting effects. These remain later production/runtime work.

## Gameplay preservation
No gameplay system is rebuilt. Movement, collision, navigation, combat, AI, quests, inventory, economy, persistence, networking and world streaming remain existing systems.

## Quality contract
Depth must reinforce clear isometric readability, natural lighting direction, visual hierarchy, silhouette readability, believable occlusion, premium atmosphere, smooth presentation and regional consistency.

## Materialization boundary
No Unreal binary assets were fabricated. Real material creation, production sprites/Flipbooks, actual lighting setup, shadow inspection, atmospheric effects, particle/VFX integration, PIE, multiplayer, Dedicated Server, profiling and final visual acceptance remain deferred.

## Completion status
🟩 Repository/source/architecture complete for the declared Phase 7 scope.
🟥 Unreal materialization and runtime validation deferred by project decision.

Next: Phase 8 — isometric camera and exploration.