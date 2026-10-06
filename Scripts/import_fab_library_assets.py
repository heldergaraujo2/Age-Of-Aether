"""Import selected FabLibrary source assets into the Age of Aether UE project.

Run from Unreal Editor's Python console after enabling Editor scripting. The
script stages FBX media into Saved/ (never edits FabLibrary sources), imports
a generated grass ground texture/material, the low-poly trees/bushes, and the free
unicorn horse + its one Idle clip. The mansion GLB is opt-in because its Fab metadata
is AI-generated/AI-forbidden. Stable /Game paths are printed for runtime use.
"""

import json
import os
import re
import shutil
import unreal


PROJECT_DIR = unreal.Paths.project_dir()
FAB_DIR = os.path.join(PROJECT_DIR, "FabLibrary")
SAVED_STAGE_DIR = os.path.join(PROJECT_DIR, "Saved", "FabImportStaging")
GRASS_SOURCE = os.path.join(PROJECT_DIR, "ArtSource", "Environment", "T_GrassGround_Source.png")
GRASS_TEXTURE_DEST = "/Game/Aether/Environment/Ground/Textures"
GRASS_MATERIAL_DEST = "/Game/Aether/Environment/Ground/Materials"
GRASS_TEXTURE_PATH = GRASS_TEXTURE_DEST + "/T_GrassGround"
GRASS_MATERIAL_PATH = GRASS_MATERIAL_DEST + "/M_GrassGround"
LANDSCAPE_GRASS_MATERIAL_PATH = GRASS_MATERIAL_DEST + "/M_GrassGround_Landscape"

TREE_SOURCE = os.path.join(
    FAB_DIR,
    "Trees_and_bush_Pack_LOWPOLY-56107089",
    "fbx",
    "trees-and-bush-pack-lowp_extracted",
    "source",
    "TreesBushLOW.fbx",
)
TREE_TEXTURE_DIR = os.path.join(
    FAB_DIR,
    "Trees_and_bush_Pack_LOWPOLY-56107089",
    "fbx",
    "trees-and-bush-pack-lowp_extracted",
    "textures",
)
TREE_DEST = "/Game/Aether/Environment/Fab/TreesBush"

HORSE_SOURCE = os.path.join(
    FAB_DIR,
    "Realistic_Unicorn_Horse_Free_Version__Idle_Animation_-45f41957",
    "fbx",
    "idle_fbx_extracted",
    "Idle.fbx",
)
HORSE_TEXTURE_SOURCE_DIR = os.path.join(os.path.dirname(HORSE_SOURCE), "1")
HORSE_DEST = "/Game/Aether/Characters/FabHorse"
HORSE_TEXTURE_DEST = HORSE_DEST + "/Textures"
HORSE_MESH_PATH = HORSE_DEST + "/SK_Fab_UnicornHorse"
HORSE_IDLE_PATH = HORSE_DEST + "/Animations/A_Fab_UnicornHorse_Idle"
HORSE_MATERIAL_PATH = HORSE_DEST + "/Materials/M_Fab_UnicornHorse"

MANSION_SOURCE = os.path.join(
    FAB_DIR,
    "Horror_3D_Mansion-c3db2332",
    "glb",
    "model.glb",
)
MANSION_DEST = "/Game/Aether/Environment/Fab/Mansion"
MANSION_MESH_PATH = MANSION_DEST + "/SM_Fab_HauntedMansion"
# The Fab metadata marks this mansion as AI-generated and AI-forbidden. Keep it
# out of the project by default until its listing terms are checked by the owner.
IMPORT_AI_RESTRICTED_MANSION = False

