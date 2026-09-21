# Age Of AetheR — Unreal 5.8.1 Execution Continuity

> Canonical checklist for the local Unreal Engine execution phase. Updated after every command/result cycle.

## Rules
1. Never mark a runtime step PASS without Helder's actual result/log/screenshot.
2. One command at a time when diagnosis depends on the previous result.
3. After every result, analyze it, update this file, then issue the next command.
4. Prefer the lowest-bureaucracy workflow: Data Assets + reusable components + a few Widget Blueprints.
5. Never create fake .uasset, .umap, FBX, animation, VFX or audio files in Git.
6. If a step fails, repair it before continuing.
7. State-changing gameplay remains server-authoritative.
8. Visual assets are presentation and never gameplay authority.

## Current global state
- Repository/source roadmap: Phases 19–56 complete at source level.
- Unreal 5.8.1 local runtime: NOT YET VERIFIED.
- Current local execution phase: U0 — Environment & Project Health.
- Current step: U0.4 — Build.bat/UBT invocation verified.
- Last command/result: Build.bat -help successfully launched the bundled .NET 10 SDK and UnrealBuildTool.dll; help confirms GenerateProjectFiles, Build, Clean, QueryTargets and UnrealHeaderTool modes.
- Blockers: local Git working-tree status cannot be verified because this copy has no Git metadata. Project-file generation has not yet been performed and runtime/editor launch or build has not been verified. Do not initialize Git or alter/copy files yet.

# U0 — Environment & Project Health

## U0.1 — Confirm project + Unreal installation
Status: COMPLETE — INSTALLATION PATH IDENTIFIED

Result:
- Project found: D:\Projeto Age of Aether\Age-Of-Aether-main\AgeOfAether.uproject
- Epic Games Unreal Engine registry key exists with INSTALLDIR = C:\Program Files\Epic Games\
- Epic Launcher manifest identifies AppName = UE_5.8.
- InstallLocation = D:\Unreal\UE_5.8.

## U0.2 — Verify exact engine version
Status: COMPLETE — PROJECT ASSOCIATION AND INSTALLED EXECUTABLE CONFIRMED

Result:
- Project EngineAssociation = 5.8.
- Installed engine manifest AppName = UE_5.8.
- Installed engine version = 5.8.2-56702186+++UE5+Release-5.8-Windows.
- Installed engine path = D:\Unreal\UE_5.8.
- UnrealEditor.exe exists at D:\Unreal\UE_5.8\Engine\Binaries\Win64\UnrealEditor.exe.

Analysis:
- The project is associated with the UE 5.8 release line and the installed editor belongs to that release line.
- No project association change is required.
- U0.1 and U0.2 installation/version/path checks are complete.
- The runtime/editor itself is still unverified because we have not launched it yet.

## U0.3 — Backup/working-tree safety
Status: BLOCKED — LOCAL COPY HAS NO GIT METADATA

Result:
- Previous `git status --short --branch` failed with: `fatal: not a git repository (or any of the parent directories): .git`.
- Recursive search for `.git` under `D:\Projeto Age of Aether` returned no results.
- Top-level project listing confirms the expected source/project structure.

Analysis:
- The local folder contains a complete-looking Unreal/source tree, but it is not a Git working tree.
- The absence of .git is consistent with a downloaded/copied repository snapshot, but its origin is not yet proven.
- We have enough evidence to know the project files are present, but not enough to claim local version-control safety.
- Do not initialize a new repository, delete files, or overwrite the local project.
- U0.3 remains incomplete.

## U0.4 — Generated project files / build metadata
Status: IN PROGRESS — NO EXPECTED ROOT-LEVEL GENERATED FILES FOUND

Result:
- Checked the project root for Binaries, Intermediate, Saved, DerivedDataCache, AgeOfAether.sln, Makefile and CMakeLists.txt.
- The command returned no output.

Analysis:
- None of the tested generated/build artifacts exists at the project root under those exact names.
- This is not an error by itself; a clean source snapshot can legitimately lack generated Unreal/IDE metadata.
- No files were modified.
- Next: check the local source tree for generated Visual Studio/build metadata elsewhere, still without changing files.

## U0.5 — First UHT/UBT build
Status: PENDING

