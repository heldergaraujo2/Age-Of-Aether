#!/usr/bin/env python3
"""Pack prepared character poses into deterministic, row-based sprite sheets.

Generated individual frames stay in ArtSource/Generated/. Prepared frames and
sheet atlases are staged under Content/Aether/Art/ImportReady/ for real Unreal
Paper2D import. The browser preview receives byte-identical local atlases.

Requires Pillow: python -m pip install Pillow
"""
from __future__ import annotations

import json
from pathlib import Path
from typing import Any

from PIL import Image, ImageOps

ROOT = Path(__file__).resolve().parents[2]
IMPORT_ROOT = ROOT / "Content/Aether/Art/ImportReady"
RAW_SHEET_ROOT = ROOT / "ArtSource/Generated/Animations/SpriteSheets"
PREVIEW_ROOT = ROOT / "Preview/assets/spritesheets"


def frame(path: str, *, scale: float = 1.0, offset_x: int = 0, offset_y: int = 0) -> dict[str, Any]:
    return {"path": path, "scale": scale, "offset_x": offset_x, "offset_y": offset_y}


SHEET_SPECS: dict[str, dict[str, Any]] = {
    "player-mage": {
        "filename": "PlayerMage_SpriteSheet.png",
        "preview_filename": "player-mage.png",
        "frame_size": (256, 320),
        "bottom_padding": 8,
        "source_frames": [
            "Content/Aether/Art/ImportReady/Characters/Mage/PlayerMage_Idle.png",
            "Content/Aether/Art/ImportReady/Animations/PlayerMage_Walk.png",
            "Content/Aether/Art/ImportReady/Animations/PlayerMage_Cast.png",
        ],
        "animations": [
            {
                "name": "idle", "fps": 4, "loop": True,
                "frames": [
                    frame("Content/Aether/Art/ImportReady/Characters/Mage/PlayerMage_Idle.png"),
                    frame("Content/Aether/Art/ImportReady/Characters/Mage/PlayerMage_Idle.png", scale=.985, offset_y=-1),
                    frame("Content/Aether/Art/ImportReady/Characters/Mage/PlayerMage_Idle.png", offset_y=-1),
                    frame("Content/Aether/Art/ImportReady/Characters/Mage/PlayerMage_Idle.png", scale=.985),
                ],
            },
            {
                "name": "walk", "fps": 9, "loop": True,
                "frames": [
                    frame("Content/Aether/Art/ImportReady/Characters/Mage/PlayerMage_Idle.png"),
                    frame("Content/Aether/Art/ImportReady/Animations/PlayerMage_Walk.png", offset_y=-1),
                    frame("Content/Aether/Art/ImportReady/Characters/Mage/PlayerMage_Idle.png", offset_y=-1),
                    frame("Content/Aether/Art/ImportReady/Animations/PlayerMage_Walk.png"),
                ],
            },
            {
                "name": "cast", "fps": 12, "loop": False,
                "frames": [
                    frame("Content/Aether/Art/ImportReady/Characters/Mage/PlayerMage_Idle.png"),
                    frame("Content/Aether/Art/ImportReady/Animations/PlayerMage_Cast.png", offset_y=-1),
                    frame("Content/Aether/Art/ImportReady/Animations/PlayerMage_Cast.png"),
                    frame("Content/Aether/Art/ImportReady/Characters/Mage/PlayerMage_Idle.png"),
                ],
            },
        ],
    },
    "elara": {
        "filename": "Elara_SpriteSheet.png",
        "preview_filename": "elara.png",
        "frame_size": (256, 320),
        "bottom_padding": 8,
        "source_frames": [
            "Content/Aether/Art/ImportReady/Animations/Elara_Idle.png",
            "Content/Aether/Art/ImportReady/Animations/Elara_Talk.png",
        ],
        "animations": [
            {
                "name": "idle", "fps": 3.5, "loop": True,
                "frames": [
                    frame("Content/Aether/Art/ImportReady/Animations/Elara_Idle.png"),
                    frame("Content/Aether/Art/ImportReady/Animations/Elara_Idle.png", scale=.985, offset_y=-1),
                    frame("Content/Aether/Art/ImportReady/Animations/Elara_Idle.png", offset_y=-1),
                    frame("Content/Aether/Art/ImportReady/Animations/Elara_Idle.png", scale=.985),
                ],
            },
            {
                "name": "talk", "fps": 5, "loop": False,
                "frames": [
                    frame("Content/Aether/Art/ImportReady/Animations/Elara_Idle.png"),
                    frame("Content/Aether/Art/ImportReady/Animations/Elara_Talk.png", offset_y=-1),
                    frame("Content/Aether/Art/ImportReady/Animations/Elara_Talk.png"),
                    frame("Content/Aether/Art/ImportReady/Animations/Elara_Idle.png"),
                ],
            },
        ],
    },
    "wild-hound": {
        "filename": "WildHound_SpriteSheet.png",
        "preview_filename": "wild-hound.png",
        "frame_size": (384, 320),
        "bottom_padding": 10,
        "source_frames": [
            "Content/Aether/Art/ImportReady/Creatures/WildHound/WildHound_Idle.png",
            "Content/Aether/Art/ImportReady/Animations/WildHound_Run.png",
            "Content/Aether/Art/ImportReady/Animations/WildHound_Attack.png",
        ],
        "animations": [
            {
                "name": "idle", "fps": 4, "loop": True,
                "frames": [
                    frame("Content/Aether/Art/ImportReady/Creatures/WildHound/WildHound_Idle.png"),
                    frame("Content/Aether/Art/ImportReady/Creatures/WildHound/WildHound_Idle.png", scale=.985, offset_y=-1),
                    frame("Content/Aether/Art/ImportReady/Creatures/WildHound/WildHound_Idle.png", offset_y=-1),
                    frame("Content/Aether/Art/ImportReady/Creatures/WildHound/WildHound_Idle.png", scale=.985),
                ],
            },
            {
                "name": "run", "fps": 10, "loop": True,
                "frames": [
                    frame("Content/Aether/Art/ImportReady/Creatures/WildHound/WildHound_Idle.png"),
                    frame("Content/Aether/Art/ImportReady/Animations/WildHound_Run.png", offset_y=-2),
                    frame("Content/Aether/Art/ImportReady/Creatures/WildHound/WildHound_Idle.png", offset_y=-1),
                    frame("Content/Aether/Art/ImportReady/Animations/WildHound_Run.png"),
                ],
            },
            {
                "name": "attack", "fps": 12, "loop": False,
                "frames": [
                    frame("Content/Aether/Art/ImportReady/Animations/WildHound_Run.png"),
                    frame("Content/Aether/Art/ImportReady/Animations/WildHound_Attack.png", offset_y=-2),
                    frame("Content/Aether/Art/ImportReady/Animations/WildHound_Attack.png"),
                    frame("Content/Aether/Art/ImportReady/Animations/WildHound_Run.png"),
                ],
            },
        ],
    },
    "aether-wisp": {
        "filename": "AetherWisp_SpriteSheet.png",
        "preview_filename": "aether-wisp.png",
        "frame_size": (384, 320),
        "bottom_padding": 14,
        "source_frames": [
            "Content/Aether/Art/ImportReady/Animations/AetherWisp_Idle.png",
            "Content/Aether/Art/ImportReady/Animations/AetherWisp_Glide.png",
            "Content/Aether/Art/ImportReady/Animations/AetherWisp_Pulse.png",
        ],
        "animations": [
            {
                "name": "idle", "fps": 4, "loop": True,
                "frames": [
                    frame("Content/Aether/Art/ImportReady/Animations/AetherWisp_Idle.png"),
                    frame("Content/Aether/Art/ImportReady/Animations/AetherWisp_Idle.png", scale=.985, offset_y=-2),
                    frame("Content/Aether/Art/ImportReady/Animations/AetherWisp_Idle.png", offset_y=-3),
                    frame("Content/Aether/Art/ImportReady/Animations/AetherWisp_Idle.png", scale=.985),
                ],
            },
            {
                "name": "glide", "fps": 9, "loop": True,
                "frames": [
                    frame("Content/Aether/Art/ImportReady/Animations/AetherWisp_Idle.png"),
                    frame("Content/Aether/Art/ImportReady/Animations/AetherWisp_Glide.png", offset_y=-2),
                    frame("Content/Aether/Art/ImportReady/Animations/AetherWisp_Idle.png", offset_y=-1),
                    frame("Content/Aether/Art/ImportReady/Animations/AetherWisp_Glide.png"),
                ],
            },
            {
                "name": "pulse", "fps": 12, "loop": False,
                "frames": [
                    frame("Content/Aether/Art/ImportReady/Animations/AetherWisp_Idle.png"),
                    frame("Content/Aether/Art/ImportReady/Animations/AetherWisp_Pulse.png", offset_y=-1),
                    frame("Content/Aether/Art/ImportReady/Animations/AetherWisp_Pulse.png"),
                    frame("Content/Aether/Art/ImportReady/Animations/AetherWisp_Idle.png"),
                ],
            },
        ],
    },
}


