# AGE OF AETHER — PHASE 5 CHARACTER FOUNDATION

## Status

Repository implementation: COMPLETE  
Repository/static validation: PASSED  
Unreal compilation: NOT VERIFIED in this environment  
Unreal multiplayer runtime: NOT VERIFIED in this environment  
Phase acceptance: Repository gate complete; local Unreal validation remains required

## Objective

Phase 5 connects authenticated accounts/sessions to persistent-style character identity and a server-authoritative playable character representation.

The phase establishes the identity and lifecycle foundation without introducing persistence/database code prematurely.

Unreal's gameplay framework separates GameMode rules, PlayerState replicated player information, PlayerController ownership/input, and Pawn/Character world representation. AGE OF AETHER follows that separation for the character foundation. citeturn0search2turn0search3turn0search4

## Character identity

FAetherCharacterId provides a generated runtime character identifier.

FAetherCharacterRecord contains:
- CharacterID;
- AccountID;
- Name;
- Class;
- Status;
- Level;
- Experience;
- BaseStats;
- DerivedStats;
- WorldLocation;
- WorldRotation.

The AccountID is authoritative and comes from the authenticated session. Character ownership is never accepted from the client.

## Character names

Names are:
- trimmed;
- lowercased for canonical storage;
- unique globally in the runtime registry;
- restricted to 3-16 characters.

The runtime registry rejects duplicates independent of letter casing or surrounding whitespace.

Character status transitions are controlled by the server-side service. Disabled and Deleted characters are excluded from selection, and Deleted characters are hidden from the account character list.

## Character limit

The foundation currently permits five characters per account.

This is a gameplay policy constant, not a database constraint. A later data/configuration system can replace it without changing the ownership model.

## Character classes

The foundation defines:
- Warrior;
- Mage;
- Archer;
- Cleric.

These are intentionally minimal identity classes. Combat, skill trees, equipment restrictions and progression rules belong to later phases.

## Lifecycle

Current lifecycle states:
- Created;
- Available;
- Selected;
- Active;
- Offline;
- Disabled;
- Deleted.

Creation produces an Available character.

Selection requires the authenticated account to own the character. The selected character becomes Active.

Only one character may be selected for an account at a time in the runtime service.

Deselecting transitions the character to Offline.

Disabled and Deleted characters cannot be selected.

## Base stats

The initial base-stat contract contains:
- Strength;
- Agility;
- Vitality;
- Energy;
- Command.

Defaults are intentionally conservative and data-oriented.

The foundation does not yet implement level-up, stat allocation, equipment modifiers or buffs.

## Derived stats

The foundation calculates:
- MaxHealth;
- MaxMana;
- AttackMin;
- AttackMax;
- Defense;
- MoveSpeed.

The formulas are only a baseline for the runtime contract. They are not the final combat balance.

Later progression/combat phases can replace the calculation strategy without changing the identity/lifecycle contract.

## Server-authoritative location

The character record owns:
- WorldLocation;
- WorldRotation.

The service only accepts location updates for an Active character owned by the supplied authenticated account.

The client does not directly mutate the character service.

Actual Unreal Character movement will use the engine's Character/Pawn networking model and must be validated in a real multiplayer session before being considered complete.

## Character service

FAetherCharacterService provides:
- creation;
- lookup by ID;
- lookup by name;
- account ownership validation;
- account character listing;
- selection;
- deselection;
- location update;
- status update;
- selected-character lookup;
- account character count.

The service is independent of Unreal Actors so it can be tested without requiring a running world.

## Character subsystem

UAetherCharacterSubsystem exposes the character service through the GameInstance runtime boundary.

This is runtime state, not persistence.

The later persistence/backend phase will be responsible for loading and saving durable character records.

## PlayerState

AAetherCharacterPlayerState replicates:
- AccountID;
- CharacterID;
- CharacterName;
- CharacterClass;
- CharacterLevel.

PlayerState is the correct multiplayer-visible boundary for relevant player information because Unreal replicates PlayerStates to clients, unlike PlayerControllers, which are local to their owning machine. citeturn0search2turn0search5

## Character Actor

AAetherCharacter derives from ACharacter.

It:
- replicates;
- replicates movement;
- carries replicated CharacterID;
- binds possession to the server-authoritative character identity.