# U1 — Editor Boot & Base Map
Status: PENDING
- Open project in Unreal 5.8.1.
- Confirm no startup crash.
- Confirm no new critical errors.
- Open/create development map.
- Confirm PlayerStart and base character.

# U2 — Visual Foundation + Real FBX
Status: PENDING
- Import one real character FBX.
- Validate skeleton, scale, materials and collision.
- Create one reusable Visual Profile Data Asset.
- Apply to AAetherCharacter.
- Verify camera/movement.

# U3 — Animation
Status: PENDING
- Create one reusable Anim Blueprint based on UAetherBaseAnimInstance.
- Configure Idle/Walk/Run/Jump/Fall.
- Import one attack montage.
- Verify attack presentation.

# U4 — Skills / Buffs / Debuffs / VFX
Status: PENDING
- Configure aether.skill.training_strike.
- Import one simple VFX/SFX.
- Test cast, range and cooldown.
- Verify server authority with 2 clients.

# U5 — Monsters / NPCs / Bosses
Status: PENDING
- Create one UAetherCreatureCatalog.
- Add one Monster, one NPC and one Boss.
- Reuse AAetherCreatureActor.
- Import only one real creature FBX initially.
- Spawn and validate MaxAlive.

# U6 — World / Maps / Streaming
Status: PENDING
- Create first real development map.
- Configure world/streaming using existing data contracts.
- Validate player spawn and transitions.

# U7 — Inventory / Loot / Equipment
Status: PENDING
- Create required Data Assets.
- Create minimum inventory/equipment UI.
- Equip one real item visual.
- Validate server-authoritative mutations.

# U8 — Crafting / Economy / Shops
Status: PENDING
- Create one shop.
- Create one recipe.
- Create one forge/crafting station.
- Validate buy/sell/craft.

# U9 — MMORPG UI/UX
Status: PENDING
Only four initial Widget Blueprints:
- WBP_HUD
- WBP_Inventory
- WBP_Character
- WBP_Skills

# U10 — Audio
Status: PENDING
- Import minimum music/ambience/UI/combat SFX.
- Create one Audio Catalog Data Asset.
- Verify client playback and Dedicated Server guard.

# U11 — Multiplayer / Persistence / Dedicated Server
Status: PENDING
- 2-client PIE.
- Dedicated Server + clients.
- Disconnect/reconnect.
- Save/load character state.
- Inventory/economy/quest persistence.

# U12 — Performance / Streaming / Scale
Status: PENDING
- Run performance budget checks.
- Profile first playable map.
- Validate streaming.

# U13 — Complete Content Package
Status: PENDING
- Assemble first playable content package Data Asset.
- Validate required AssetIDs.

# U14 — Alpha Gate
Status: PENDING
- Build, Content, Security, Persistence, Networking, Dedicated Server, Multiplayer, Performance, UI, Audio.
- Any unverified check remains NOT PASSED.

# U15 — Beta / RC / Release
Status: PENDING
- Fix runtime defects.
- Repeat regression.
- Package client/server.
- Progress Beta → RC → Release only after gates pass.

# Command/result journal

## Entry 001 — U0.1
Status: PARTIAL / BLOCKED ON ENGINE PATH
Command: PowerShell project + UE_5.8 root scan.
Result:
- Project: D:\Projeto Age of Aether\Age-Of-Aether-main\AgeOfAether.uproject
- No UE_5.8 directory found in the four tested roots.
Analysis: Project is confirmed, but the Unreal installation path remained unresolved.

## Entry 002 — U0.1
Status: PARTIAL / BLOCKED ON ENGINE PATH
Command: PowerShell PATH lookup for UnrealEditor.exe.
Result: No output.
Analysis: UnrealEditor.exe is not exposed through PATH. This does not prove Unreal is absent.

## Entry 003 — U0.1
Status: PARTIAL — INSTALL ROOT CONFIRMED
Command: PowerShell registry lookup for the Epic Games Unreal Engine installation root.
Result:
- INSTALLDIR : C:\Program Files\Epic Games\
Analysis: The Windows registry confirms the Epic Games Unreal Engine installation root, but not the engine subdirectory.

## Entry 004 — U0.1
Status: PARTIAL — ENGINE LOCATION NOT YET CONFIRMED
Command: Get-ChildItem on the confirmed Epic Games root.
Result:
- DirectXRedist
- GameInputRedist
- Launcher
Analysis: No UE engine folder exists at the top level of the registered Epic root.

