# AGE OF AETHER — PHASE 2 CORE RUNTIME

## Status

**Repository implementation:** COMPLETE  
**Unreal compilation:** NOT VERIFIED in this environment  
**Unreal automation execution:** NOT VERIFIED in this environment  
**Repository/source validation:** PASSED by inspection  
**Phase acceptance:** Pending local Unreal validation

## Scope

Phase 2 establishes the reusable runtime foundation required by later MMORPG systems:
- explicit runtime lifecycle state;
- health state;
- authoritative server clock abstraction;
- deterministic scheduler abstraction;
- runtime configuration;
- service registry;
- core logging category;
- runtime subsystem integration;
- graceful shutdown path through the scheduler tick;
- automation tests for core contracts.

The implementation does not introduce networking, accounts, characters, items or persistence ahead of their roadmap phases.

## Implemented

### Runtime lifecycle

UAetherRuntimeSubsystem owns the runtime lifecycle:

Uninitialized → Initializing → Running → ShuttingDown → Stopped

Configuration failures transition the runtime to Failed.

The subsystem is a UGameInstanceSubsystem, allowing the lifecycle to be owned by Unreal rather than by ad-hoc global state.

### Server clock

FAetherServerClock provides:
- UTC wall-clock access;
- monotonic uptime;
- explicit start/stop lifecycle.

Gameplay scheduling must use the monotonic clock for elapsed-time decisions rather than wall-clock time.

### Scheduler

FAetherScheduler supports:
- one-shot callbacks;
- repeating callbacks;
- cancellation;
- explicit ticking;
- invalid-delay rejection;
- clearing all pending tasks.

Tasks are anchored to the latest runtime tick, so scheduling a relative delay does not accidentally treat the delay as an absolute timestamp.

### Configuration

FAetherRuntimeConfig reads:
- bEnabled;
- bFailOnInvalidConfiguration;
- TickIntervalSeconds.

Defaults are stored in Config/DefaultGame.ini.

### Graceful shutdown

RequestShutdown transitions the runtime to ShuttingDown. The next runtime tick performs service shutdown, clears scheduled work, stops the server clock and removes the ticker callback. Unreal's Deinitialize path performs the same cleanup defensively.

### Service registry

FAetherServiceRegistry provides:
- unique service-name registration;
- initialization before registration;
- lookup;
- removal with shutdown;
- shutdown-all behavior.

The registry is intentionally generic so later systems can register services without introducing a global manager.

### Logging

LogAgeOfAetherCore is the dedicated log category for core runtime messages.

### Tests

Source/AgeOfAether/Private/Tests/AetherCoreRuntimeTests.cpp adds Unreal Automation Framework tests covering:
- runtime result success/failure contracts;
- scheduler timing, repetition, cancellation and invalid input;
- server clock lifecycle and uptime behavior.

Epic documents the Unreal Automation Framework as the C++ mechanism for unit/feature/content automation and provides command-line execution support. See the official Unreal Engine 5.8 documentation. 

## Validation performed here

### Repository/source inspection

Passed:
- all Phase 2 source files were created in the expected Public/Private module layout;
- runtime subsystem references the clock, scheduler, registry and configuration;
- configuration has safe defaults;
- scheduler timing semantics are internally consistent;
- tests cover the core scheduler/clock/result behaviors;
- no database, network, client-authority or future-phase implementation was introduced.

### Not executable in the current agent environment

The following require the user's Unreal development machine:
1. Generate/update Unreal project files.
2. Build AgeOfAetherEditor with Unreal Build Tool.
3. Run Unreal Header Tool.
4. Launch the Editor.
5. Execute AgeOfAether.Core.Runtime.* automation tests.
6. Inspect runtime logs during initialization/shutdown.
7. Confirm clean shutdown through the actual Unreal subsystem lifecycle.

These are not marked as passed.

## Architecture decision

Phase 2 intentionally keeps the runtime core small. It does not create a giant GameManager, and it does not create placeholder implementations for future gameplay systems.

Epic's Unreal documentation describes modules as the basic building blocks of the engine architecture and recommends separating public/private code and dependencies through module build rules. The Phase 2 implementation follows that model.

## Exit criteria

Phase 2 repository work is complete when:
- runtime state contract exists;
- health contract exists;
- clock exists;
- scheduler exists;
- configuration exists;
- service registry exists;
- logging exists;
- Unreal subsystem owns initialization/deinitialization;
- automated tests exist.

The remaining acceptance gate is local Unreal compilation and test execution.