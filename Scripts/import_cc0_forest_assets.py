"""Import the bundled CC0 forest GLBs into stable Unreal Content paths.

Run from Unreal Editor using Tools > Execute Python Script after building the
Editor target. The script imports each GLB with Unreal's Interchange-backed
AssetImportTask, then normalizes its individual StaticMesh products. It never
edits a level or deletes existing project assets.
"""

import json
import os
import re
import struct

import unreal


PROJECT_DIR = unreal.Paths.project_dir()
SOURCE_DIR = os.path.join(PROJECT_DIR, "ArtSource", "Environment", "CC0Forest")

KAYKIT_SOURCE = os.path.join(SOURCE_DIR, "forest.glb")
KENNEY_SOURCE = os.path.join(SOURCE_DIR, "nature.glb")
KAYKIT_DEST = "/Game/Aether/Environment/CC0Forest/KayKit"
KENNEY_DEST = "/Game/Aether/Environment/CC0Forest/Kenney"


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


def _normalize_name(name):
    return re.sub(r"[^a-z0-9]", "", name.lower())


def _glb_mesh_names(filename):
    if not os.path.isfile(filename):
        raise RuntimeError("CC0 forest source file is missing: {}".format(filename))

    with open(filename, "rb") as source_file:
        data = source_file.read()

    if len(data) < 20 or data[:4] != b"glTF":
        raise RuntimeError("Not a valid binary glTF (GLB) file: {}".format(filename))

    version, total_length = struct.unpack_from("<II", data, 4)
    json_length, json_type = struct.unpack_from("<II", data, 12)
    if version != 2 or json_type != 0x4E4F534A or total_length != len(data):
        raise RuntimeError("Unsupported or malformed GLB header: {}".format(filename))

    document = json.loads(data[20 : 20 + json_length].decode("utf-8").rstrip(" \0"))
    meshes = document.get("meshes", [])
    names = []
    for node in document.get("nodes", []):
        mesh_index = node.get("mesh")
        if mesh_index is None:
            continue
        name = node.get("name") or meshes[mesh_index].get("name")
        if name and name not in names:
            names.append(name)

    if not names:
        raise RuntimeError("No named mesh nodes were found in {}.".format(filename))
    return names


def _load_asset(asset_path, asset_class):
    asset_name = asset_path.rsplit("/", 1)[-1]
    for path in (asset_path, asset_path + "." + asset_name):
        asset = unreal.EditorAssetLibrary.load_asset(path)
        if isinstance(asset, asset_class):
            return asset
    return None


def _list_static_meshes(destination):
    meshes = []
    seen = set()
    for asset_path in unreal.EditorAssetLibrary.list_assets(
        destination, recursive=True, include_folder=False
    ):
        asset = unreal.EditorAssetLibrary.load_asset(asset_path)
        if isinstance(asset, unreal.StaticMesh) and asset.get_path_name() not in seen:
            meshes.append(asset)
            seen.add(asset.get_path_name())
    return meshes


def _import_glb(filename, destination):
    task = unreal.AssetImportTask()
    _set(task, "filename", filename)
    _set(task, "destination_path", destination)
    _set(task, "automated", True)
    _set(task, "replace_existing", False)
    _set(task, "save", True)
    _set(task, "async_", False, required=False)

    unreal.AssetToolsHelpers.get_asset_tools().import_asset_tasks([task])
    imported_paths = list(task.get_editor_property("imported_object_paths") or [])
    unreal.log(
        "CC0 GLB import {} -> {}".format(
            os.path.basename(filename), imported_paths
        )
    )
    return imported_paths


def _find_imported_mesh(meshes, source_name):
    wanted = _normalize_name(source_name)
    exact_or_prefixed = [
        mesh
        for mesh in meshes
        if _normalize_name(mesh.get_name()) == wanted
        or _normalize_name(mesh.get_name()).endswith(wanted)
    ]
    if len(exact_or_prefixed) == 1:
        return exact_or_prefixed[0]
    if len(exact_or_prefixed) > 1:
        exact = [mesh for mesh in exact_or_prefixed if _normalize_name(mesh.get_name()) == wanted]
        if len(exact) == 1:
            return exact[0]
        raise RuntimeError(
            "More than one imported StaticMesh matches '{}': {}".format(
                source_name, [mesh.get_path_name() for mesh in exact_or_prefixed]
            )
        )

    # Some importers add a generated prefix/suffix. Use a substring only when it
    # identifies one unambiguous product (e.g. it must not match both a pine and
    # its separate snow variant).
    candidates = [
        mesh
        for mesh in meshes
        if wanted in _normalize_name(mesh.get_name())
    ]
    return candidates[0] if len(candidates) == 1 else None


def _normalize_pack(filename, destination, asset_prefix):
    source_names = _glb_mesh_names(filename)
    unreal.EditorAssetLibrary.make_directory(destination)

    missing = [
        source_name
        for source_name in source_names
        if not _load_asset(
            destination + "/" + asset_prefix + source_name, unreal.StaticMesh
        )
    ]
    if not missing:
        unreal.log(
            "Reusing {} imported CC0 meshes in {}.".format(
                len(source_names), destination
            )
        )
        return

    _import_glb(filename, destination)
    imported_meshes = _list_static_meshes(destination)
    if not imported_meshes:
        raise RuntimeError(
            "Unreal imported no StaticMeshes from {}. Check that the UE 5.8 "
            "Interchange/glTF importer is enabled.".format(filename)
        )

    unresolved = []
    normalized_count = 0
    for source_name in source_names:
        target_name = asset_prefix + source_name
        target_path = destination + "/" + target_name
        if _load_asset(target_path, unreal.StaticMesh):
            continue

        mesh = _find_imported_mesh(imported_meshes, source_name)
        if not mesh:
            unresolved.append(source_name)
            continue

        current_path = mesh.get_path_name().rsplit(".", 1)[0]
        if current_path != target_path:
            if unreal.EditorAssetLibrary.does_asset_exist(target_path):
                # Do not overwrite or delete a pre-existing project asset.
                if _load_asset(target_path, unreal.StaticMesh):
                    continue
                unresolved.append(source_name)
                continue
            if not unreal.EditorAssetLibrary.rename_asset(current_path, target_path):
                unresolved.append(source_name)
                continue

        normalized = _load_asset(target_path, unreal.StaticMesh)
        if not normalized:
            unresolved.append(source_name)
            continue
        unreal.EditorAssetLibrary.save_loaded_asset(normalized)
        normalized_count += 1

    if unresolved:
        available_names = sorted(mesh.get_name() for mesh in imported_meshes)
        raise RuntimeError(
            "Could not normalize {} GLB mesh(es): {}. Imported StaticMeshes: {}. "
            "Check the Interchange pipeline's Combine Static Meshes option is off.".format(
                len(unresolved), unresolved, available_names
            )
        )

    unreal.log(
        "CC0 pack ready: {} source meshes, {} normalized in {}.".format(
            len(source_names), normalized_count, destination
        )
    )


def import_cc0_forest_assets():
    _normalize_pack(KAYKIT_SOURCE, KAYKIT_DEST, "SM_CC0_KayKit_")
    _normalize_pack(KENNEY_SOURCE, KENNEY_DEST, "SM_CC0_Kenney_")

    unreal.log(
        "CC0 forest assets are ready. The development world loads the green "
        "tree, bush, flower, and grass meshes automatically at runtime."
    )


import_cc0_forest_assets()