## Entry 005 — U0.1 / U0.2
Status: COMPLETE — PATH AND VERSION CONFIRMED
Command: Epic Games Launcher manifest lookup.
Result:
- AppName: UE_5.8
- DisplayName: Unreal Engine
- InstallLocation: D:\Unreal\UE_5.8
- AppVersion: 5.8.2-56702186+++UE5+Release-5.8-Windows
Analysis: Unreal Engine installation is confirmed at D:\Unreal\UE_5.8 and the installed version is 5.8.2.

## Entry 006 — U0.2
Status: COMPLETE — PROJECT ASSOCIATION CONFIRMED
Command: Read AgeOfAether.uproject EngineAssociation.
Result:
- EngineAssociation: 5.8
Analysis: The project is associated with the UE 5.8 release line, matching the installed UE_5.8 engine. No project association change is required.

## Entry 007 — U0.2
Status: COMPLETE — EXECUTABLE CONFIRMED
Command: Test-Path for UnrealEditor.exe at the confirmed engine path.
Result:
- True
Analysis: The Unreal Editor executable exists at D:\Unreal\UE_5.8\Engine\Binaries\Win64\UnrealEditor.exe. Installation/path verification is complete. Runtime/editor launch remains unverified.
Next: Perform a safe working-tree/status check before generating files or launching/building.

## Entry 008 — U0.3
Status: BLOCKED — LOCAL FOLDER IS NOT A GIT WORKING TREE
Command: `git status --short --branch`
Result:
- `fatal: not a git repository (or any of the parent directories): .git`
Analysis:
- The current local project folder is not recognized as a Git working tree.
- This may be a ZIP/export or copied repository, but that is not yet verified.
- No repository initialization or file movement should be performed yet.
Next: Locate the nearest existing `.git` metadata without modifying anything.

## Entry 009 — U0.3
Status: BLOCKED — NO LOCAL GIT METADATA FOUND
Command: Recursive `.git` directory search under `D:\Projeto Age of Aether`.
Result:
- No output; no `.git` directory was found.
Analysis:
- The local project tree has no Git metadata under its parent directory.
- We cannot use local Git status as a safety check for this copy.
- No destructive or repository-initializing action was performed.

## Entry 010 — U0.3
Status: BLOCKED — PROJECT TREE PRESENT, GIT METADATA ABSENT
Command: Top-level local project structure listing.
Result:
- .github
- Config
- Content
- Docs
- PROJECT_MEMORY
- Source
- .gitattributes
- .gitignore
- AgeOfAether.uproject
- ROADMAP.md
- ROADMAP_CONTENT_AND_CLIENT.md
- ROADMAP_VISUAL_AND_PLAYABLE.md
Analysis:
- The expected Unreal/source directories and repository documentation are present.
- The local copy is structurally consistent with the project repository, but it remains a non-Git working tree.
- No files were changed by this diagnostic.
Next: Inspect the .uproject file for its declared modules/plugins without modifying it.

## Entry 011 — U0.3
Status: DIAGNOSTIC PASS — PROJECT DECLARATION IS PARSEABLE

Command:
`Get-Content '.\AgeOfAether.uproject' -Raw | ConvertFrom-Json | Select-Object EngineAssociation, Modules, Plugins`

Result:
- EngineAssociation: 5.8
- Modules: one Runtime module named AgeOfAether, LoadingPhase=Default
- Plugins: EnhancedInput enabled

Analysis:
- The .uproject file parses successfully as JSON through PowerShell.
- Its declared engine association matches the previously verified UE 5.8 installation.
- The project declares the expected AgeOfAether Runtime module and has EnhancedInput enabled.
- This is a configuration-structure check only; it does not prove UHT/UBT compilation or editor startup.
- U0.3 remains incomplete because Git safety is unresolved.
Next: Verify whether the local source tree contains the generated project/build metadata that would indicate whether this copy has already been prepared locally, without modifying anything.

## Entry 012 — U0.4
Status: DIAGNOSTIC PASS — NO ROOT-LEVEL GENERATED METADATA FOUND

Command:
Get-ChildItem on the project root filtered for Binaries, Intermediate, Saved, DerivedDataCache, AgeOfAether.sln, Makefile and CMakeLists.txt.

Result:
- No output.

