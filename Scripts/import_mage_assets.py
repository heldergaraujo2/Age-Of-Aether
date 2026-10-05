"""Import the repository's Mixamo-style mage FBX source and base-color texture.

Run from Unreal Editor's Python console after enabling the Python Editor Script
Plugin and Editor Scripting Utilities. The script is safe to rerun: it reimports
source assets, enforces the exact /Game paths expected by AetherCharacter, checks
that every sequence uses the mage skeleton, and reapplies the generated material.
"""

import os
import unreal


PROJECT_DIR = unreal.Paths.project_dir()
SOURCE_DIR = os.path.join(
    PROJECT_DIR,
    "Animaçoes + texturas",
    "Animaçoes + malha",
)
TEXTURE_SOURCE = os.path.join(
    PROJECT_DIR,
    "Animaçoes + texturas",
    "Textura",
    "Mago+Age+of+Aether_basecolor.jpg",
)

MAGE_DIR = "/Game/Aether/Characters/Mage"
ANIMATION_DIR = MAGE_DIR + "/Animations"
TEXTURE_DIR = MAGE_DIR + "/Textures"
MATERIAL_DIR = MAGE_DIR + "/Materials"
SKELETAL_MESH_PATH = MAGE_DIR + "/SK_Mago_AgeOfAether"
TEXTURE_PATH = TEXTURE_DIR + "/T_Mago_BaseColor"
MATERIAL_PATH = MATERIAL_DIR + "/M_Mago_AgeOfAether"

ANIMATION_SOURCES = (
    ("Parado.fbx", "A_Idle"),
    ("Caminhada.fbx", "A_Walk"),
    ("Correndo.fbx", "A_Run"),
    ("Correndo_de_costas.fbx", "A_RunBackward"),
    ("Jumping.fbx", "A_Jump"),
    ("Morte.fbx", "A_Death"),
    ("Pegando_drop.fbx", "A_Pickup"),
    ("Queda.fbx", "A_Fall"),
    ("Standing Melee Attack Downward.fbx", "A_MeleeDownward"),
)


def _set(obj, property_name, value, required=True):
    try:
        obj.set_editor_property(property_name, value)
    except Exception:
        if required:
            raise
        unreal.log_warning(
            "Optional FBX import setting '{}' is unavailable in this engine build.".format(property_name)
        )


def _make_import_task(filename, destination, asset_name, options=None):
    if not os.path.isfile(filename):
        raise RuntimeError("Source file does not exist: {}".format(filename))

    task = unreal.AssetImportTask()
    _set(task, "filename", filename)
    _set(task, "destination_path", destination)
    _set(task, "destination_name", asset_name)
    _set(task, "automated", True)
    _set(task, "replace_existing", True)
    _set(task, "save", True)
    _set(task, "replace_existing_settings", True, required=False)
    if options is not None:
        _set(task, "options", options)
    return task


def _make_fbx_options(import_mesh, import_animations, skeleton=None):
    options = unreal.FbxImportUI()
    _set(options, "import_mesh", import_mesh)
    _set(options, "import_as_skeletal", True)
    _set(options, "import_animations", import_animations)
    _set(options, "import_materials", False)
    _set(options, "import_textures", False)
    _set(options, "mesh_type_to_import", unreal.FBXImportType.FBXIT_SKELETAL_MESH)
    if skeleton is not None:
        _set(options, "skeleton", skeleton)
    return options


def _import_task(task):
    unreal.AssetToolsHelpers.get_asset_tools().import_asset_tasks([task])
    imported_paths = task.get_editor_property("imported_object_paths")
    unreal.log("Imported {} -> {}".format(task.get_editor_property("filename"), imported_paths))
    return imported_paths


def _load_imported_asset(imported_paths, asset_class, expected_path):
    asset_paths = list(imported_paths or [])
    asset_name = expected_path.rsplit("/", 1)[-1]
    asset_paths.extend((expected_path, expected_path + "." + asset_name))
    for asset_path in asset_paths:
        asset = unreal.EditorAssetLibrary.load_asset(asset_path)
        if isinstance(asset, asset_class):
            return asset
    raise RuntimeError(
        "Could not find {} at {}. Check the Unreal import log for FBX errors.".format(
            asset_class.__name__, expected_path
        )
    )


def _ensure_asset_path(asset, destination, asset_name, asset_class):
    expected_package_path = destination + "/" + asset_name
    current_package_path = asset.get_path_name().rsplit(".", 1)[0]
    if current_package_path != expected_package_path:
        if unreal.EditorAssetLibrary.does_asset_exist(expected_package_path):
            existing_asset = unreal.EditorAssetLibrary.load_asset(expected_package_path)
            if existing_asset and existing_asset != asset:
                if not unreal.EditorAssetLibrary.delete_asset(expected_package_path):
                    raise RuntimeError(
                        "Could not replace the existing asset at {}. Close any asset editor using it and rerun the import.".format(
                            expected_package_path
                        )
                    )
        if not unreal.EditorAssetLibrary.rename_asset(current_package_path, expected_package_path):
            raise RuntimeError(
                "Could not rename imported asset {} to {}.".format(
                    current_package_path, expected_package_path
                )
            )

    return _load_imported_asset(
        [],
        asset_class,
        expected_package_path,
    )