Character uses the existing authenticated PlayerController.

## GameMode integration

AAetherNetworkGameMode now:
- uses AAetherCharacterPlayerState;
- uses AAetherCharacter as DefaultPawnClass;
- starts players as spectators;
- exposes server-side character spawning after successful character selection.

This prevents the server from automatically creating a playable character before authentication and character selection.

Unreal's GameMode controls player entry/spawn rules on the server, while PlayerState carries replicated player information. citeturn0search10turn0search11

## Network API

AAetherNetworkPlayerController now exposes:
- RequestCharacterList;
- CreateCharacter;
- SelectCharacter;
- DeselectCharacter.

Server RPCs perform:
- authentication check;
- request ordering/replay protection;
- ownership validation;
- character lifecycle validation;
- server-side spawn.

Client receives:
- character list;
- character operation result;
- resulting character record.

## Logout behavior

When an authenticated session logs out:
1. the session is closed;
2. the selected character is deselected;
3. the active pawn is destroyed;
4. the controller authentication state is cleared.

This prevents the old character from remaining associated with a disconnected authenticated identity.

## Security decisions

The client cannot:
- create a character for another account;
- select another account's character;
- select a deleted/disabled character;
- create more than the account limit;
- force a character state;
- directly write the authoritative character registry;
- select the authoritative spawn location.

The server owns all of those decisions.

## Persistence boundary

Character data is currently runtime-only.

No database fake was introduced.

The later persistence phase will provide:
- durable character storage;
- transactions;
- migrations;
- crash recovery;
- load/save;
- persistence of location and progression.

## Tests

Added:

Source/AgeOfAether/Private/Tests/AetherCharacterTests.cpp

Coverage:
1. character creation;
2. generated CharacterID;
3. name normalization;
4. initial level/status;
5. derived stat calculation;
6. lookup by name;
7. ownership;
8. cross-account selection rejection;
9. account character limit;
10. name length boundaries;
11. duplicate names;
12. selection;
13. selected-character lookup;
14. one-active-character rule;
15. deselection;
16. location update authority;
17. character listing;
18. deterministic character-list ordering.

These are Unreal Automation Framework tests.

## Local validation gate

The following require Unreal Engine 5.8.1 locally:

1. Generate/update Visual Studio project files.
2. Run UHT.
3. Compile AgeOfAetherEditor.
4. Open Editor.
5. Start listen/server PIE.
6. Authenticate a test account.
7. Create at least two characters.
8. Request character list.
9. Select a character.
10. Verify server spawns AAetherCharacter.
11. Verify AAetherCharacterPlayerState replicates identity to clients.
12. Verify CharacterID replicates.
13. Verify movement in a client/server session.
14. Verify server correction under network emulation.
15. Verify a client cannot select another account's character.
16. Verify duplicate active-character selection is rejected.
17. Logout and verify pawn destruction and character deselection.
18. Reconnect/authenticate and verify character can be selected again.
19. Run AgeOfAether.Character.* automation tests.

Perfect local connectivity is not enough to validate MMORPG character networking. Latency, packet loss and jitter should be exercised before marking runtime networking as validated.

## Known limitations

- Character records are not persistent yet.
- Character creation does not yet use a data-driven class configuration asset.
- Level progression is not implemented.
- Stat allocation is not implemented.
- Equipment modifiers are not implemented.
- Combat is not implemented.
- Skills are not implemented.
- Production character-name reservation is not distributed/persistent.
- Runtime movement/network behavior still requires local Unreal validation.

## Definition of Done

Repository-side Phase 5 is complete when:
- CharacterID exists;
- account ownership exists;
- name rules exist;
- class exists;
- lifecycle exists;
- base stats exist;
- derived stats exist;
- location exists;
- creation exists;
- listing exists;
- selection exists;
- deselection exists;
- PlayerState replication exists;
- Character Actor exists;
- server-authoritative spawn exists;
- logout cleanup exists;
- automation tests exist;
- roadmap and continuity are updated.

Local Unreal validation remains explicitly unverified.

## Next phase

PHASE 6 — Item and Inventory System

The next system must attach inventory ownership to CharacterID rather than AccountID alone, while preserving server authority and data-driven item definitions.