Analysis:
- None of the tested generated/build artifacts exists at the project root under those exact names.
- This is not an error by itself; a clean source snapshot can legitimately lack generated Unreal/IDE metadata.
- No files were modified.
- Next: check for generated Visual Studio/build metadata elsewhere in the local project tree, still without changing files.

## Entry 013 — U0.4
Status: DIAGNOSTIC PASS — NO VISUAL STUDIO PROJECT METADATA FOUND

Command:
Recursive search under the local project tree for AgeOfAether.sln, AgeOfAether.vcxproj, UE5.sln, and all .sln/.vcxproj/.vcxproj.filters files.

Result:
- No output.

Analysis:
- No generated Visual Studio solution/project metadata exists anywhere under the local project tree from this search.
- Together with Entry 012, this confirms the local copy is currently a source-only/clean generated-state snapshot for the tested Unreal build metadata.
- This is not a failure. Unreal can generate the required metadata when requested.
- No files were modified by this diagnostic.
- U0.4 is complete.
Next: perform the first controlled Unreal project-file generation using the installed UE 5.8 tools, then inspect the result before compiling.

## Entry 014 — U0.4
Status: BLOCKED — EXPECTED PROJECT-FILE GENERATOR EXECUTABLE NOT FOUND

Command:
`& 'D:\\Unreal\\UE_5.8\\Engine\\Binaries\\Win64\\UnrealVersionSelector.exe' -projectfiles 'D:\\Projeto Age of Aether\\Age-Of-Aether-main\\AgeOfAether.uproject'`

Result:
- PowerShell returned `CommandNotFoundException` stating that `D:\\Unreal\\UE_5.8\\Engine\\Binaries\\Win64\\UnrealVersionSelector.exe` is not recognized.
- No project files were generated by this command.

Analysis:
- The previously confirmed UnrealEditor.exe path is valid, but this separate UnrealVersionSelector executable is not present at the assumed location.
- This is a tooling-path issue, not yet evidence of a project or source-code failure.
- No project files were modified by the failed invocation.
- U0.4 generation remains incomplete.
Next: locate the actual project-file generation tooling inside the confirmed UE 5.8 installation, without modifying the project.

## Entry 015 — U0.4
Status: BLOCKED — NO EXPECTED PROJECT-FILE GENERATOR FOUND UNDER ENGINE

Command:
Recursive search inside `D:\\Unreal\\UE_5.8\\Engine` for `UnrealVersionSelector.exe`, `GenerateProjectFiles.bat`, and `GenerateProjectFiles.command`.

Result:
- No output.

Analysis:
- None of the expected project-file generator names was found under the Engine directory.
- This does not indicate that the Unreal installation is broken; the installed distribution may expose project generation through another tool/workflow.
- No files were modified.
- U0.4 generation remains blocked until the correct installed tooling is identified.
Next: inspect the confirmed UE 5.8 installation's Win64 binaries for available Unreal build/tool executables, without changing the project.

## Entry 016 — U0.4
Status: DIAGNOSTIC PASS — UE WIN64 TOOLSET INVENTORIED

Command:
List UE 5.8 `Engine\\Binaries\\Win64` executables whose names begin with Unreal, Build, UBT or UHT.

Result:
- Found `UnrealEditor.exe`, `UnrealEditor-Cmd.exe`, `UnrealGame.exe`, DebugGame variants, UnrealInsights, UnrealPak and other UE runtime/tools.
- No `UnrealBuildTool.exe` or `UnrealHeaderTool.exe` appeared in this Win64 directory under the tested name filter.

Analysis:
- The confirmed UE installation contains the Unreal Editor and command-line editor executables required for runtime diagnostics.
- UBT/UHT are not exposed as Win64 root executables under those names, so the next step should locate the build tools by searching their actual known filenames/paths rather than assuming they are Win64 binaries.
- No project files were modified.
- U0.4 remains focused on identifying the correct generation/build entry point before U0.5.
Next: locate `UnrealBuildTool.exe` and `UnrealHeaderTool.exe` anywhere under the confirmed UE installation, without changing files.

## Entry 017 — U0.4
Status: DIAGNOSTIC PASS — STANDALONE UBT/UHT EXECUTABLES NOT PRESENT

Command:
Recursive search of `D:\\Unreal\\UE_5.8` for `UnrealBuildTool.exe` and `UnrealHeaderTool.exe`.

