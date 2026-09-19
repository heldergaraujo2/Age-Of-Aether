# PHASE 17 — AI / GPT INTEGRATION

## Status
Repository implementation: COMPLETE
Repository/static validation: PASS
Unreal 5.8.1 UHT/UBT/Editor/Automation/live provider runtime: NOT VERIFIED in this environment.

## Architecture

AI is advisory and server-controlled. The model never becomes gameplay authority.

Client -> authenticated server -> AI service -> provider adapter -> validated response -> authoritative gameplay/UI.

OpenAI's Responses API supports model responses and tool/function calling. AGE OF AETHER keeps the core gameplay module provider-agnostic so a server-side OpenAI adapter can be added without shipping credentials to clients. The provider boundary is deliberate. citeturn0search12

## Implemented

Added:
- AI request/result/tool enums;
- bounded AI configuration;
- character-scoped runtime memory;
- AI context contract;
- tool-proposal contract;
- provider interface;
- AI service;
- GameInstance AI subsystem;
- AI Data Asset configuration;
- automation tests.

The service provides:
- initialization/shutdown;
- per-account request budget;
- per-account monotonic replay protection;
- context-size limit;
- output-size limit;
- provider failure mapping;
- output leakage checks;
- tool-proposal validation;
- bounded memory with oldest-entry eviction.

## Gameplay authority

A proposal such as OfferQuest or OpenShop is only a proposal. The AI service does not execute it.

The future authoritative path is:
1. validate the proposal;
2. resolve authenticated account and active character;
3. call the existing quest/economy/world/social/combat API;
4. let that subsystem decide whether the operation is legal;
5. replicate the resulting authoritative state.

This preserves the project's server-authoritative architecture.

## Memory

AI memory is character-scoped and bounded. It is runtime-only in Phase 17. Durable memory belongs to the existing persistence/backend layer.

## Security

No API key, provider secret, bearer token or client-side provider dependency is added.

The service rejects obvious credential/system-prompt leakage markers and invalid tool proposals. This is a defense-in-depth filter, not a substitute for provider safety controls or authoritative gameplay validation.

## Tests

AetherAITests.cpp covers:
1. configuration validation;
2. successful provider request;
3. per-account replay rejection;
4. independent account request IDs;
5. per-account rate limiting;
6. rate-window expiry;
7. oversized context;
8. bounded memory;
9. unsafe output;
10. invalid tool proposal;
11. provider failure.

Static audits:
- C++ delimiter balance;
- no escaped-newline artifacts;
- provider isolated from gameplay module;
- no credentials committed;
- no direct client authority over AI actions.

## Unreal gate

Because Unreal 5.8.1 is unavailable in this environment, UHT, UBT, Editor, Automation Framework, PIE and live provider/network tests remain NOT VERIFIED. No claim of live GPT connectivity is made.

## Provider adapter plan

A production adapter may use OpenAI's server-side Responses API or another provider. Credentials must remain on the server. The adapter should also implement timeouts, retries with bounded backoff, provider request IDs, response-size limits and observability before production rollout.

## Next phase

PHASE 18 — PRODUCTION & LIVE MMORPG
