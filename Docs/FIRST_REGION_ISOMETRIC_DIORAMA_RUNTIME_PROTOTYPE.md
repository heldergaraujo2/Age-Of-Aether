# First Region — Runtime Isometric Diorama Prototype

## Target

The owner’s first reference image defines the art direction: a colorful, legible 3D isometric storybook realm with a blue-roofed castle, river and bridge, village, fields, forest and rich terrain. The second image is the earlier poor visual state, not the target. The scene is a navigable world-space composition, not a painted map image.

## Runtime blockout

`AAetherDevelopmentWorldActor` assembles the first region from Unreal BasicShapes at runtime. The composition contains a winding river and roads, stone bridge, round village plaza, cottages, market/fountain/lanterns, three crop fields, a multi-tower castle and courtyard, perimeter woodland, a windmill, a watchtower, ruins and rocks. Repeated elements use instanced static-mesh components.

The existing character, networking and gameplay systems remain in place. Art changes are presentation-only and do not replace movement, collision, combat or server authority. Paper2D profiles remain available as optional authored presentation paths.

## Visual-fidelity pass

See [`FIRST_REGION_VISUAL_FIDELITY_PASS.md`](FIRST_REGION_VISUAL_FIDELITY_PASS.md) for the active work and local UE steps. The pass adds clustered tree foliage and branches, procedural grass/wildflowers, roof-tile relief, and an Editor import workflow for the supplied mage FBX/base-color assets, including simple movement-driven skeletal animation. The scene remains a procedural blockout—not final production art.

## Asset boundary

The map is generated in C++ when the development world starts; no `.umap` or fabricated `.uasset` is part of this source change. Raw FBX/JPG sources stay in `Animaçoes + texturas/`. Unreal must import/convert those sources into local `.uasset` files through the documented Editor Python script before the skeletal mage path can replace its proxy.

## Validation status

The owner confirmed the UE 5.8 build succeeded at commit `05fe52a` after the earlier Unity Build fixes. **This new visual-fidelity change has not been compiled with UBT or reviewed in PIE.** The agent sandbox has no Unreal Engine. Local acceptance is still required for UE 5.8 compile, FBX import/material assignment, camera framing, visible tile/foliage placement, character scale/animation, collision, replication and frame time. Do not mark the scene or its art as production-ready until those checks pass.
