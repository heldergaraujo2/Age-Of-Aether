# Generated 2D art provenance — sprite animation pass

**Created:** 2026-10-17 (UTC)
**Project direction:** painterly 2D/isometric fantasy game presentation
**Generation source:** Arena AI image generation, created specifically for this project
**Third-party assets:** none used
**Browser status:** four local sprite atlases are loaded and animated by the Workspace Canvas preview, including the Aether Wisp follow-up encounter
**Unreal status:** PNG source/atlas files are prepared only; no Paper2D Sprite or Flipbook has been imported or validated in Unreal.

## Animation scope and limitations

The Mage and Wild Hound began as illustrations. The first animation pass added generated key poses and the Elara NPC; the follow-up adds an Aether Wisp with Idle/Glide/Pulse states. Prepared poses have their neutral matte removed, then are packed into deterministic, transparent row-based sprite sheets. The packer places **one pose per cell**—it does not ask an image model to draw a grid. Some idle/hold cells intentionally repeat a pose with a tiny scale/anchor offset; the walk, cast, talk, run and attack rows use the distinct generated key poses. This is a short prototype animation pass, not a fully hand-drawn 8-direction production set.

## Sprite sheets

| Sheet / asset ID | Layout | Prepared Unreal import PNG | Browser PNG |
|---|---:|---|---|
| `AOA.Character.Player.Mage.SpriteSheet` | 4 columns × 3 rows, 256 × 320 cells; Idle 4 FPS, Walk 9 FPS, Cast 12 FPS | `Content/Aether/Art/ImportReady/Animations/SpriteSheets/PlayerMage_SpriteSheet.png` | `Preview/assets/spritesheets/player-mage.png` |
| `AOA.Character.NPC.Elara.SpriteSheet` | 4 columns × 2 rows, 256 × 320 cells; Idle 3.5 FPS, Talk 5 FPS | `Content/Aether/Art/ImportReady/Animations/SpriteSheets/Elara_SpriteSheet.png` | `Preview/assets/spritesheets/elara.png` |
| `AOA.Creature.FirstRegion.WildHound.SpriteSheet` | 4 columns × 3 rows, 384 × 320 cells; Idle 4 FPS, Run 10 FPS, Attack 12 FPS | `Content/Aether/Art/ImportReady/Animations/SpriteSheets/WildHound_SpriteSheet.png` | `Preview/assets/spritesheets/wild-hound.png` |
| `AOA.Creature.FirstRegion.AetherWisp.SpriteSheet` | 4 columns × 3 rows, 384 × 320 cells; Idle 4 FPS, Glide 9 FPS, Pulse 12 FPS | `Content/Aether/Art/ImportReady/Animations/SpriteSheets/AetherWisp_SpriteSheet.png` | `Preview/assets/spritesheets/aether-wisp.png` |

The exact row/frame metadata is stored in `Content/Aether/Art/ImportReady/Animations/SpriteSheets/manifest.json` and copied to `Preview/assets/spritesheets/manifest.json`. `Tools/Art/build_sprite_sheets.py` is the deterministic packer.

## Generated source poses