TREE_MESHES = (
    ("bushbig2", "SM_Fab_BushBig"),
    ("bushflowersmall", "SM_Fab_BushFlowers"),
    ("bushmed", "SM_Fab_BushMedium"),
    ("bushmed2", "SM_Fab_BushMedium2"),
    ("bushsmall", "SM_Fab_BushSmall"),
    ("bushsmall2", "SM_Fab_BushSmall2"),
    ("mushbig", "SM_Fab_MushroomCluster"),
    ("pine1", "SM_Fab_Pine1"),
    ("pine2", "SM_Fab_Pine2"),
    ("tree2", "SM_Fab_Tree2"),
    ("treesmall", "SM_Fab_TreeSmall"),
)

# The FBX pack's cards need the alpha from these atlas textures wired to a
# masked opacity input. Keep the mushroom cluster on its original material.
FOLIAGE_TEXTURE_BY_MESH = {
    "bushbig2": "bushbig",
    "bushflowersmall": "bushflowers",
    "bushmed": "bushbig",
    "bushmed2": "bushbig",
    "bushsmall": "bushbig",
    "bushsmall2": "bushbig",
    "pine1": "pine1",
    "pine2": "pine2",
    "tree2": "tree2",
    "treesmall": "tree1",
}


def _set(obj, property_name, value, required=True):
    try:
        obj.set_editor_property(property_name, value)
    except Exception:
        if required:
            raise
        unreal.log_warning(
            "Optional Unreal import setting '{}' is unavailable in this engine build.".format(
                property_name
            )
        )


def _make_task(filename, destination, asset_name=None, options=None):
    if not os.path.isfile(filename):
        raise RuntimeError("Fab source file does not exist: {}".format(filename))

    task = unreal.AssetImportTask()
    _set(task, "filename", filename)
    _set(task, "destination_path", destination)
    if asset_name:
        _set(task, "destination_name", asset_name)
    _set(task, "automated", True)
    _set(task, "replace_existing", True)
    _set(task, "save", True)
    _set(task, "replace_existing_settings", True, required=False)
    if options is not None:
        _set(task, "options", options)
    return task


def _run_import(task):
    unreal.AssetToolsHelpers.get_asset_tools().import_asset_tasks([task])
    paths = list(task.get_editor_property("imported_object_paths") or [])
    unreal.log("Fab import {} -> {}".format(task.get_editor_property("filename"), paths))
    return paths


def _load_assets(imported_paths, asset_class):
    assets = []
    seen = set()
    for path in imported_paths:
        asset = unreal.EditorAssetLibrary.load_asset(path)
        if isinstance(asset, asset_class) and asset.get_path_name() not in seen:
            assets.append(asset)
            seen.add(asset.get_path_name())
    return assets


def _load_asset(expected_path, asset_class):
    asset_name = expected_path.rsplit("/", 1)[-1]
    for path in (expected_path, expected_path + "." + asset_name):
        asset = unreal.EditorAssetLibrary.load_asset(path)
        if isinstance(asset, asset_class):
            return asset
    return None


def _ensure_path(asset, destination, asset_name, asset_class):
    expected = destination + "/" + asset_name
    current = asset.get_path_name().rsplit(".", 1)[0]
    if current != expected:
        if unreal.EditorAssetLibrary.does_asset_exist(expected):
            existing = unreal.EditorAssetLibrary.load_asset(expected)
            if existing and existing != asset:
                if not unreal.EditorAssetLibrary.delete_asset(expected):
                    raise RuntimeError("Could not replace existing Fab asset at {}.".format(expected))
        if not unreal.EditorAssetLibrary.rename_asset(current, expected):
            raise RuntimeError("Could not rename Fab asset {} to {}.".format(current, expected))
    result = _load_asset(expected, asset_class)
    if not result:
        raise RuntimeError("Could not load normalized Fab asset {}.".format(expected))
    return result


def _normalize_name(name):
    return re.sub(r"[^a-z0-9]", "", name.lower())