def _import_mage_mesh():
    source = os.path.join(SOURCE_DIR, "Parado.fbx")
    task = _make_import_task(
        source,
        MAGE_DIR,
        "SK_Mago_AgeOfAether",
        _make_fbx_options(import_mesh=True, import_animations=False),
    )
    imported_paths = _import_task(task)
    mesh = _load_imported_asset(
        imported_paths,
        unreal.SkeletalMesh,
        SKELETAL_MESH_PATH,
    )
    mesh = _ensure_asset_path(mesh, MAGE_DIR, "SK_Mago_AgeOfAether", unreal.SkeletalMesh)
    skeleton = mesh.get_editor_property("skeleton")
    if not skeleton:
        raise RuntimeError("The imported mage Skeletal Mesh has no Skeleton asset.")
    unreal.log("Mage mesh ready: {} using skeleton {}".format(mesh.get_path_name(), skeleton.get_path_name()))
    return mesh, skeleton


def _import_animations(skeleton):
    for filename, asset_name in ANIMATION_SOURCES:
        source = os.path.join(SOURCE_DIR, filename)
        task = _make_import_task(
            source,
            ANIMATION_DIR,
            asset_name,
            _make_fbx_options(
                import_mesh=False,
                import_animations=True,
                skeleton=skeleton,
            ),
        )
        imported_paths = _import_task(task)
        animation = _load_imported_asset(
            imported_paths,
            unreal.AnimSequence,
            ANIMATION_DIR + "/" + asset_name,
        )
        animation = _ensure_asset_path(animation, ANIMATION_DIR, asset_name, unreal.AnimSequence)
        animation_skeleton = animation.get_editor_property("skeleton")
        if not animation_skeleton or animation_skeleton.get_path_name() != skeleton.get_path_name():
            raise RuntimeError(
                "Animation {} imported against the wrong Skeleton: {} (expected {}).".format(
                    asset_name,
                    animation_skeleton.get_path_name() if animation_skeleton else "None",
                    skeleton.get_path_name(),
                )
            )
        unreal.log("Mage animation ready: {}".format(animation.get_path_name()))


def _import_base_color_texture():
    task = _make_import_task(
        TEXTURE_SOURCE,
        TEXTURE_DIR,
        "T_Mago_BaseColor",
    )
    imported_paths = _import_task(task)
    texture = _load_imported_asset(imported_paths, unreal.Texture2D, TEXTURE_PATH)
    return _ensure_asset_path(texture, TEXTURE_DIR, "T_Mago_BaseColor", unreal.Texture2D)


def _create_base_color_material(texture):
    material = unreal.EditorAssetLibrary.load_asset(MATERIAL_PATH)
    if material:
        try:
            for expression in list(material.get_editor_property("expressions")):
                unreal.MaterialEditingLibrary.delete_material_expression(material, expression)
        except Exception:
            unreal.EditorAssetLibrary.delete_asset(MATERIAL_PATH)
            material = None

    if not material:
        material = unreal.AssetToolsHelpers.get_asset_tools().create_asset(
            "M_Mago_AgeOfAether",
            MATERIAL_DIR,
            unreal.Material,
            unreal.MaterialFactoryNew(),
        )
    if not material:
        raise RuntimeError("Could not create the mage base-color material.")

    _set(material, "blend_mode", unreal.BlendMode.BLEND_OPAQUE)
    texture_sample = unreal.MaterialEditingLibrary.create_material_expression(
        material,
        unreal.MaterialExpressionTextureSample,
        -420,
        0,
    )
    if not texture_sample:
        raise RuntimeError("Could not create the base-color texture sample node.")
    _set(texture_sample, "texture", texture)
    unreal.MaterialEditingLibrary.connect_material_property(
        texture_sample,
        "RGB",
        unreal.MaterialProperty.MP_BASE_COLOR,
    )

    roughness = unreal.MaterialEditingLibrary.create_material_expression(
        material,
        unreal.MaterialExpressionConstant,
        -180,
        220,
    )
    _set(roughness, "r", 0.78)
    unreal.MaterialEditingLibrary.connect_material_property(
        roughness,
        "",
        unreal.MaterialProperty.MP_ROUGHNESS,
    )
    unreal.MaterialEditingLibrary.recompile_material(material)
    unreal.EditorAssetLibrary.save_loaded_asset(material)
    return material


def _assign_material(mesh, material):
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


def run():
    for directory in (MAGE_DIR, ANIMATION_DIR, TEXTURE_DIR, MATERIAL_DIR):
        unreal.EditorAssetLibrary.make_directory(directory)

    mesh, skeleton = _import_mage_mesh()
    _import_animations(skeleton)
    texture = _import_base_color_texture()
    material = _create_base_color_material(texture)
    _assign_material(mesh, material)

    unreal.log_warning(
        "Mage import complete. Restart the editor after compiling the updated C++ character; "
        "AetherCharacter will load SK_Mago_AgeOfAether and switch Idle/Walk/Run/Jump sequences automatically."
    )


run()
