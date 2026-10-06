# CC0 Forest Assets — Unreal Integration

## Included source content

`ArtSource/Environment/CC0Forest/` contains two self-contained GLBs (about 1.6 MB total):

- `forest.glb`: 16 KayKit Forest Nature Pack meshes by Kay Lousberg.
- `nature.glb`: 82 Kenney Nature Kit meshes, including green jungle trees, pines, shrubs, grass and flowers; the bundled GLB has vertex colours.

The upstream art credits identify both packs as **CC0 1.0**. Provenance and links are in `ArtSource/Environment/CC0Forest/CREDITS.md`. Only the art GLBs are included; the MIT-licensed code from the upstream bundling repository is not.

These are stylized low-poly assets rather than photoreal foliage. They were selected because their license permits direct inclusion and redistribution in this repository.

## Import and use

1. Build the Unreal Editor target for UE 5.8 and open the project.
2. In Unreal Editor, run `Scripts/import_cc0_forest_assets.py` using **Tools → Execute Python Script**. UE's Interchange/glTF importer should be enabled (it is enabled by default in UE 5.8).
3. The script imports separate Static Mesh assets under `/Game/Aether/Environment/CC0Forest/KayKit` and `/Game/Aether/Environment/CC0Forest/Kenney`, names them deterministically, and saves them. It does not alter or save the open map. If import reports that meshes could not be matched, check that **Combine Static Meshes** is off in the Interchange asset pipeline.
4. The current development build is intentionally in **characters-only** mode while the environment is rebuilt. `AAetherDevelopmentWorldActor` hides map-authored Landscape/Foliage actors at runtime, skips all procedural terrain/vegetation/buildings, retains an invisible collision plane, and spawns the ambient Mage walkers. It does not currently instantiate the CC0 foliage meshes.

The imported CC0 meshes remain available for a later environment pass. Their source GLB materials and vertex colours are not manually reassigned by the importer. The local map and external actor packages are not deleted or rewritten by this runtime mode.