def _stage_tree_source():
    stage_dir = os.path.join(SAVED_STAGE_DIR, "TreesBush")
    media_dir = os.path.join(stage_dir, "TreesBushLOW.fbm")
    os.makedirs(media_dir, exist_ok=True)
    staged_fbx = os.path.join(stage_dir, "TreesBushLOW.fbx")
    shutil.copy2(TREE_SOURCE, staged_fbx)
    for filename in os.listdir(TREE_TEXTURE_DIR):
        source = os.path.join(TREE_TEXTURE_DIR, filename)
        if os.path.isfile(source):
            shutil.copy2(source, os.path.join(media_dir, filename))
    return staged_fbx


def _create_masked_foliage_material(asset_name, texture):
    material_name = "M_Fab_{}_Masked".format(asset_name.replace("SM_Fab_", ""))
    material_path = TREE_DEST + "/Materials/" + material_name
    material = unreal.EditorAssetLibrary.load_asset(material_path)
    if material:
        try:
            for expression in list(material.get_editor_property("expressions")):
                unreal.MaterialEditingLibrary.delete_material_expression(material, expression)
        except Exception:
            unreal.EditorAssetLibrary.delete_asset(material_path)
            material = None

    if not material:
        unreal.EditorAssetLibrary.make_directory(TREE_DEST + "/Materials")
        material = unreal.AssetToolsHelpers.get_asset_tools().create_asset(
            material_name,
            TREE_DEST + "/Materials",
            unreal.Material,
            unreal.MaterialFactoryNew(),
        )
    if not material:
        raise RuntimeError("Could not create masked foliage material {}.".format(material_path))

    _set(material, "blend_mode", unreal.BlendMode.BLEND_MASKED)
    _set(material, "two_sided", True)
    _set(material, "opacity_mask_clip_value", 0.35, required=False)

    foliage_sample = unreal.MaterialEditingLibrary.create_material_expression(
        material, unreal.MaterialExpressionTextureSample, -460, 0
    )
    _set(foliage_sample, "texture", texture)
    unreal.MaterialEditingLibrary.connect_material_property(
        foliage_sample, "RGB", unreal.MaterialProperty.MP_BASE_COLOR
    )
    unreal.MaterialEditingLibrary.connect_material_property(
        foliage_sample, "A", unreal.MaterialProperty.MP_OPACITY_MASK
    )

    roughness = unreal.MaterialEditingLibrary.create_material_expression(
        material, unreal.MaterialExpressionConstant, -170, 220
    )
    _set(roughness, "r", 0.82)
    unreal.MaterialEditingLibrary.connect_material_property(
        roughness, "", unreal.MaterialProperty.MP_ROUGHNESS
    )

    unreal.MaterialEditingLibrary.recompile_material(material)
    unreal.EditorAssetLibrary.save_loaded_asset(material)
    return material


