# Phase 56 — Minimal Unreal Release Setup

## One release gate
Create/configure one UAetherReleaseGate in the existing production/build validation flow.

Populate:
- Version
- BuildId
- ContentPackageID
- ContentVersion
- GitCommit

Run existing Automation tests first.

## Alpha
- Editor/Game build
- initial content package loads
- character movement/camera
- class/evolution presentation
- basic attack
- training skill
- creature spawn
- inventory/equipment
- crafting/shop
- HUD/inventory/character/skills widgets
- audio playback

## Beta
All Alpha checks plus:
- 2-client PIE
- server authority validation
- disconnect/reconnect
- persistence
- map/streaming transition
- performance budget/profiling
- Dedicated Server

## Release Candidate
All Beta checks must pass with:
- no blocking/crash errors;
- clean packaged build;
- validated initial content package;
- real imported character/creature/item/UI/audio assets;
- release manifest and build ID recorded.

The gate does not invent or infer these results. Each result must be explicitly recorded.

## Visual shortcut
For the first complete slice, use:
- one playable character FBX;
- one reusable Anim Blueprint;
- one attack montage;
- one skill VFX;
- one creature FBX;
- one NPC presentation using the same creature actor;
- one boss using the same creature actor;
- a small set of item meshes;
- four initial UMG widgets;
- a small audio set.

After that slice is accepted, expand content by data rather than by new C++.
