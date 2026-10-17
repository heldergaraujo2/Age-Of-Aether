from __future__ import annotations

import tempfile
import unittest
from pathlib import Path

from PIL import Image, ImageDraw

from Tools.Art.build_sprite_sheets import SHEET_SPECS, frame, pack_sheet


class SpriteSheetPackingTests(unittest.TestCase):
    def test_packs_transparent_aligned_animation_rows_and_metadata(self) -> None:
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary)
            red = Image.new("RGBA", (30, 52), (0, 0, 0, 0))
            ImageDraw.Draw(red).rectangle((5, 4, 24, 50), fill=(220, 75, 65, 255))
            red.save(root / "red.png")
            blue = Image.new("RGBA", (30, 52), (0, 0, 0, 0))
            ImageDraw.Draw(blue).rectangle((4, 3, 25, 49), fill=(65, 125, 220, 255))
            blue.save(root / "blue.png")

            spec = {
                "frame_size": (64, 80),
                "bottom_padding": 6,
                "source_frames": ["red.png", "blue.png"],
                "animations": [
                    {"name": "idle", "fps": 4, "loop": True, "frames": [frame("red.png"), frame("red.png", scale=.98)]},
                    {"name": "walk", "fps": 8, "loop": True, "frames": [frame("red.png"), frame("blue.png")]},
                ],
            }
            sheet, metadata = pack_sheet(spec, root)

            self.assertEqual(sheet.size, (128, 160))
            self.assertEqual(sheet.mode, "RGBA")
            self.assertEqual(sheet.getchannel("A").getextrema(), (0, 255))
            self.assertEqual(metadata["animations"]["idle"], {"row": 0, "frames": 2, "fps": 4, "loop": True})
            self.assertEqual(metadata["animations"]["walk"]["row"], 1)
            self.assertEqual(metadata["cell"], {"width": 64, "height": 80})

            idle_alpha = sheet.crop((0, 0, 64, 80)).getchannel("A").getbbox()
            walk_blue = sheet.crop((64, 80, 128, 160))
            self.assertIsNotNone(idle_alpha)
            self.assertGreater(walk_blue.getpixel((32, 60))[3], 0)
            self.assertEqual(walk_blue.getpixel((32, 60))[:3], (65, 125, 220))

    def test_aether_wisp_atlas_declares_all_preview_animation_rows(self) -> None:
        spec = SHEET_SPECS["aether-wisp"]
        self.assertEqual(spec["frame_size"], (384, 320))
        self.assertEqual([animation["name"] for animation in spec["animations"]], ["idle", "glide", "pulse"])
        self.assertTrue(all(len(animation["frames"]) == 4 for animation in spec["animations"]))
        self.assertEqual(spec["preview_filename"], "aether-wisp.png")

    def test_rejects_a_missing_prepared_frame(self) -> None:
        spec = {
            "frame_size": (64, 80),
            "bottom_padding": 6,
            "source_frames": ["missing.png"],
            "animations": [{"name": "idle", "fps": 4, "loop": True, "frames": [frame("missing.png")]}],
        }
        with tempfile.TemporaryDirectory() as temporary:
            with self.assertRaises(FileNotFoundError):
                pack_sheet(spec, Path(temporary))


if __name__ == "__main__":
    unittest.main()