def _create_grass_ground_material(texture, for_landscape=False):
    material_name = "M_GrassGround_Landscape" if for_landscape else "M_GrassGround"
    material_path = LANDSCAPE_GRASS_MATERIAL_PATH if for_landscape else GRASS_MATERIAL_PATH
    material = unreal.EditorAssetLibrary.load_asset(material_path)
    if material:
        try:
            for expression in list(material.get_editor_property("expressions")):
                unreal.MaterialEditingLibrary.delete_material_expression(material, expression)
        except Exception:
            unreal.EditorAssetLibrary.delete_asset(material_path)
            material = None

    if not material:
        unreal.EditorAssetLibrary.make_directory(GRASS_MATERIAL_DEST)
        material = unreal.AssetToolsHelpers.get_asset_tools().create_asset(
            material_name,
            GRASS_MATERIAL_DEST,
            unreal.Material,
            unreal.MaterialFactoryNew(),
        )
    if not material:
        raise RuntimeError("Could not create grass material {}.".format(material_path))

    _set(material, "blend_mode", unreal.BlendMode.BLEND_OPAQUE)
    _set(material, "two_sided", False)

    if for_landscape:
        coordinates = unreal.MaterialEditingLibrary.create_material_expression(
            material, unreal.MaterialExpressionLandscapeLayerCoords, -700, 0
        )
        _set(coordinates, "mapping_scale", 200.0)
    else:
        coordinates = unreal.MaterialEditingLibrary.create_material_expression(
            material, unreal.MaterialExpressionTextureCoordinate, -700, 0
        )
        _set(coordinates, "u_tiling", 32.0)
        _set(coordinates, "v_tiling", 24.0)

    grass_sample = unreal.MaterialEditingLibrary.create_material_expression(
        material, unreal.MaterialExpressionTextureSample, -420, 0
    )
    _set(grass_sample, "texture", texture)
    uv_connected = unreal.MaterialEditingLibrary.connect_material_expressions(
        coordinates, "", grass_sample, "Coordinates"
    )
    if not uv_connected:
        # Some Editor builds expose the same input using its visible pin label.
        uv_connected = unreal.MaterialEditingLibrary.connect_material_expressions(
            coordinates, "", grass_sample, "UVs"
        )
    if not uv_connected:
        unreal.log_warning(
            "Could not connect the grass UV node to the Texture Sample; default UVs will be used for {}.".format(
                material_path
            )
        )
    else:
        unreal.log("Grass UV tiling connected for {}.".format(material_path))

    if not unreal.MaterialEditingLibrary.connect_material_property(
        grass_sample, "RGB", unreal.MaterialProperty.MP_BASE_COLOR
    ):
        raise RuntimeError("Could not connect grass texture to Base Color for {}.".format(material_path))

    roughness = unreal.MaterialEditingLibrary.create_material_expression(
        material, unreal.MaterialExpressionConstant, -150, 220
    )
    _set(roughness, "r", 0.94)
    unreal.MaterialEditingLibrary.connect_material_property(
        roughness, "", unreal.MaterialProperty.MP_ROUGHNESS
    )

    unreal.MaterialEditingLibrary.recompile_material(material)
    if not unreal.EditorAssetLibrary.save_loaded_asset(material):
        raise RuntimeError("Unreal could not save generated grass material {}.".format(material_path))
    return material


def _import_grass_ground():
    unreal.EditorAssetLibrary.make_directory(GRASS_TEXTURE_DEST)
    unreal.EditorAssetLibrary.make_directory(GRASS_MATERIAL_DEST)

    # Reuse the already imported/saved assets on reruns. In particular, do not
    # rebuild M_GrassGround while its Material Editor tab is open; the landscape
    # assignment below is independent and should not fail because that asset is in use.
    texture = _load_asset(GRASS_TEXTURE_PATH, unreal.Texture2D)
    if not texture:
        task = _make_task(GRASS_SOURCE, GRASS_TEXTURE_DEST, "T_GrassGround")
        texture = next(iter(_load_assets(_run_import(task), unreal.Texture2D)), None)
        if not texture:
            texture = _load_asset(GRASS_TEXTURE_PATH, unreal.Texture2D)
        if not texture:
            raise RuntimeError("The generated grass ground texture could not be imported.")

        texture = _ensure_path(texture, GRASS_TEXTURE_DEST, "T_GrassGround", unreal.Texture2D)
        _set(texture, "srgb", True)
        _set(texture, "compression_settings", unreal.TextureCompressionSettings.TC_DEFAULT, required=False)
        if not unreal.EditorAssetLibrary.save_loaded_asset(texture):
            raise RuntimeError("Unreal could not save the imported grass texture {}.".format(texture.get_path_name()))
    else:
        unreal.log("Reusing existing grass texture {}.".format(texture.get_path_name()))

    ground_material = _load_asset(GRASS_MATERIAL_PATH, unreal.Material)
    if not ground_material:
        ground_material = _create_grass_ground_material(texture)
    else:
        unreal.log("Reusing existing runtime grass material {}.".format(ground_material.get_path_name()))

    landscape_material = _load_asset(LANDSCAPE_GRASS_MATERIAL_PATH, unreal.Material)
    if not landscape_material:
        landscape_material = _create_grass_ground_material(texture, for_landscape=True)
    else:
        unreal.log("Reusing existing Landscape grass material {}.".format(landscape_material.get_path_name()))

    unreal.log("Textured grass ground ready: texture={} ground_material={} landscape_material={}".format(
        texture.get_path_name(), ground_material.get_path_name(), landscape_material.get_path_name()
    ))
    return ground_material, landscape_material


