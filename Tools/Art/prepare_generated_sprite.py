#!/usr/bin/env python3
"""Prepare generated checkerboard-preview art as transparent Paper2D source PNGs.

The image generator may render a checkerboard into the pixels instead of writing
an alpha channel. This script removes only the border-connected, near-neutral
background, preserving similarly colored pixels enclosed by the subject outline.

Requires Pillow: python -m pip install Pillow
"""
from __future__ import annotations

import argparse
from collections import deque
from pathlib import Path

from PIL import Image


def near_neutral_background(rgb: tuple[int, int, int], min_luma: int, max_luma: int, max_chroma: int) -> bool:
    red, green, blue = rgb
    luma = (red + green + blue) // 3
    chroma = max(red, green, blue) - min(red, green, blue)
    return min_luma <= luma <= max_luma and chroma <= max_chroma


def matte_border_background(
    image: Image.Image,
    *,
    min_luma: int,
    max_luma: int,
    max_chroma: int,
    edge_chroma: int,
    remove_enclosed_neutral: bool = False,
) -> Image.Image:
    """Return RGBA art with the connected neutral matte made transparent."""
    source = image.convert("RGB")
    width, height = source.size
    pixels = source.load()
    total = width * height

    candidate = bytearray(total)
    for y in range(height):
        row = y * width
        for x in range(width):
            candidate[row + x] = near_neutral_background(
                pixels[x, y], min_luma, max_luma, max_chroma
            )

    background = bytearray(total)
    queue: deque[int] = deque()

    def seed(index: int) -> None:
        if candidate[index] and not background[index]:
            background[index] = 1
            queue.append(index)

    for x in range(width):
        seed(x)
        seed((height - 1) * width + x)
    for y in range(height):
        seed(y * width)
        seed(y * width + width - 1)

    # Four-neighbour flood fill prevents neutral details inside a closed outline
    # (for example house roof tiles) from being removed with the matte.
    while queue:
        index = queue.popleft()
        x = index % width
        if x > 0:
            neighbour = index - 1
            if candidate[neighbour] and not background[neighbour]:
                background[neighbour] = 1
                queue.append(neighbour)
        if x + 1 < width:
            neighbour = index + 1
            if candidate[neighbour] and not background[neighbour]:
                background[neighbour] = 1
                queue.append(neighbour)
        if index >= width:
            neighbour = index - width
            if candidate[neighbour] and not background[neighbour]:
                background[neighbour] = 1
                queue.append(neighbour)
        if index + width < total:
            neighbour = index + width
            if candidate[neighbour] and not background[neighbour]:
                background[neighbour] = 1
                queue.append(neighbour)

    result = Image.new("RGBA", source.size, (0, 0, 0, 0))
    output = result.load()
    removed = 0
    for index in range(total):
        x = index % width
        y = index // width
        red, green, blue = pixels[x, y]
        if background[index] or (remove_enclosed_neutral and candidate[index]):
            removed += 1
            continue

        # Remove the first antialiased matte fringe only when it is still almost
        # neutral and directly touches the flood-filled background. The stricter
        # chroma bound protects gray/brown subject detail from edge erosion.
        touches_background = (
            (x > 0 and background[index - 1])
            or (x + 1 < width and background[index + 1])
            or (y > 0 and background[index - width])
            or (y + 1 < height and background[index + width])
        )
        if touches_background and near_neutral_background(
            (red, green, blue), max(0, min_luma - 20), max_luma, edge_chroma
        ):
            removed += 1
            continue
        output[x, y] = (red, green, blue, 255)

    alpha = result.getchannel("A")
    bounds = alpha.getbbox()
    if bounds is None:
        raise ValueError("Background removal erased the entire image; adjust thresholds.")

    # Keep transparent breathing room while retaining the artwork's foot/base anchor.
    padding = 12
    left = max(0, bounds[0] - padding)
    top = max(0, bounds[1] - padding)
    right = min(width, bounds[2] + padding)
    bottom = min(height, bounds[3] + padding)
    return result.crop((left, top, right, bottom))


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("source", type=Path, help="Generated source PNG with a visible checkerboard matte")
    parser.add_argument("destination", type=Path, help="Prepared RGBA PNG path for Unreal import")
    parser.add_argument("--min-luma", type=int, default=110)
    parser.add_argument("--max-luma", type=int, default=255)
    parser.add_argument("--max-chroma", type=int, default=24)
    parser.add_argument("--edge-chroma", type=int, default=40)
    parser.add_argument(
        "--remove-enclosed-neutral",
        action="store_true",
        help="Also remove neutral matte-colored pixels enclosed by the silhouette (use only for safe source art).",
    )
    args = parser.parse_args()

    if not args.source.is_file():
        raise SystemExit(f"Source image does not exist: {args.source}")
    if not 0 <= args.min_luma <= args.max_luma <= 255:
        raise SystemExit("Luminance thresholds must satisfy 0 <= min <= max <= 255.")
    if not 0 <= args.max_chroma <= 255 or not 0 <= args.edge_chroma <= 255:
        raise SystemExit("Chroma thresholds must be between 0 and 255.")

    with Image.open(args.source) as image:
        result = matte_border_background(
            image,
            min_luma=args.min_luma,
            max_luma=args.max_luma,
            max_chroma=args.max_chroma,
            edge_chroma=args.edge_chroma,
            remove_enclosed_neutral=args.remove_enclosed_neutral,
        )

    args.destination.parent.mkdir(parents=True, exist_ok=True)
    result.save(args.destination, format="PNG", optimize=True)
    alpha = result.getchannel("A")
    print(
        f"Prepared {args.destination}: {result.width}x{result.height}, "
        f"alpha={alpha.getextrema()}, bytes={args.destination.stat().st_size}"
    )


if __name__ == "__main__":
    main()
