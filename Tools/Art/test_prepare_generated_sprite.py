from __future__ import annotations

import unittest

from PIL import Image

from Tools.Art.prepare_generated_sprite import matte_border_background


class MatteGeneratedSpriteTests(unittest.TestCase):
    def setUp(self) -> None:
        # A synthetic baked checkerboard, a saturated silhouette, and a neutral
        # detail enclosed by that silhouette.
        self.image = Image.new("RGB", (32, 32))
        pixels = self.image.load()
        for y in range(32):
            for x in range(32):
                pixels[x, y] = (188, 188, 188) if (x // 4 + y // 4) % 2 else (255, 255, 255)
        for y in range(8, 25):
            for x in range(8, 25):
                pixels[x, y] = (30, 70, 130)
        for y in range(14, 18):
            for x in range(14, 18):
                pixels[x, y] = (180, 180, 180)

    def prepare(self, remove_enclosed_neutral: bool = False) -> Image.Image:
        return matte_border_background(
            self.image,
            min_luma=150,
            max_luma=255,
            max_chroma=24,
            edge_chroma=34,
            remove_enclosed_neutral=remove_enclosed_neutral,
        )

    def test_only_border_connected_matte_is_removed_by_default(self) -> None:
        result = self.prepare()
        alpha = result.getchannel("A")
        self.assertEqual(alpha.getpixel((0, 0)), 0)
        self.assertEqual(alpha.getpixel((10, 10)), 255)
        self.assertEqual(alpha.getpixel((15, 15)), 255)

    def test_enclosed_checker_pixels_can_be_removed_when_requested(self) -> None:
        result = self.prepare(remove_enclosed_neutral=True)
        alpha = result.getchannel("A")
        self.assertEqual(alpha.getpixel((0, 0)), 0)
        self.assertEqual(alpha.getpixel((10, 10)), 255)
        self.assertEqual(alpha.getpixel((15, 15)), 0)

    def test_output_keeps_an_rgba_alpha_channel_and_subject_bounds(self) -> None:
        result = self.prepare()
        self.assertEqual(result.mode, "RGBA")
        self.assertEqual(result.getchannel("A").getextrema(), (0, 255))
        self.assertGreaterEqual(result.width, 17)
        self.assertGreaterEqual(result.height, 17)


if __name__ == "__main__":
    unittest.main()