def _apply_grass_material_to_open_landscapes(material):
    actor_subsystem = unreal.get_editor_subsystem(unreal.EditorActorSubsystem)
    landscapes = [
        actor for actor in actor_subsystem.get_all_level_actors()
        if isinstance(actor, unreal.Landscape)
    ]
    if not landscapes:
        unreal.log_warning(
            "No Landscape actor found in the open level; the imported grass material remains available at {}.".format(
                material.get_path_name()
            )
        )
        return

    changed = []
    for landscape in landscapes:
        if landscape.get_editor_property("landscape_material") != material:
            landscape.set_editor_property("landscape_material", material)
            changed.append(landscape.get_actor_label())
            unreal.log("Assigned grass landscape material {} to Landscape '{}'".format(
                material.get_path_name(), landscape.get_actor_label()
            ))

    if changed:
        if unreal.EditorLoadingAndSavingUtils.save_current_level():
            unreal.log("Saved the open level after assigning grass to {} Landscape actor(s).".format(len(changed)))
        else:
            unreal.log_warning(
                "Grass was assigned to the Landscape, but Unreal could not save the open level. Save it with Ctrl+S."
            )
    else:
        unreal.log("The Landscape actor(s) already use the imported grass material.")


def _assign_static_material(mesh, material):
    slots = list(mesh.get_editor_property("static_materials") or [])
    if not slots:
        mesh.add_material(material)
    else:
        for material_index in range(len(slots)):
            mesh.set_material(material_index, material)
    unreal.EditorAssetLibrary.save_loaded_asset(mesh)