Result:
- No output.

Analysis:
- No standalone `UnrealBuildTool.exe` or `UnrealHeaderTool.exe` was found anywhere in the confirmed UE 5.8 installation.
- The installation still contains `UnrealEditor.exe` and `UnrealEditor-Cmd.exe`, so this result does not imply the editor/runtime installation is unusable.
- The expected source-build/project-generation toolchain is not exposed under the usual standalone filenames in this installed distribution.
- No project files were modified.
Next: inspect the UE 5.8 installation for the AutomationTool/Build scripts that normally provide the build/project-generation entry points.

## Entry 018 — U0.4
Status: DIAGNOSTIC PASS — UE BUILD ENTRY POINT IDENTIFIED

Command:
Search `D:\\Unreal\\UE_5.8\\Engine\\Build` for build/automation scripts.

Result:
- `D:\\Unreal\\UE_5.8\\Engine\\Build\\BatchFiles\\Build.bat`
- `D:\\Unreal\\UE_5.8\\Engine\\Build\\BatchFiles\\RunUAT.bat`

Analysis:
- The installed UE 5.8 distribution provides the standard Build.bat and RunUAT.bat entry points even though standalone UBT/UHT executables are not present.
- We now have a confirmed build entry point for the next controlled phase.
- No project files were modified by this diagnostic.
Next: inspect Build.bat help/usage only, without invoking a project build yet, to confirm the accepted target/platform/configuration syntax in this installed engine.

## Entry 019 — U0.4
Status: DIAGNOSTIC PASS — UNREALBUILDTOOL ENTRY VERIFIED

Command:
`Build.bat -help`

Result:
- Bundled DotNet SDK 10.0 win-x64 launched successfully.
- UnrealBuildTool was invoked through `Engine\\Binaries\\DotNET\\UnrealBuildTool\\UnrealBuildTool.dll`.
- Help confirms `-ProjectFiles` / `-Mode=GenerateProjectFiles`, `-Mode=Build`, `-Clean`, `QueryTargets`, and `UnrealHeaderTool` modes.
- A UBT trace was written successfully.

Analysis:
- The previous absence of standalone UBT/UHT executables was expected for this installation layout; the tools are provided through the .NET DLL and Build.bat wrapper.
- The project-file generation path is now confirmed.
- No project source files were intentionally modified by the help invocation.
- U0.4 generation is ready to proceed.
Next: invoke Build.bat with -ProjectFiles against the .uproject, then inspect the generated metadata before any compile.

## Entry 020 — U0.4
Status: GENERATION SUCCEEDED WITH TARGET-CONFIGURATION WARNING

Command:
`Build.bat -ProjectFiles D:\\Projeto Age of Aether\\Age-Of-Aether-main\\AgeOfAether.uproject`

Result:
- UnrealBuildTool successfully discovered modules, targets and source code.
- Visual Studio solution/project files were generated: `AgeOfAether.sln`, `AgeOfAether.slnx`, plus automation solution files.
- Rules assemblies compiled successfully and QueryTargets data was generated.
- Overall UBT result: `Succeeded` in 16.56s.
- UBT reported that the installed bundled runtime is .NET 10.0.7 and Visual Studio 2022 does not support .NET 10 C# projects; it generated the Automation solution for Visual Studio 2026. This did not prevent project-file generation.
- UBT reported several engine-distribution Program targets unsupported; these are engine-side target generation notices and did not fail generation.
- Important project-specific warning: `AgeOfAetherEditor` modifies warning-level properties while sharing build products with `UnrealEditor`; UBT says this is not allowed unless the target uses a unique build environment or the override is explicitly enabled.
- UBT wrote a partial `Binaries\\Win64\\AgeOfAetherEditor.target` receipt, but the final generation result was still `Succeeded`.

Analysis:
- Project-file generation itself is confirmed successful.
- The `AgeOfAetherEditor` build-environment conflict is a real issue to investigate before treating an editor build as healthy; it must not be ignored.
- No source correction has been made yet because the next action should first inspect the generated files and target definition to identify exactly where the conflicting settings originate.
Next: inspect the generated solution/metadata and the AgeOfAether target files without compiling.

## Entry 021 — U0.4
Status: INSPECTION RESULT — TARGET FILES LOCATED

