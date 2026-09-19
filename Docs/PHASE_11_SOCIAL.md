# PHASE 11 — SOCIAL

## Status

Repository implementation: **COMPLETE**.  
Repository/static validation: **PASSED**.  
Unreal 5.8.1 compilation/runtime validation: **NOT VERIFIED** because Unreal is unavailable in this execution environment and no CI build exists.

## Scope

Phase 11 establishes the first server-authoritative social foundation:
- friends and relationship requests;
- block/unblock;
- parties;
- party invitations and membership;
- guilds;
- guild invitations and roles;
- authoritative chat validation and routing;
- Blueprint-facing controller events;
- runtime subsystem boundary;
- automation coverage.

The implementation intentionally does not make an external online-service provider a gameplay dependency. Unreal 5.8's Online Services framework exposes Social, Sessions, Presence, Lobbies and other provider interfaces, but Epic documents the Online Services framework as Beta and notes it has not been tested in shipping titles. The native Age of Aether social domain therefore remains provider-agnostic; a later backend/online-service adapter can integrate with EOS or another provider without replacing gameplay authority. citeturn0search1turn0search5

## Friends

Implemented:
- pending outgoing/incoming requests;
- accept;
- reject;
- remove;
- block;
- unblock;
- deterministic friend listing;
- self-target rejection;
- blocked-account rejection.

Relationship state is keyed by AccountID rather than CharacterID, so changing characters does not implicitly destroy the account-level social relationship.

## Party

Implemented:
- create party;
- one leader;
- member roles;
- invitations;
- invite acceptance;
- maximum party size;
- leave;
- leader handoff when the leader leaves;
- leader kick;
- account and character membership indexes.

Default maximum party size: 5.

Party membership is character-based, with account indexes preventing one account from occupying multiple parties simultaneously.

## Guild

Implemented:
- create guild;
- normalized unique guild name;
- leader;
- officer/member role model;
- invitations;
- invitation acceptance;
- maximum guild size;
- role changes by leader;
- member leave;
- leader cannot abandon a non-empty guild without first transferring leadership.

Default maximum guild size: 100.

## Chat

Implemented:
- Local;
- Party;
- Guild;
- Whisper;
- System channels;
- message normalization;
- maximum message length;
- server-side cooldown;
- blocked-account protection for whispers;
- authoritative recipient routing.

The client submits a message request, but the server validates the character, channel membership, target, block state, rate limit and message size before delivery.

Local messages are routed by server distance using a 2000 Unreal-unit radius.

Party and guild messages are routed from authoritative membership state.

Whispers are routed by AccountID.

System messages are represented by the same message contract for future server-generated notifications.

## Networking

`AAetherNetworkPlayerController` now exposes server-authoritative requests for:
- friends;
- friend requests;
- accept/reject/remove/block/unblock;
- party create/invite/accept/leave/kick;
- guild create/invite/accept/leave/role changes;
- chat.

Social operations have their own monotonically increasing RequestID sequence.

For character-based operations:
- AccountID is derived from authenticated session state;
- CharacterID is derived from the active Character PlayerState;
- the client cannot submit authoritative ownership.

## Architecture

Added:
- `FAetherSocialService`
- `UAetherSocialSubsystem`
- `FAetherSocialConfig`
- `UAetherSocialConfigDataAsset`
- friend/party/guild/chat contracts.

The service is runtime-only. Persistence is deliberately deferred to Phase 14.

## Tests

Added automation tests covering:
1. friend request;
2. duplicate request;
3. friend acceptance;
4. friend removal;
5. blocking;
6. unblock;
7. party creation;
8. party invitation;
9. party acceptance;
10. party kick;
11. party leave;
12. party capacity;
13. guild creation;
14. guild invitation;
15. guild acceptance;
16. guild role change;
17. guild member leave;
18. guild leader lifecycle;
19. self-target protection;
20. non-leader party authority;
21. invalid chat;
22. chat normalization;
23. chat rate limiting.

## Security

The client cannot:
- assign itself party leader;
- assign itself guild leader;
- modify another account's friend state;
- bypass party/guild membership checks;
- bypass block state for whispers;
- send chat without an active character;
- choose another character as the sender;
- bypass server chat rate limits;
- choose recipient membership for party/guild delivery.

The server derives social authority from authenticated AccountID and active CharacterID.

## Persistence boundary

Not implemented here:
- database persistence;
- crash recovery;
- cross-server social synchronization;
- durable friend/guild repositories;
- offline notifications;
- mail;
- guild bank;
- guild progression;
- guild wars;
- party loot rules;
- matchmaking;
- voice chat.

Those remain future phases.

## Unreal Online Services boundary

Epic's Online Services documentation exposes provider-neutral Social and Sessions interfaces and describes asynchronous provider communication. Sessions support invitation and membership concepts, while the Social interface handles relationships and blocking. These are future integration adapters, not a replacement for Age of Aether's server-authoritative domain model. citeturn0search0turn0search2

## Validation truth

Repository/static checks completed:
- balanced delimiters;
- no escaped-newline artifacts;
- all social RPC declarations and implementations reconciled;
- subsystem/service methods reconciled;
- controller social delegates present;
- chat routing uses server-side membership;
- character cleanup does not mutate arrays during range iteration;
- no fake Unreal content assets were created.

Not executable here:
- Unreal Header Tool;
- Unreal Build Tool;
- C++ compilation against UE 5.8.1;
- Editor startup;
- PIE;
- multiplayer replication;
- Automation Framework execution;
- network emulation.

Therefore the phase is repository-complete, not falsely marked Unreal-runtime-complete.

## Next phase

**PHASE 12 — Economy & Crafting**
