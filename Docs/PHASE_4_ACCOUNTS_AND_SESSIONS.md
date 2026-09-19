# AGE OF AETHER — PHASE 4 ACCOUNTS AND SESSIONS

## Status

**Repository implementation:** COMPLETE  
**Repository/static validation:** PASSED  
**Unreal compilation:** NOT VERIFIED in this environment  
**Unreal multiplayer runtime:** NOT VERIFIED in this environment  
**Phase acceptance:** Repository gate complete; local Unreal validation remains required

## Objective

Phase 4 establishes the server-authoritative identity and connection-session foundation that sits above the Phase 3 networking transport.

The phase deliberately separates account identity, authentication verification, session lifecycle, permissions, runtime state and future persistence.

It does not pretend that an in-memory runtime registry is a production database.

## Implemented

### Account identity

FAetherAccountId provides a stable runtime account identifier generated from a GUID string.

FAetherAccountRecord contains:
- AccountID;
- normalized username;
- account status;
- permission mask.

Usernames are normalized by trimming surrounding whitespace and converting to lowercase.

### Account status

Current statuses:
- Active;
- Disabled;
- Locked.

Disabled and locked accounts cannot authenticate.

Changing an active account to a non-active status also closes its current authoritative session.

### Permissions

The foundation defines granular permission flags:
- Player;
- VIP;
- Moderator;
- GM;
- Admin;
- Owner.

The server owns the permission value. It is returned as part of the authentication response and is not accepted from the client as authoritative state.

### Authentication abstraction

The runtime service accepts an injected FCredentialVerifier.

The account/session service never stores plaintext passwords.

The verifier receives normalized username and opaque credential proof.

The verifier is intentionally an abstraction boundary. A later authentication/backend implementation can connect it to secure credential storage, external identity, challenge-response, or another approved provider without rewriting session lifecycle rules.

If no verifier is configured, authentication returns AuthenticationUnavailable rather than accepting credentials blindly.

### Session identity

FAetherSessionId provides a unique runtime session identifier.

FAetherSessionRecord contains:
- SessionID;
- AccountID;
- state;
- creation time;
- last heartbeat time.

### Login

The authoritative server:
1. validates protocol compatibility;
2. validates the request shape;
3. normalizes the username;
4. delegates credential verification;
5. validates account status;
6. rejects a second simultaneous session for the same account;
7. creates a unique session;
8. records the session as active;
9. returns AccountID, SessionID and permissions.

The client never chooses the authoritative session state.

### Reconnect

Reconnect requires:
- AccountID;
- existing SessionID;
- credential proof;
- compatible protocol.

The server verifies ownership of the session, verifies credentials and account status, refreshes the heartbeat and returns the authoritative session.

A timed-out session cannot be reactivated by the reconnect operation.

### Logout

Logout is server-authoritative.

The server requires the controller's current authenticated session to match the requested session ID before closing it.

Closed sessions remain in the runtime registry for diagnostics until the runtime is destroyed.

### Heartbeat and timeout

Active sessions expose an authoritative heartbeat operation.

The runtime service has a configurable heartbeat timeout (30 seconds by default).

The GameInstance subsystem periodically expires sessions whose last heartbeat is older than the timeout.

The timeout state is TimedOut, and the account's active-session mapping is removed.

### Network integration

AAetherNetworkPlayerController now exposes:
- AuthenticateAccount;
- ReconnectAccount;
- LogoutAccount;
- SendSessionHeartbeat;
- IsAccountAuthenticated;
- GetAuthenticatedAccountId;
- GetSessionId.

Corresponding server RPCs validate and execute operations through UAetherAccountSessionSubsystem.

Client-facing delegates expose authentication/session responses and heartbeat acceptance.

Account-operation request IDs are tracked independently from the generic Phase 3 network request IDs.

This prevents account-operation replay/order checks from sharing mutable state with unrelated networking requests.

### Runtime subsystem

