#!/usr/bin/env python3
"""Validate source-art provenance and prepared PNG alpha in the asset manifest."""
from __future__ import annotations

import json
from pathlib import Path

from PIL import Image

ROOT = Path(__file__).resolve().parents[2]
MANIFEST = ROOT / "Content/Aether/Art/2D_ASSET_MANIFEST.json"


def validate() -> int:
    data = json.loads(MANIFEST.read_text(encoding="utf-8"))
    assets = data.get("assets")
    if not isinstance(assets, list):
        raise ValueError("Manifest must contain an assets array.")

    ids: set[str] = set()
    checked = 0
    for asset in assets:
        asset_id = asset.get("asset_id", "").strip()
        if not asset_id:
            raise ValueError("Every manifest record must have a stable asset_id.")
        if asset_id in ids:
            raise ValueError(f"Duplicate asset_id: {asset_id}")
        ids.add(asset_id)

        if not asset.get("source_status", "").startswith("GENERATED_SOURCE_READY"):
            continue

        for key in ("source_path", "prepared_import_path", "provenance_record", "preparation_tool"):
            if not asset.get(key):
                raise ValueError(f"{asset_id}: generated asset is missing {key}.")

        if asset["source_path"].startswith("Content/"):
            raise ValueError(f"{asset_id}: raw source must remain outside Unreal's Content directory.")
        if not asset["prepared_import_path"].startswith("Content/Aether/Art/ImportReady/"):
            raise ValueError(f"{asset_id}: prepared source must be staged under Content/Aether/Art/ImportReady/.")
        if asset.get("runtime_status") != "NOT_IMPORTED_OR_VALIDATED_IN_UNREAL":
            raise ValueError(f"{asset_id}: runtime status must remain explicitly unverified until Unreal acceptance.")

        source = ROOT / asset["source_path"]
        prepared = ROOT / asset["prepared_import_path"]
        provenance = ROOT / asset["provenance_record"]
        tool = ROOT / asset["preparation_tool"]
        for path in (source, prepared, provenance, tool):
            if not path.is_file():
                raise ValueError(f"{asset_id}: referenced file does not exist: {path.relative_to(ROOT)}")
        if source.resolve() == prepared.resolve():
            raise ValueError(f"{asset_id}: raw and prepared sources must remain separate.")

        with Image.open(source) as original, Image.open(prepared) as image:
            if original.format != "PNG" or image.format != "PNG":
                raise ValueError(f"{asset_id}: source and prepared images must be PNG.")
            if image.mode != "RGBA":
                raise ValueError(f"{asset_id}: prepared image must be RGBA, got {image.mode}.")
            alpha_min, alpha_max = image.getchannel("A").getextrema()
            if alpha_min != 0 or alpha_max != 255:
                raise ValueError(f"{asset_id}: prepared image must contain transparent and opaque pixels.")
            if image.width < 32 or image.height < 32:
                raise ValueError(f"{asset_id}: prepared image is unexpectedly small.")

        metadata_path_value = asset.get("animation_metadata_path")
        preview_path_value = asset.get("preview_asset_path")
        if metadata_path_value or preview_path_value:
            if not metadata_path_value or not preview_path_value:
                raise ValueError(f"{asset_id}: sprite sheet needs both metadata and preview paths.")
            metadata_path = ROOT / metadata_path_value
            preview_path = ROOT / preview_path_value
            if not metadata_path.is_file() or not preview_path.is_file():
                raise ValueError(f"{asset_id}: sprite-sheet metadata or browser atlas is missing.")
            metadata_key = Path(preview_path_value).stem
            all_metadata = json.loads(metadata_path.read_text(encoding="utf-8"))
            if metadata_key not in all_metadata:
                raise ValueError(f"{asset_id}: sprite-sheet metadata has no entry for {metadata_key}.")
            sheet_metadata = all_metadata[metadata_key]
            cell = sheet_metadata.get("cell", {})
            expected_layout = asset.get("sheet_layout", {})
            if expected_layout.get("cell") != cell:
                raise ValueError(f"{asset_id}: manifest frame size does not match the atlas metadata.")
            with Image.open(preview_path) as preview:
                expected_size = (cell.get("width", 0) * sheet_metadata.get("columns", 0), cell.get("height", 0) * sheet_metadata.get("rows", 0))
                if preview.mode != "RGBA" or preview.size != expected_size:
                    raise ValueError(f"{asset_id}: browser atlas dimensions/mode do not match metadata.")
                if preview.getchannel("A").getextrema() != (0, 255):
                    raise ValueError(f"{asset_id}: browser atlas must preserve sprite transparency.")
            for animation_name, expected_row in expected_layout.get("rows", {}).items():
                animation = sheet_metadata.get("animations", {}).get(animation_name)
                if not animation or animation.get("row") != expected_row:
                    raise ValueError(f"{asset_id}: animation row mismatch for {animation_name}.")
        checked += 1

    if checked == 0:
        raise ValueError("No generated source assets were checked.")
    print(f"Validated {len(ids)} unique art IDs and {checked} generated RGBA source assets.")
    return checked


if __name__ == "__main__":
    validate()