| Pose | Raw AI-generated source | Prepared RGBA frame |
|---|---|---|
| Mage idle (original frame) | `ArtSource/Generated/Characters/Mage/PlayerMage_Idle.png` | `Content/Aether/Art/ImportReady/Characters/Mage/PlayerMage_Idle.png` |
| Mage walk key pose | `ArtSource/Generated/Animations/PlayerMage_WalkPose.png` | `Content/Aether/Art/ImportReady/Animations/PlayerMage_Walk.png` |
| Mage spell-cast key pose | `ArtSource/Generated/Animations/PlayerMage_CastPose.png` | `Content/Aether/Art/ImportReady/Animations/PlayerMage_Cast.png` |
| Elara idle | `ArtSource/Generated/Animations/Elara_Idle.png` | `Content/Aether/Art/ImportReady/Animations/Elara_Idle.png` |
| Elara talk / quest gesture | `ArtSource/Generated/Animations/Elara_TalkPose.png` | `Content/Aether/Art/ImportReady/Animations/Elara_Talk.png` |
| Wild Hound idle (original frame) | `ArtSource/Generated/Creatures/WildHound/WildHound_Idle.png` | `Content/Aether/Art/ImportReady/Creatures/WildHound/WildHound_Idle.png` |
| Wild Hound run key pose | `ArtSource/Generated/Animations/WildHound_RunPose.png` | `Content/Aether/Art/ImportReady/Animations/WildHound_Run.png` |
| Wild Hound snapping / pounce key pose | `ArtSource/Generated/Animations/WildHound_AttackPose.png` | `Content/Aether/Art/ImportReady/Animations/WildHound_Attack.png` |
| Aether Wisp idle | `ArtSource/Generated/Animations/AetherWisp_Idle.png` | `Content/Aether/Art/ImportReady/Animations/AetherWisp_Idle.png` |
| Aether Wisp gliding key pose | `ArtSource/Generated/Animations/AetherWisp_MovePose.png` | `Content/Aether/Art/ImportReady/Animations/AetherWisp_Glide.png` |
| Aether Wisp pulse / projectile key pose | `ArtSource/Generated/Animations/AetherWisp_AttackPose.png` | `Content/Aether/Art/ImportReady/Animations/AetherWisp_Pulse.png` |
| First-region settlement house | `ArtSource/Generated/Environment/FirstRegion/SettlementHouse_01.png` | `Content/Aether/Art/ImportReady/Environment/FirstRegion/SettlementHouse_01.png` |

Sprite-sheet source atlases, built from the prepared poses, are retained outside Unreal's `Content/` scanner in `ArtSource/Generated/Animations/SpriteSheets/`.

## Generation prompts — animation poses

### Mage walk

> Make a single clean full-body game-animation frame of the EXACT same Aelwyn from the reference, not a redesign. Preserve the reference's pale silver hair and face, the same deep navy-blue hood and long robe with teal runic trim and gold clasp, the same brown boots, and especially the small floating AMBER crystal above her open right palm. Do NOT add a staff, weapon, armor, new accessories, or different crystal color. Change only the body pose: a clear walking cycle key pose with left foot stepping forward, right foot behind, slight natural shift of shoulders and robe hem, while the right hand stays raised with that same amber crystal hovering just above it. Keep all body parts, crystal and boots fully inside the frame; three-quarter isometric view toward lower-right, painterly style matching the reference. One character centered on a flat neutral medium-gray background only; no checkerboard, scenery, floor, cast shadow, text, border, other objects, contact sheet, sprite sheet, or multiple poses.

### Mage cast

> Create ONE standalone full-body animation frame of the EXACT same playable mage Aelwyn in the reference. Preserve the reference design: same pale silver hair and face, navy-blue hood and long robe with teal runic edging and gold clasp, brown boots, and the small floating amber crystal above her open right palm. Do not add a staff, weapon, armor, new accessories or change crystal color. Change only the pose to a magical casting action: right palm extended forward with the amber crystal flaring brighter, left hand lifted in a controlled spell gesture, slight turn through shoulders and a small flowing robe motion. Full silhouette from hood through both boots and crystal, three-quarter isometric view facing lower-right, painterly fantasy-game style consistent with the reference, clear foot anchor. One centered character on a perfectly flat neutral medium-gray background only; no checkerboard, scenery, ground plane, cast shadow, text, border, extra objects, other characters, collage, sprite sheet, or grid.

### Elara idle

