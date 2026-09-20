# Phase 47 — Minimal Unreal Setup

## First vertical slice
Create Starter and Forest maps and one UAetherWorldMapCatalog Data Asset.

Add two map rows:
- starter → /Game/Aether/Maps/Starter
- forest → /Game/Aether/Maps/Forest

Add one link:
- starter_forest

No C++ is needed for the second map.

## Visual/streaming rule
Code owns identity, validation and authority contracts. Unreal assets own geometry, lighting, World Partition/level configuration and visual presentation.

For the first test, do not build a large world. Two small maps prove the pipeline.

## Important
The repository intentionally does not fabricate .umap files. Actual map creation and runtime streaming must be done in Unreal Editor 5.8.1 and then registered through the catalog.