def _load_frame(source: Path, cell_size: tuple[int, int], bottom_padding: int, pose: dict[str, Any]) -> Image.Image:
    with Image.open(source) as loaded:
        sprite = loaded.convert("RGBA")
    alpha = sprite.getchannel("A")
    bounds = alpha.getbbox()
    if bounds is None:
        raise ValueError(f"Sprite frame is fully transparent: {source}")
    sprite = sprite.crop(bounds)
    if pose.get("flip_x"):
        sprite = ImageOps.mirror(sprite)

    cell_width, cell_height = cell_size
    maximum_width = cell_width - 24
    maximum_height = cell_height - bottom_padding - 12
    scale = min(maximum_width / sprite.width, maximum_height / sprite.height) * float(pose.get("scale", 1.0))
    output_size = (max(1, round(sprite.width * scale)), max(1, round(sprite.height * scale)))
    sprite = sprite.resize(output_size, Image.Resampling.LANCZOS)

    x = (cell_width - sprite.width) // 2 + int(pose.get("offset_x", 0))
    y = cell_height - bottom_padding - sprite.height + int(pose.get("offset_y", 0))
    if x < 0 or y < 0 or x + sprite.width > cell_width or y + sprite.height > cell_height:
        raise ValueError(f"Pose does not fit its {cell_width}x{cell_height} frame cell: {source}")

    cell = Image.new("RGBA", cell_size, (0, 0, 0, 0))
    cell.alpha_composite(sprite, (x, y))
    return cell