> Create ONE standalone full-body 2D game sprite for Elara, the village frontier watchkeeper in Age of Aether. A distinct adult female ranger, warm brown skin, short chestnut braids partly tucked under a deep plum hood, muted burgundy-violet travel cloak over practical charcoal leather armor, antique-gold clasp and a small teal aether pendant, dark boots, carrying a short wooden watch-staff. Calm alert idle stance, approachable but vigilant expression. Three-quarter isometric view facing toward the lower-right, painterly fantasy game art with the same polished soft-brush style, lighting, proportions, and detail density as the attached Aelwyn reference, but clearly a different NPC. Orthographic game-sprite proportions, full silhouette from hood to boots, feet fully visible and aligned to a bottom-center anchor, centered with generous margin. Isolated on a perfectly flat neutral medium-gray background, no checkerboard, scenery, ground plane, cast shadow, text, UI, border, extra objects or characters, no sprite sheet or grid.

### Elara talk

> Create ONE standalone full-body animation frame of the EXACT same Elara NPC from the attached reference: same adult female frontier watchkeeper, warm brown skin, chestnut braids, deep plum hood and burgundy-violet travel cloak over charcoal leather armor, gold clasp and teal aether pendant, same wooden watch-staff and painterly Age of Aether fantasy game style. Preserve identity, clothes, colors, proportions and face; do not redesign her. Change only the pose into a gentle speaking / quest-giving gesture: her free hand lifts slightly with an open palm toward the player, mouth subtly open, staff still held in the other hand, cloak barely shifting. Three-quarter isometric view toward lower-right, full body and boots visible with the feet aligned to the same bottom-center anchor. Single centered character on a perfectly flat neutral medium-gray background; no checkerboard, scenery, ground plane, cast shadow, text, border, UI, extra characters, contact sheet, or sprite grid.

### Wild Hound run

> Create ONE standalone full-body animation frame of the EXACT same Wild Hound from the reference. Preserve its lean low canine build, same ash-brown/russet fur with charcoal markings, pale chest and muzzle, pointed ears, amber eyes and cyan-blue aether cracks on cheek and shoulder; same painterly Age of Aether fantasy-game style. Change only the pose to a fast ground-hugging run cycle key pose, body stretched forward with forelegs reaching and hind legs pushing off, tail streaming behind; all four legs and tail clearly readable, no cropping. Three-quarter isometric view facing lower-left, orthographic proportions and clear silhouette, paws aligned to bottom anchor. Isolated centered creature on a perfectly flat neutral medium-gray background, no checkerboard, scenery, floor, cast shadow, text, border, other objects or characters, no sprite sheet/grid/collage.

### Wild Hound attack

> Create ONE standalone full-body 2D game sprite pose of the exact same Wild Hound creature shown in the reference. Preserve its identity and design: lean low canine predator, ash-russet and charcoal fur, pale muzzle/chest, pointed ears, amber eyes, subtle cyan-blue aether markings along shoulder and cheek, same painterly fantasy game art style and scale. Change only the pose to a readable low pouncing / snapping attack: front paws braced and one lifted slightly, body stretched forward, mouth open with teeth visible, tail swept back; keep anatomy believable and the entire tail and all paws within frame. Three-quarter isometric view facing toward the lower-left, orthographic game-sprite proportions, clear silhouette, feet aligned to bottom-center anchor. Isolated centered creature on a perfectly flat neutral medium-gray background, no checkerboard, no scenery, no ground plane, no cast shadow, no text, no border, no extra objects, no sheet or grid.

### Aether Wisp idle

> One isolated game sprite of the same Aether Wisp from the reference, hovering calmly in a neutral idle pose. Preserve the teal carved mask and amber eyes, glowing turquoise orb-body, flowing translucent cyan ribbons and wispy tail, with a few tiny crystals. IMPORTANT: mouth/front area is empty, no mouth, NO projectile, NO bolt, NO attack, NO muzzle flash, NO energy blast, no pose of casting. Ribbons hang and curl gently around it rather than streaming backwards. Compact upright floating silhouette, front three-quarter isometric view facing lower-left, centered with generous margins. Painterly polished fantasy RPG sprite matching the reference. Perfectly flat neutral gray background, no floor, no cast shadow, no scenery, text, UI, extra objects, frame, collage or sprite sheet. Show exactly one calm idle pose.