def _import_tree_bush_pack():
    staged_fbx = _stage_tree_source()
    options = unreal.FbxImportUI()
    _set(options, "import_mesh", True)
    _set(options, "import_as_skeletal", False)
    _set(options, "import_animations", False)
    _set(options, "import_materials", True)
    _set(options, "import_textures", True)
    _set(options, "mesh_type_to_import", unreal.FBXImportType.FBXIT_STATIC_MESH)
    static_data = options.get_editor_property("static_mesh_import_data")
    if static_data:
        _set(static_data, "combine_meshes", False, required=False)
        _set(static_data, "auto_generate_collision", False, required=False)

    task = _make_task(staged_fbx, TREE_DEST, "TreesBushLOW", options)
    imported_paths = _run_import(task)
    meshes = _load_assets(imported_paths, unreal.StaticMesh)
    if not meshes:
        raise RuntimeError(
            "No StaticMesh objects were imported from TreesBushLOW.fbx. Check the UE FBX log."
        )

    normalized = {_normalize_name(mesh.get_name()): mesh for mesh in meshes}
    unreal.log("TreesBush imported mesh names: {}".format(sorted(normalized.keys())))
    required_names = {"bushflowersmall", "pine1", "pine2", "tree2", "treesmall", "bushmed"}
    for source_alias, asset_name in TREE_MESHES:
        mesh = normalized.get(source_alias)
        if not mesh:
            candidates = [
                value
                for name, value in normalized.items()
                if name.endswith(source_alias) or source_alias in name
            ]
            mesh = candidates[0] if candidates else None
        if not mesh:
            if source_alias in required_names:
                raise RuntimeError(
                    "Could not find imported tree/bush mesh '{}' (available: {}).".format(
                        source_alias, sorted(normalized.keys())
                    )
                )
            unreal.log_warning("Optional tree/bush mesh '{}' was not imported.".format(source_alias))
            continue
        _ensure_path(mesh, TREE_DEST, asset_name, unreal.StaticMesh)

    # Import the texture atlas files explicitly as a fallback for FBX references
    # that still contain the pack author's original absolute Windows paths.
    texture_dest = TREE_DEST + "/Textures"
    unreal.EditorAssetLibrary.make_directory(texture_dest)
    diffuse_textures = {}
    for filename in os.listdir(TREE_TEXTURE_DIR):
        if not filename.lower().endswith((".png", ".jpg", ".jpeg")):
            continue
        name = os.path.splitext(filename)[0]
        texture_paths = _run_import(
            _make_task(os.path.join(TREE_TEXTURE_DIR, filename), texture_dest, "T_Fab_" + name)
        )
        textures = _load_assets(texture_paths, unreal.Texture2D)
        if not textures:
            texture = _load_asset(texture_dest + "/T_Fab_" + name, unreal.Texture2D)
            textures = [texture] if texture else []

        for texture in textures:
            if name.lower().endswith("normal"):
                _set(texture, "compression_settings", unreal.TextureCompressionSettings.TC_NORMALMAP)
                _set(texture, "srgb", False)
            elif name.lower() not in ("internal_ground_ao_texture",):
                # Keep the imported RGBA alpha channel available to masked cards.
                _set(texture, "compression_settings", unreal.TextureCompressionSettings.TC_DEFAULT)
                _set(texture, "srgb", True)
                diffuse_textures[name.lower()] = texture
            unreal.EditorAssetLibrary.save_loaded_asset(texture)

    for source_alias, asset_name in TREE_MESHES:
        asset = _load_asset(TREE_DEST + "/" + asset_name, unreal.StaticMesh)
        if not asset:
            continue
        texture_name = FOLIAGE_TEXTURE_BY_MESH.get(source_alias)
        texture = diffuse_textures.get(texture_name)
        if texture_name and texture:
            material = _create_masked_foliage_material(asset_name, texture)
            _assign_static_material(asset, material)
            unreal.log(
                "Assigned two-sided masked material {} to {} using alpha from {}".format(
                    material.get_path_name(), asset.get_path_name(), texture.get_path_name()
                )
            )
        else:
            unreal.EditorAssetLibrary.save_loaded_asset(asset)
            if texture_name:
                unreal.log_warning(
                    "Could not assign masked foliage material to {}: texture '{}' was not imported.".format(
                        asset.get_path_name(), texture_name
                    )
                )
        unreal.log("Fab vegetation ready: {}".format(asset.get_path_name()))


def _make_horse_fbx_options():
    options = unreal.FbxImportUI()
    _set(options, "import_mesh", True)
    _set(options, "import_as_skeletal", True)
    _set(options, "import_animations", True)
    _set(options, "import_materials", False)
    _set(options, "import_textures", False)
    _set(options, "mesh_type_to_import", unreal.FBXImportType.FBXIT_SKELETAL_MESH)
    return options


def _import_texture(filename, destination, asset_name):
    task = _make_task(filename, destination, asset_name)
    paths = _run_import(task)
    texture = _load_assets(paths, unreal.Texture2D)
    if texture:
        return texture[0]
    return _load_asset(destination + "/" + asset_name, unreal.Texture2D)