UAetherAccountSessionSubsystem is a UGameInstanceSubsystem.

It owns the runtime account/session service and a ticker used to expire timed-out sessions.

This lifetime is intentional: Unreal's GameInstance subsystem is automatically instantiated for the lifetime of a game instance, which makes it a suitable boundary for process/runtime session state. The subsystem remains a runtime service boundary, not a database. citeturn0search0turn0search3

## Security decisions

The phase explicitly avoids:
- plaintext password storage;
- client-selected AccountID authority;
- client-selected SessionID authority;
- client-selected permission authority;
- client-selected session state;
- blind acceptance when an authentication verifier is unavailable;
- fake persistent account storage.

Credential verification is still an infrastructure boundary. Production authentication hardening, rate limiting, abuse detection, transport security and anti-cheat belong to later security/backend work.

## Persistence boundary

Accounts are currently registered in memory through RegisterAccount.

This is deliberate.

The service is designed so that a later persistence layer can load accounts, provide credential verification, persist status/permissions and restore session-related state where appropriate.

The current phase does not write database code because persistence belongs to the later Persistence/Backend phase.

## Tests added

Source/AgeOfAether/Private/Tests/AetherAccountSessionTests.cpp

Coverage:
1. account registration;
2. username normalization;
3. duplicate account rejection;
4. credential rejection;
5. successful authentication;
6. permission propagation;
7. duplicate simultaneous login rejection;
8. heartbeat refresh;
9. reconnect;
10. logout;
11. closed-session heartbeat rejection;
12. timeout expiration;
13. diagnostic inspection of timed-out sessions;
14. disabled account rejection;
15. re-enabled account authentication;
16. incompatible protocol rejection before authentication;
17. protocol rejection creates no session.

These are Unreal Automation Framework tests.

## Local validation required

The following still require the user's Unreal Engine 5.8.1 environment:

1. Generate/update project files.
2. Run UHT successfully.
3. Compile AgeOfAetherEditor.
4. Open the project in Unreal Editor.
5. Start a listen/server PIE instance.
6. Configure a development credential verifier from C++ test/bootstrap code.
7. Register a test account on the authoritative server.
8. Authenticate from a client.
9. Verify the client receives the accepted response and session ID.
10. Verify duplicate login is rejected.
11. Send session heartbeat repeatedly.
12. Stop heartbeat and verify server timeout.
13. Reconnect within the timeout window and verify the session is restored.
14. Attempt reconnect after timeout and verify SessionExpired.
15. Disable the account and verify subsequent authentication is rejected.
16. Logout and verify the session becomes closed.
17. Attempt logout/heartbeat using another session ID and verify rejection.
18. Run AgeOfAether.Accounts.* automation tests.
19. Repeat important flows with Unreal network emulation.

Perfect local connectivity is not sufficient to validate MMORPG networking. Latency/loss/jitter emulation should be included before treating the account/session runtime as network-validated.

## Known limitations

- The authentication verifier is an injected abstraction and is not yet backed by a database or external identity provider.
- Runtime accounts/sessions are intentionally non-persistent.
- No production password hashing/KDF implementation is embedded in gameplay code.
- No rate limiting, IP/device reputation, brute-force protection or anti-cheat exists yet; those belong to later security work.
- Unreal compilation and runtime multiplayer validation are not available in this environment.

## Definition of Done

Repository-side Phase 4 is complete when:
- AccountID exists;
- authentication abstraction exists;
- session exists;
- login exists;
- logout exists;
- reconnect exists;
- timeout exists;
- heartbeat exists;
- permissions exist;
- account status exists;
- server authority is enforced;
- request ordering/replay protection exists;
- network controller integration exists;
- runtime subsystem exists;
- automation tests exist;
- roadmap and continuity are updated.

The local Unreal acceptance gate remains explicitly unverified.

## Next phase

**PHASE 5 — Character Foundation**

The next system will attach CharacterID and character lifecycle to the authenticated account/session boundary without moving authority to the client.