Command:
`Get-ChildItem ... | Where-Object { $_.Name -match 'AgeOfAether.*Target|AgeOfAether.*Build|AgeOfAether\\.slnx|AgeOfAether\\.sln' }`

Result:
- Generated solutions exist: `AgeOfAether.sln`, `AgeOfAether.slnx`, and Automation variants.
- Generated partial receipt exists: `Binaries\\Win64\\AgeOfAetherEditor.target`.
- Project source target files exist: `Source\\AgeOfAether.Target.cs`, `Source\\AgeOfAetherEditor.Target.cs`, `Source\\AgeOfAetherServer.Target.cs`.
- Module rules file exists: `Source\\AgeOfAether.Build.cs`.

Analysis:
- The exact editor target source file is now identified and should be inspected before any modification or compilation.
- No project files were changed by this inspection command.
Next: read `Source\\AgeOfAetherEditor.Target.cs` exactly as currently present in the local project.

## Entry 022 — U0.4
Status: INSPECTION RESULT — EDITOR TARGET SOURCE READ

Command:
`Get-Content Source\\AgeOfAetherEditor.Target.cs`

Result:
- Target type is `Editor`.
- `DefaultBuildSettings = BuildSettingsVersion.V5`.
- `IncludeOrderVersion = EngineIncludeOrderVersion.Latest`.
- Module `AgeOfAether` is added.
- The file does NOT explicitly set the warning-level properties named by UBT.
- The file does NOT set `BuildEnvironment = TargetBuildEnvironment.Unique` or `bOverrideBuildEnvironment = true`.

Analysis:
- The UBT conflict is not directly configured in this target file; it is likely inherited from engine/default target settings versus the generated/shared UnrealEditor build environment.
- Do not blindly add a unique build environment yet; first inspect the other target/module rules to determine whether the project is overriding warning settings elsewhere.
Next: inspect `Source\\AgeOfAether.Target.cs`, `Source\\AgeOfAetherServer.Target.cs`, and `Source\\AgeOfAether.Build.cs` for warning/build-environment overrides.

## Entry 023 — U0.4
Status: INSPECTION RESULT — GAME/SERVER/MODULE RULES READ

Result:
- `AgeOfAether.Target.cs`, `AgeOfAetherServer.Target.cs`, and `AgeOfAether.Build.cs` contain no explicit warning-level overrides.
- All three targets use `BuildSettingsVersion.V5` and `EngineIncludeOrderVersion.Latest`.
- Module dependencies are `Core`, `CoreUObject`, `Engine`, `InputCore`, and `EnhancedInput`.
- No target sets a unique build environment or override flag.

Analysis:
- The project source shown does not contain the warning-level assignments reported by UBT.
- The conflict therefore appears related to the UE 5.8 shared `UnrealEditor` build environment/defaults rather than an obvious project rule override.
- Because UBT explicitly offered `BuildEnvironment = TargetBuildEnvironment.Unique` as the targeted remedy, the safest next step is to inspect the exact generated `.target` receipt and UBT log for the editor target before changing source.
Next: inspect the generated `Binaries\\Win64\\AgeOfAetherEditor.target` receipt and relevant UBT log lines for the four warning-level properties.

## Entry 024 — U0.4
Status: DIAGNOSTIC CONFIRMED — SHARED BUILD ENVIRONMENT CONFLICT

Evidence:
- Generated `AgeOfAetherEditor.target` receipt reports `TargetBuildEnvironment: Shared`.
- UBT log line 105 explicitly reports four property differences: `UndefinedIdentifierWarningLevel: Off != Error`, `UnreachableCodeWarningLevel: Off != Error`, `ReturnTypeWarningLevel: Off != Error`, `DanglingWarningLevel: Off != Error`.
- UBT explicitly recommends either removing the modified setting, setting `BuildEnvironment = TargetBuildEnvironment.Unique;`, or forcing the override.
- The project target/module source inspected so far does not explicitly set these properties.

Analysis:
- The issue is confirmed as a shared-build-environment compatibility conflict, not a missing target file or failed project-file generation.
- The generated receipt is partial and must not be treated as proof of a successful editor build.
- Before modifying source, inspect whether these properties are being supplied through project config (`.ini`) or another rules/configuration file. This avoids adding a workaround when the underlying setting can be removed.
Next: search project configuration/source text for the four exact property names and related warning/build-environment settings.