def _create_horse_material(base_color, normal, specular):
    material = unreal.EditorAssetLibrary.load_asset(HORSE_MATERIAL_PATH)
    if material:
        try:
            for expression in list(material.get_editor_property("expressions")):
                unreal.MaterialEditingLibrary.delete_material_expression(material, expression)
        except Exception:
            unreal.EditorAssetLibrary.delete_asset(HORSE_MATERIAL_PATH)
            material = None

    if not material:
        material = unreal.AssetToolsHelpers.get_asset_tools().create_asset(
            "M_Fab_UnicornHorse",
            HORSE_DEST + "/Materials",
            unreal.Material,
            unreal.MaterialFactoryNew(),
        )
    if not material:
        raise RuntimeError("Could not create the unicorn material.")

    base_sample = unreal.MaterialEditingLibrary.create_material_expression(
        material, unreal.MaterialExpressionTextureSample, -460, 0
    )
    _set(base_sample, "texture", base_color)
    unreal.MaterialEditingLibrary.connect_material_property(
        base_sample, "RGB", unreal.MaterialProperty.MP_BASE_COLOR
    )

    if normal:
        _set(normal, "compression_settings", unreal.TextureCompressionSettings.TC_NORMALMAP)
        _set(normal, "srgb", False)
        unreal.EditorAssetLibrary.save_loaded_asset(normal)
        normal_sample = unreal.MaterialEditingLibrary.create_material_expression(
            material, unreal.MaterialExpressionTextureSample, -460, 180
        )
        _set(normal_sample, "texture", normal)
        unreal.MaterialEditingLibrary.connect_material_property(
            normal_sample, "RGB", unreal.MaterialProperty.MP_NORMAL
        )

    if specular:
        _set(specular, "srgb", False)
        unreal.EditorAssetLibrary.save_loaded_asset(specular)
        spec_sample = unreal.MaterialEditingLibrary.create_material_expression(
            material, unreal.MaterialExpressionTextureSample, -460, 360
        )
        _set(spec_sample, "texture", specular)
        unreal.MaterialEditingLibrary.connect_material_property(
            spec_sample, "R", unreal.MaterialProperty.MP_SPECULAR
        )

    roughness = unreal.MaterialEditingLibrary.create_material_expression(
        material, unreal.MaterialExpressionConstant, -180, 300
    )
    _set(roughness, "r", 0.58)
    unreal.MaterialEditingLibrary.connect_material_property(
        roughness, "", unreal.MaterialProperty.MP_ROUGHNESS
    )
    unreal.MaterialEditingLibrary.recompile_material(material)
    unreal.EditorAssetLibrary.save_loaded_asset(material)
    return material


def _assign_skeletal_material(mesh, material):
    slots = list(mesh.get_editor_property("materials"))
    if not slots:
        slot = unreal.SkeletalMaterial()
        _set(slot, "material_interface", material)
        slots.append(slot)
    else:
        for slot in slots:
            _set(slot, "material_interface", material)
    _set(mesh, "materials", slots)
    unreal.EditorAssetLibrary.save_loaded_asset(mesh)


def _import_horse():
    task = _make_task(HORSE_SOURCE, HORSE_DEST, "SK_Fab_UnicornHorse", _make_horse_fbx_options())
    imported_paths = _run_import(task)
    meshes = _load_assets(imported_paths, unreal.SkeletalMesh)
    sequences = _load_assets(imported_paths, unreal.AnimSequence)
    mesh = meshes[0] if meshes else _load_asset(HORSE_MESH_PATH, unreal.SkeletalMesh)
    if not mesh:
        raise RuntimeError("The unicorn FBX did not produce a SkeletalMesh.")
    mesh = _ensure_path(mesh, HORSE_DEST, "SK_Fab_UnicornHorse", unreal.SkeletalMesh)

    if not sequences:
        raise RuntimeError(
            "The free unicorn FBX did not produce its advertised Idle AnimSequence. Check the FBX import log."
        )
    idle = _ensure_path(
        sequences[0],
        HORSE_DEST + "/Animations",
        "A_Fab_UnicornHorse_Idle",
        unreal.AnimSequence,
    )
    skeleton = mesh.get_editor_property("skeleton")
    idle_skeleton = idle.get_editor_property("skeleton")
    if not skeleton or not idle_skeleton or skeleton.get_path_name() != idle_skeleton.get_path_name():
        raise RuntimeError("The unicorn Idle clip does not use the imported unicorn Skeleton.")

    for directory in (HORSE_TEXTURE_DEST, HORSE_DEST + "/Materials", HORSE_DEST + "/Animations"):
        unreal.EditorAssetLibrary.make_directory(directory)
    basic_color = _import_texture(
        os.path.join(HORSE_TEXTURE_SOURCE_DIR, "BasicColor.png"),
        HORSE_TEXTURE_DEST,
        "T_Fab_UnicornHorse_BaseColor",
    )
    normal = _import_texture(
        os.path.join(HORSE_TEXTURE_SOURCE_DIR, "Normal.png"),
        HORSE_TEXTURE_DEST,
        "T_Fab_UnicornHorse_Normal",
    )
    specular = _import_texture(
        os.path.join(HORSE_TEXTURE_SOURCE_DIR, "Spec.png"),
        HORSE_TEXTURE_DEST,
        "T_Fab_UnicornHorse_Specular",
    )
    if not basic_color:
        raise RuntimeError("The unicorn base-color texture could not be imported.")
    _assign_skeletal_material(mesh, _create_horse_material(basic_color, normal, specular))
    unreal.log("Fab horse mesh ready: {}".format(mesh.get_path_name()))
    unreal.log("Fab horse Idle ready: {}".format(idle.get_path_name()))
    unreal.log_warning(
        "The Fab free horse source contains Idle only; it can be an animated stable animal, "
        "but it does not include a walk cycle for wandering NPC movement."
    )


