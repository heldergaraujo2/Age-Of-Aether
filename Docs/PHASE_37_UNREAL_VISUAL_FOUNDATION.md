# PHASE 37 — UNREAL VISUAL FOUNDATION

## STATUS
**Repository implementation: COMPLETE.**
**Unreal 5.8.1 runtime acceptance: PENDING LOCAL EXECUTION.**

This phase establishes the lowest-friction Unreal foundation needed to turn the existing server-authoritative architecture into a playable visual project without fabricating binary Unreal assets in GitHub.

## IMPLEMENTADO

### 1. Unreal project foundation
- Enhanced Input enabled explicitly in AgeOfAether.uproject.
- EnhancedInput added to the module dependencies.
- Existing server-authoritative GameMode, GameState, PlayerController and Character PlayerState remain the authoritative multiplayer foundation.
- UAetherGameInstance added and configured through DefaultGame.ini.

### 2. Playable character foundation
AAetherCharacter now provides:
- replicated character identity;
- server-controlled movement replication;
- character movement defaults;
- rotation toward movement;
- capsule sizing;
- spring-arm third-person camera;
- camera collision;
- follow camera;
- mouse look;
- WASD movement;
- Space jump;
- Enhanced Input bindings created automatically at runtime when no authored input assets exist.

The runtime fallback intentionally avoids forcing Helder to create several Input Action/Mapping Context assets before the first playable test. Authored Enhanced Input assets can replace this fallback later without changing the gameplay authority model.

### 3. Development world foundation
AAetherDevelopmentWorldActor was added:
- uses Unreal's built-in BasicShapes cube as temporary ground;
- creates collision suitable for the placeholder character;
- is spawned by the server when the level has no development ground;
- does not require a fabricated .umap or .uasset file.

The GameMode also creates a PlayerStart automatically when the test level has none. This keeps the first PIE test extremely simple.

### 4. HUD/debug presentation
AAetherFoundationHUD was added with a lightweight native HUD:
- current foundation state;
- character presence;
- WASD/mouse/Space controls;
- server/client authority indicator.

It is intentionally a native debug HUD rather than a Widget Blueprint at this stage, so visual gameplay can start immediately and the full MMORPG UI can be assembled in Phase 51.

### 5. Asset Manager foundation
UAetherAssetManager was added and configured as the project Asset Manager.
UAetherVisualFoundationDataAsset provides:
- stable AssetID;
- DefinitionID;
- primary asset identity;
- visual asset reference;
- fallback asset reference;
- version.

DefaultEngine.ini scans /Game/Aether for AetherVisual primary assets.

This connects future real assets to the existing stable-ID presentation architecture without committing fake binary assets.

### 6. Collision foundation
Project trace channels were named:
- AetherInteract;
- AetherTarget;
- AetherMovement.

They are intentionally separated so later interaction, targeting and movement validation can evolve without reusing generic engine channels.

### 7. Automation/static validation
Added AetherVisualFoundationTests.cpp covering:
- Primary Asset ID generation;
- character camera component existence;
- foundation class availability.

The repository workflow now validates:
- required Phase 37 files;
- Unreal project JSON;
- Enhanced Input plugin/dependency;
- Asset Manager configuration;
- primary asset scan configuration;
- collision channel configuration;
- GameInstance configuration;
- character input/camera/replication contract;
- GameMode development-world and PlayerStart setup;
- absence of fabricated .uasset and .umap binaries.

## COMPILADO

**Repository/static contract: PASS.**

The GitHub Actions workflow was extended to audit Phase 37.

**Unreal UHT/UBT compilation: NOT EXECUTED HERE.**

The repository does not contain an Unreal 5.8.1 installation/runtime, so claiming a real UHT/UBT compile would be false.

## TESTADO

Static tests/checks implemented:
- project structure;
- source delimiter audit;
- Phase 19–36 regression contract;
- Phase 37 configuration contract;
- no fake Unreal binary assets.

Unreal Automation tests were authored but cannot be executed in this environment.

## VALIDADO

Validated design boundaries:
- server remains authoritative;
- client presentation cannot grant gameplay authority;
- stable IDs are preserved;
- no visual Unreal path is sent as gameplay authority;
- real assets remain optional until Phase 38;
- first visual test does not require manually authored Input Action assets;
- no fake .uasset or .umap files were committed;
- the development floor and PlayerStart are generated at runtime to reduce setup friction.

Epic's Unreal 5.8 documentation confirms Enhanced Input uses Input Actions and Mapping Contexts and supports runtime context management, and that custom Asset Manager classes are configured through AssetManagerClassName in DefaultEngine.ini.

## FALHAS

No repository/static failure remains known.

The only unclosed gate is the actual Unreal runtime gate:
- UHT;
- UBT Editor;
- UBT Game;
- Dedicated Server;
- Editor startup;
- level startup;
- PIE;
- input;
- camera;
- multiplayer PIE;
- Automation Framework.

## PENDÊNCIAS

A real Unreal Editor machine must perform the final acceptance:
1. Open AgeOfAether.uproject in Unreal Engine 5.8.1.
2. Allow modules to compile.
3. Create one empty level and save it as Content/Aether/Maps/Aether_Development.umap.
4. Set that level as the Game Default Map.
5. Press Play.
6. Confirm the placeholder character appears.
7. Confirm WASD movement.
8. Confirm mouse camera.
9. Confirm Space jump.
10. Confirm the debug HUD.
11. Run the Phase 37 Automation tests.
12. Run a 2-client PIE session and confirm the character remains server-authoritative.
13. Run a Dedicated Server + client smoke test.
14. Check Output Log for critical errors.

The only manually created binary asset required to close this phase is the development map itself. Everything else needed for the first playable test has already been prepared in source/config.

## RISCOS

- The runtime-created Enhanced Input context is a bootstrap/fallback path. Phase 40 can replace it with authored Input Actions/Mapping Contexts after the pipeline is proven.
- The development ground uses Unreal Engine BasicShapes and is not production art.
- The native HUD is intentionally temporary and will be replaced/expanded by the real MMORPG UI.
- Actual FBX/skeleton/material/animation validation belongs to Phases 38–43.

## PRÓXIMO PASSO

**PHASE 38 — REAL ASSET PIPELINE**

Before mass asset production, close the local Unreal 5.8.1 runtime gate above. Then import the first real FBX, establish the definitive skeleton/material/Physics Asset/LOD/socket pipeline, register its stable AssetID, and validate it in the Phase 36 presentation contract.

## SIMPLIFICAÇÃO PARA HELDER

The first Unreal test is intentionally reduced to:

**Open project → create one Empty Level → save it → Set as Game Default Map → Play.**

No manual creation of character Blueprint, Input Action assets, Input Mapping Context, floor mesh, PlayerStart or debug widget is required for the first foundation test.