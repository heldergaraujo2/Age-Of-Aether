# Fab Library environment integration

This change connects selected sources in `FabLibrary/` to the first-region runtime diorama. The source FBX/GLB files remain untouched; Unreal-generated `.uasset` files are local project data and are not committed.

## Import into Unreal Editor

1. Pull the branch, open `AgeOfAether.uproject`, and allow Unreal to enable/rebuild the `PythonScriptPlugin` and `EditorScriptingUtilities` plugins (restart the Editor if prompted).
2. In Unreal Editor, use **Tools → Execute Python Script** and select `Scripts/import_fab_library_assets.py` from the project directory. Alternatively, run it from the Output Log Python console with the full path to the script.
3. Wait for the import/save operations to finish. The Output Log prints the normalized asset paths and any importer errors. The FBX texture files are staged under `Saved/FabImportStaging/`; the original `FabLibrary/` sources are not rewritten.
4. Compile the project and run the development map in PIE. `AAetherDevelopmentWorldActor` loads the imported assets at runtime and keeps its existing built-in meshes as fallbacks if an import is missing.

### Expected runtime paths

- Trees/bushes: `/Game/Aether/Environment/Fab/TreesBush/SM_Fab_*`
- Unicorn horse: `/Game/Aether/Characters/FabHorse/SK_Fab_UnicornHorse`
- Horse Idle: `/Game/Aether/Characters/FabHorse/Animations/A_Fab_UnicornHorse_Idle`
- Mansion (opt-in only): `/Game/Aether/Environment/Fab/Mansion/SM_Fab_HauntedMansion`

## What the runtime pass adds

- Replaces the diorama's procedural tree silhouettes with authored low-poly FBX trees when available, then adds a deterministic outer tree belt and clustered city/forest shrubs. Instanced components keep the added foliage relatively inexpensive; grass tufts already in the diorama remain in place.
- Adds ten Mage walker placeholders using the existing skeletal mesh and `A_Walk_Anim` (or `A_Walk`). They move back and forth along the village-to-castle road; these are visual components with no collision, interaction, replication, or gameplay AI. The user confirmed this placeholder composition.
- Provides an optional farmyard placement for the Fab unicorn and plays its Idle sequence. It remains off for the selected ten-human composition; the source pack has no walking animation, so the horse is not sent walking.

`bEnableMageWalkerPlaceholder` is **on by default**; `bPlaceIdleFabHorseAtStable` remains off. Both can be changed under **Age of Aether → Ambient Characters**.
- Does not turn `controllable_Rain` into rain. Its GLB is geometry, not a Niagara particle system; a real weather effect still needs a separate Niagara implementation.

The ambient walkers also require the Mage mesh and Walk sequence already imported by `Scripts/import_mage_assets.py`. If those assets are absent, the world logs a warning and skips the walkers rather than substituting broken meshes.

## Mansion metadata

The mansion's Fab metadata sets both `isAiGenerated=true` and `isAiForbidden=true`. The importer therefore skips it by default. Check the Fab listing's current terms/permissions before using it in a distributed game; only after that review, explicitly set `IMPORT_AI_RESTRICTED_MANSION = True` near the top of the import script and rerun it. The runtime has an optional placement hook, but no mansion asset is required for the vegetation or NPC pass.

## Validation status

Python syntax and repository whitespace checks can be run in a normal checkout. Actual FBX/GLB importing, Unreal C++ compilation, and PIE visual validation must be done in the Unreal Editor on the target PC; those cannot be verified from this source-only sandbox.