def _import_mansion():
    metadata_path = os.path.join(os.path.dirname(MANSION_SOURCE), "metadata")
    if os.path.isfile(metadata_path):
        try:
            with open(metadata_path, "r", encoding="utf-16") as metadata_file:
                metadata = json.load(metadata_file)
            listing = metadata.get("listing", {})
            if listing.get("isAiForbidden") and not IMPORT_AI_RESTRICTED_MANSION:
                unreal.log_warning(
                    "Skipping the mansion GLB: Fab metadata marks it isAiGenerated=true and "
                    "isAiForbidden=true. Check the Fab listing terms first. If cleared, explicitly "
                    "set IMPORT_AI_RESTRICTED_MANSION=True in this script and rerun."
                )
                return
        except Exception as exc:
            unreal.log_warning("Could not parse mansion Fab metadata ({}); skipping the asset safely.".format(exc))
            return

    # The .glb is imported by Unreal's Interchange glTF factory. If no factory is
    # registered in this Editor build, this raises instead of silently skipping it.
    task = _make_task(MANSION_SOURCE, MANSION_DEST, "SM_Fab_HauntedMansion")
    imported_paths = _run_import(task)
    meshes = _load_assets(imported_paths, unreal.StaticMesh)
    mesh = meshes[0] if meshes else _load_asset(MANSION_MESH_PATH, unreal.StaticMesh)
    if not mesh:
        raise RuntimeError(
            "No StaticMesh was imported from model.glb. Enable Unreal's Interchange glTF importer, "
            "then rerun this script."
        )
    mesh = _ensure_path(mesh, MANSION_DEST, "SM_Fab_HauntedMansion", unreal.StaticMesh)
    unreal.EditorAssetLibrary.save_loaded_asset(mesh)
    unreal.log("Fab mansion ready: {}".format(mesh.get_path_name()))


def run():
    for directory in (
        GRASS_TEXTURE_DEST,
        GRASS_MATERIAL_DEST,
        TREE_DEST,
        TREE_DEST + "/Textures",
        HORSE_DEST,
        HORSE_TEXTURE_DEST,
        HORSE_DEST + "/Animations",
        HORSE_DEST + "/Materials",
        MANSION_DEST,
    ):
        unreal.EditorAssetLibrary.make_directory(directory)

    _, landscape_grass_material = _import_grass_ground()
    _apply_grass_material_to_open_landscapes(landscape_grass_material)
    _import_tree_bush_pack()
    _import_horse()
    _import_mansion()
    unreal.log_warning(
        "Fab asset import finished. The generated grass ground texture/material and "
        "TreesBush FBX are ready; the TreesBush pack provides low-poly trees/bushes; "
        "the free horse version provides one Idle clip. The mansion is skipped by default "
        "because its listing metadata is AI-generated/AI-forbidden. The small controllable_Rain "
        "GLB remains source-only because it does not contain a Niagara system."
    )


run()
