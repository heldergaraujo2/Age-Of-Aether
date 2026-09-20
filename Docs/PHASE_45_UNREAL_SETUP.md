# Phase 45 — Minimal Unreal Setup

The goal is one reusable skill pipeline, not dozens of Blueprints.

## First test
Use the automatically registered aether.skill.training_strike.

### Character
Use the existing AAetherCharacter. The skill visual component is already attached.

### Visual profile
Create one Blueprint/Data Asset wrapper for FAetherSkillVisualProfile using the project's preferred designer asset workflow, then assign:
- SkillID: aether.skill.training_strike
- optional cast montage
- optional cast VFX
- optional impact VFX
- optional SFX

All visual references are soft/optional.

### Input
For the first vertical slice, use a Blueprint button or existing UI action to call CastSkill with the training SkillID and TargetCharacterId.

The server is authoritative. Do not call the skill subsystem directly from client Blueprint.

### Acceptance
Test:
- valid target;
- target out of range;
- repeated cast before cooldown;
- dead attacker;
- dead target;
- invalid SkillID;
- target spoofing;
- 2-client PIE;
- Dedicated Server.

No Unreal binary asset is required in Git for this source-level phase.

## Recommended next visual step
Once one skill is visually working, duplicate only the data/profile and swap assets. Do not create per-skill C++ code.