### Aether Wisp glide

> Create ONE standalone full-body animation pose of the EXACT same Aether Wisp shown in the reference. Preserve its bright turquoise-blue core, dim amber eyes, dark teal mask, translucent cyan ribbons, smoky tapering tail and tiny close-orbiting crystal shards; same proportions and painterly Age of Aether game-art style. Change only the pose to a fast gliding / darting movement: body leans forward, ribbons and smoky tail stream behind, one small crystal trails slightly, clear silhouette and readable direction toward lower-left. No legs. Three-quarter isometric view, centered, all effects fully inside the frame, isolated on a perfectly flat neutral medium-gray background. No checkerboard, scenery, floor, cast shadow, text, border, UI, extra characters or objects, no sheet/grid/collage. A single movement key pose only.

### Aether Wisp pulse / attack

> Create ONE standalone full-body animation pose of the EXACT same Aether Wisp shown in the reference. Preserve its bright turquoise-blue core, dim amber eyes, dark teal mask, translucent cyan ribbons, smoky tapering tail and tiny close-orbiting crystal shards, with the same painterly Age of Aether fantasy game-art style. Change only the pose into a magical attack key pose: core flares brighter, ribbons flare outward, the wisp thrusts forward while releasing one compact blue-white aether bolt from its front; keep the projectile small and close to the body so the full silhouette remains clear. No legs. Three-quarter isometric view toward lower-left, centered, all details fully inside the frame, isolated on a perfectly flat neutral medium-gray background. No checkerboard, scenery, floor, cast shadow, text, border, UI, extra creatures, no sheet/grid/collage. One single attack pose only.

## Generation prompts — first static source pass

### Player Mage — Idle

> Single isolated production-ready character source image for Age of Aether, a premium hand-painted 2D isometric fantasy MMORPG. Full-body young male mage, three-quarter isometric view facing toward the lower-right of frame, calm readable idle stance, feet fully visible and aligned to a clear bottom-center ground anchor. Distinctive deep midnight-blue hooded mantle with layered cloth, subtle teal runic embroidery and antique gold clasp, leather boots, a small warm amber arcane crystal in one hand, expressive but restrained face, polished painterly brushwork, refined silhouette, warm cool-balanced lighting, strong readable values at small gameplay scale. Orthographic game sprite illustration, no perspective distortion. One character only, centered with generous transparent padding, crisp clean edges, isolated on a truly transparent alpha background, no cast shadow, no scenery, no text, no frame, no sheet, no extra objects. Preserve the full silhouette, both hands and both feet.

### Wild Hound — Idle

> Single isolated production-ready creature source image for Age of Aether, a premium hand-painted 2D isometric fantasy MMORPG. Full-body wild hound monster, low agile canine silhouette, three-quarter isometric view facing toward the lower-left of frame, alert idle stance with all four paws visible and clear bottom-center ground anchor. Ash-brown fur with dark charcoal markings, faint luminous blue-aether cracks along one shoulder and cheek, expressive amber eyes, believable anatomy, layered painterly fur strokes, polished hand-painted game concept, rich but disciplined detail, dramatic readable silhouette and values at small gameplay scale. Orthographic isometric game sprite illustration. One creature only, centered with generous transparent padding, crisp clean edges, isolated on a truly transparent alpha background, no cast shadow, no scenery, no text, no frame, no sheet, no extra objects. Keep all paws and tail inside the canvas.

### First Region — Settlement House

