# PHASE 44 — MINIMUM UNREAL SETUP

Only one animation assignment is required to make the first combat slice visible.

### One-time setup
- Open the existing character Animation Profile.
- Assign a real `UAnimMontage` to `Basic Attack Montage`.
- No new character Blueprint is required.
- No combat Blueprint is required.

### Input
Left mouse is created by the native foundation input bootstrap.

### Targeting
The character performs a short camera-forward Pawn trace. If it hits another `AAetherCharacter`, only that target's stable CharacterID is sent to the existing combat gateway.

### Safety
Do not implement damage calculations in Blueprint.
Do not trust client damage/health/crit values.
The server combat service remains the only authority.

### Reuse
The same animation profile can be shared by all five classes initially. Later class/evolution montages can be assigned as data without new C++.