def pack_sheet(spec: dict[str, Any], root: Path = ROOT) -> tuple[Image.Image, dict[str, Any]]:
    cell_size = tuple(spec["frame_size"])
    animations = spec["animations"]
    columns = max(len(animation["frames"]) for animation in animations)
    rows = len(animations)
    sheet = Image.new("RGBA", (cell_size[0] * columns, cell_size[1] * rows), (0, 0, 0, 0))
    metadata_animations: dict[str, Any] = {}

    for row, animation in enumerate(animations):
        for column, pose in enumerate(animation["frames"]):
            source = root / pose["path"]
            if not source.is_file():
                raise FileNotFoundError(f"Missing prepared pose frame: {pose['path']}")
            cell = _load_frame(source, cell_size, spec["bottom_padding"], pose)
            sheet.alpha_composite(cell, (column * cell_size[0], row * cell_size[1]))
        metadata_animations[animation["name"]] = {
            "row": row,
            "frames": len(animation["frames"]),
            "fps": animation["fps"],
            "loop": animation["loop"],
        }

    metadata = {
        "schema": "age-of-aether.sprite-sheet.v1",
        "layout": "row-major; animation rows are zero-based; frames advance left-to-right",
        "cell": {"width": cell_size[0], "height": cell_size[1]},
        "columns": columns,
        "rows": rows,
        "animations": metadata_animations,
        "source_frames": spec["source_frames"],
    }
    return sheet, metadata


def build_all_sheets(root: Path = ROOT) -> dict[str, dict[str, Any]]:
    results: dict[str, dict[str, Any]] = {}
    preview_dir = root / "Preview/assets/spritesheets"
    raw_dir = root / "ArtSource/Generated/Animations/SpriteSheets"
    import_dir = root / "Content/Aether/Art/ImportReady/Animations/SpriteSheets"
    for output_dir in (preview_dir, raw_dir, import_dir):
        output_dir.mkdir(parents=True, exist_ok=True)

    for name, spec in SHEET_SPECS.items():
        sheet, metadata = pack_sheet(spec, root)
        raw_path = raw_dir / spec["filename"]
        import_path = import_dir / spec["filename"]
        preview_path = preview_dir / spec["preview_filename"]
        for path in (raw_path, import_path, preview_path):
            sheet.save(path, format="PNG", optimize=True, compress_level=9)
        results[name] = {
            "metadata": metadata,
            "source_path": raw_path.relative_to(root).as_posix(),
            "prepared_import_path": import_path.relative_to(root).as_posix(),
            "preview_path": preview_path.relative_to(root).as_posix(),
            "size": sheet.size,
        }

    manifest = {name: result["metadata"] for name, result in results.items()}
    encoded = json.dumps(manifest, indent=2, ensure_ascii=False) + "\n"
    (preview_dir / "manifest.json").write_text(encoded, encoding="utf-8")
    (import_dir / "manifest.json").write_text(encoded, encoding="utf-8")
    return results


def main() -> None:
    results = build_all_sheets()
    for name, result in results.items():
        width, height = result["size"]
        print(f"Built {name}: {width}x{height} -> {result['prepared_import_path']} and {result['preview_path']}")


if __name__ == "__main__":
    main()
