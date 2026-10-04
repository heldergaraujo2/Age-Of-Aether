# AGE OF AETHER — PHASE 5: CREATURES, NPCs AND BOSSES 2D

## 1. Objective

Adapt the presentation of the existing living-world actors to the official premium 2D isometric direction without rebuilding gameplay.

This phase covers the visual presentation layer for creatures, NPCs and bosses.

The implementation is deliberately actor-agnostic so the same presentation contract can be attached to existing AI, quest, combat, loot, navigation, spawn, persistence and authority actors.

## 2. Non-negotiable preservation rule

Phase 5 does not recreate creature AI, NPC AI, boss logic, combat, damage, skills, loot, quests/events, navigation, spawning/despawning rules, networking/server authority, persistence or progression.

The new layer consumes the existing actor state and exposes presentation operations only.

Canonical rule: preserve → adapt → integrate → validate → expand.

## 3. New source architecture

### UAether2DLivingVisualProfile

Data-driven profile for a living actor's 2D presentation.

Fields:
- VisualProfileID — stable profile identity.
- VisualKind — Creature, NPC or Boss.
- FamilyID — visual family/variant grouping.
- Flipbooks — state-to-Flipbook mapping.
- VisualScale — 2D scale.
- SpriteWorldOffset — presentation offset.
- bUseAsPrimaryPresentation — whether the 2D layer becomes the primary visual presentation.
- bDriveStateFromMovement — whether movement drives Idle/Walk/Run.

Required source state: Idle.

Optional states: Walk, Run, Attack, Hit, Death, Cast and Interaction.

This intentionally reuses the Phase 4 visual state vocabulary rather than creating a second incompatible animation state system.

### UAether2DLivingVisualComponent

Reusable actor component that accepts a living visual profile, validates it, creates a UPaperFlipbookComponent on the owning actor, applies scale and world offset, drives Idle/Walk/Run from actor velocity when enabled, exposes SetVisualState calls for gameplay systems, keeps the visual layer disabled on Dedicated Server, and optionally hides an existing skeletal mesh when the 2D profile is explicitly configured as primary.

The component is intentionally not coupled to a particular creature, NPC or boss class.

## 4. Integration contract

Existing gameplay remains the source of truth.

Examples:
- existing AI decides that a creature attacks → existing gameplay/AI calls SetVisualState(Attack, false);
- existing damage system reports a hit → caller can request Hit;
- existing death flow reports final death → caller can request Death;
- existing spell/skill system requests a cast → caller can request Cast;
- existing interaction/dialogue flow can request Interaction.

The presentation layer never decides whether an attack, death, quest, loot event or skill actually happened.

## 5. Creature/NPC/boss variation

Variation is data-driven through VisualKind + FamilyID + VisualProfileID + Flipbooks.

This supports several creatures sharing a visual family while using different scale/asset variants, different NPC professions using different profiles while keeping the same actor/quest framework, bosses using the same state contract with unique high-quality art and timing, and elite variants reusing a family while replacing only the required visual assets.

No duplicate gameplay class is required merely to change presentation.

## 6. Animation quality contract

The permanent project requirement remains active.

Production assets must target extremely fluid motion, readable silhouettes, coherent isometric perspective, responsive anticipation and follow-through, convincing attack/hit/death transitions, consistent frame timing, consistent scale between actor families, high-resolution source art, clean silhouettes and transparent backgrounds, coherent lighting direction, readable effects and impact frames, and premium visual finish.

Source image, sprite sheet or Flipbook reference is not considered a completed Unreal runtime asset until it is legitimately materialized and validated.

## 7. Asset identity

Profiles must use stable IDs compatible with the Phase 3 pipeline.

Examples:
- AOA.Creature.Wolf.Idle
- AOA.Creature.Wolf.Attack
- AOA.Creature.Wolf.Death
- AOA.NPC.Merchant.Idle
- AOA.NPC.Merchant.Interaction
- AOA.Boss.AncientGuardian.Attack

The exact final production asset inventory remains a later content-production gate.

## 8. Networking and authority

The component is presentation-only.

Dedicated Server does not create or drive the Paper2D presentation component.

Gameplay/network state remains owned by the existing authoritative systems. Clients may render the appropriate presentation based on replicated/gameplay state, but this phase does not alter replication contracts.

## 9. Unreal materialization boundary

No .uasset, .umap or other Unreal binary was fabricated.

The repository now contains the source contracts and implementation needed to materialize living 2D presentations legitimately later.

The following remain deferred: importing/creating production textures, creating real Paper2D Sprites, creating real Flipbooks, assigning profiles in real Unreal assets, PIE validation, multiplayer validation, Dedicated Server validation and visual quality inspection in the running game.

## 10. Source files

Added:
- Source/AgeOfAether/Public/Characters/Aether2DLivingVisualProfile.h
- Source/AgeOfAether/Private/Characters/Aether2DLivingVisualProfile.cpp
- Source/AgeOfAether/Public/Characters/Aether2DLivingVisualComponent.h
- Source/AgeOfAether/Private/Characters/Aether2DLivingVisualComponent.cpp

The implementation reuses the existing Phase 4 state contract and the existing Paper2D module dependency.

## 11. Validation status

### Repository/source

🟩 COMPLETE FOR THE DECLARED SOURCE/ARCHITECTURE SCOPE

Implemented: creature/NPC/boss profile contract; data-driven visual identity; reusable actor component; Idle/Walk/Run movement presentation; explicit combat/interaction presentation states; dedicated-server presentation guard; optional primary 2D presentation; validation of profile identity, family, Idle state, scale and asset references; documentation of integration and materialization boundaries.

### Unreal runtime

🟥 DEFERRED

No Unreal UHT/UBT/PIE/runtime result is claimed by this phase.

The user explicitly requested that Unreal tests and validation remain for a later dedicated validation window.

## 12. Completion boundary

Phase 5 is complete at the repository/source/architecture level.

It is not being represented as runtime-complete.

The next phase is Phase 6 — 2D environments and props.