> Single isolated production-ready environment prop source image for Age of Aether, a premium hand-painted 2D isometric fantasy MMORPG. A small medieval frontier settlement house, complete readable building in three-quarter isometric view: warm pale stone-and-timber walls, steep muted blue slate roof, one little chimney, wooden front door, two warmly lit windows, a small planter, a few mossy stones at the foundation. Hand-painted painterly materials with clear stone, wood, slate, moss distinctions, attractive crafted details, coherent soft daylight from upper-left, refined readable silhouette and rich values at gameplay zoom. Orthographic isometric game prop illustration, no perspective distortion. One building only, centered with generous transparent padding, crisp clean edges, isolated on a truly transparent alpha background, no cast shadow, no ground plane, no scenery, no text, no frame, no sheet, no people, no extra structures.

## Preparation and reproduction

The image generator returned neutral RGB mattes rather than reliable alpha. The prepared individual poses below were processed with the existing border-connected matte remover; only connected near-neutral pixels are removed so enclosed neutral detail is preserved.

```bash
python Tools/Art/prepare_generated_sprite.py ArtSource/Generated/Animations/PlayerMage_WalkPose.png Content/Aether/Art/ImportReady/Animations/PlayerMage_Walk.png --min-luma 100 --max-luma 220 --max-chroma 24 --edge-chroma 36
python Tools/Art/prepare_generated_sprite.py ArtSource/Generated/Animations/PlayerMage_CastPose.png Content/Aether/Art/ImportReady/Animations/PlayerMage_Cast.png --min-luma 100 --max-luma 220 --max-chroma 24 --edge-chroma 36
python Tools/Art/prepare_generated_sprite.py ArtSource/Generated/Animations/Elara_Idle.png Content/Aether/Art/ImportReady/Animations/Elara_Idle.png --min-luma 100 --max-luma 220 --max-chroma 24 --edge-chroma 36
python Tools/Art/prepare_generated_sprite.py ArtSource/Generated/Animations/Elara_TalkPose.png Content/Aether/Art/ImportReady/Animations/Elara_Talk.png --min-luma 100 --max-luma 220 --max-chroma 24 --edge-chroma 36
python Tools/Art/prepare_generated_sprite.py ArtSource/Generated/Animations/WildHound_RunPose.png Content/Aether/Art/ImportReady/Animations/WildHound_Run.png --min-luma 100 --max-luma 220 --max-chroma 24 --edge-chroma 36
python Tools/Art/prepare_generated_sprite.py ArtSource/Generated/Animations/WildHound_AttackPose.png Content/Aether/Art/ImportReady/Animations/WildHound_Attack.png --min-luma 100 --max-luma 220 --max-chroma 24 --edge-chroma 36
python Tools/Art/prepare_generated_sprite.py ArtSource/Generated/Animations/AetherWisp_Idle.png Content/Aether/Art/ImportReady/Animations/AetherWisp_Idle.png --min-luma 100 --max-luma 220 --max-chroma 24 --edge-chroma 36
python Tools/Art/prepare_generated_sprite.py ArtSource/Generated/Animations/AetherWisp_MovePose.png Content/Aether/Art/ImportReady/Animations/AetherWisp_Glide.png --min-luma 100 --max-luma 220 --max-chroma 24 --edge-chroma 36
python Tools/Art/prepare_generated_sprite.py ArtSource/Generated/Animations/AetherWisp_AttackPose.png Content/Aether/Art/ImportReady/Animations/AetherWisp_Pulse.png --min-luma 100 --max-luma 220 --max-chroma 24 --edge-chroma 36
python Tools/Art/build_sprite_sheets.py
python Tools/Art/validate_manifest.py
python -m unittest Tools.Art.test_prepare_generated_sprite Tools.Art.test_build_sprite_sheets
```

The preparation and packing scripts create PNG source art only. They do not create `.uasset`, `.umap`, Paper2D Sprite, Flipbook, or Unreal runtime evidence. UE 5.8 import, texture settings, row-to-Flipbook creation, visual profile wiring, PIE inspection, and actual runtime acceptance remain pending.
