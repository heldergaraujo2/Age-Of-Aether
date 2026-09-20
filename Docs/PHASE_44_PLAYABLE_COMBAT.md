# PHASE 44 — PLAYABLE COMBAT

## STATUS
Repository/source implementation complete. Unreal 5.8.1 runtime acceptance remains local.

## IMPLEMENTADO
- Reused the existing server-authoritative `UAetherCombatSubsystem` / `FAetherCombatService` instead of creating a second combat authority.
- Added a low-friction basic attack input to `AAetherCharacter` using left mouse.
- Client performs only target acquisition: short camera-forward line trace against Pawn.
- The client sends only the target CharacterID through the existing authenticated network combat gateway.
- Server-side combat remains responsible for ownership, cooldown, range, state, deterministic damage and persistence/application of combat state.
- Added optional `BasicAttackMontage` to the existing reusable animation profile.
- Added `PlayBasicAttackAnimation()` to the visual component.
- Attack animation is presentation-only and never decides damage.
- Dedicated Server does not load/play the attack montage.
- No per-class combat C++ and no per-evolution Blueprint is required.

## LOW-BUREAUCRACY UNREAL WORKFLOW
1. Import one real character FBX.
2. Create/reuse one Animation Profile.
3. Assign one Basic Attack Montage.
4. Play in PIE and click an in-range character.
5. Reuse the same montage/profile for other classes until their unique animations are ready.
6. Later class/evolution-specific montages are data assignments only.

## SECURITY
The client does NOT submit damage, health, crit, cooldown or position as authoritative combat state. It only requests an attack against a CharacterID selected from the local trace. The existing server combat subsystem resolves the request.

## LOCAL ACCEPTANCE
- PIE single player: attack input fires without errors.
- PIE two clients: client can target another character and request an attack.
- Verify server combat result/cooldown.
- Verify attack montage plays locally when configured.
- Verify removing the montage leaves combat functional.
- Dedicated Server + client: no animation asset loading on server and combat request remains server authoritative.
- Test repeated clicks against cooldown and out-of-range targets.

## LIMITS
Full skill system, buffs/debuffs, hit VFX, monster/NPC combat and combat UI are later phases.

## RESULT
The first playable combat slice is now connected end-to-end: input -> target acquisition -> authenticated server combat -> existing deterministic combat resolution, with an optional data-driven attack animation layer.
