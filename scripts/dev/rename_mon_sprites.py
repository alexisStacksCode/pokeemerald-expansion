import os


GRAPHICS_DIR_PATH: str = "../../graphics/pokemon"
OLD_TO_NEW_FILENAMES: dict[str, str] = {
    "anim_front.png": "front.png",
    "anim_front_gba.png": "front_gba.png",
    "anim_back.png": "back.png",
    "anim_back_gba.png": "back_gba.png",
    "anim_frontf.png": "frontf.png",
    "anim_frontf_gba.png": "frontf_gba.png",
    "anim_backf.png": "backf.png",
    "anim_backf_gba.png": "backf_gba.png",
}


for dir_path, dir_names, filenames in os.walk(GRAPHICS_DIR_PATH):
    for filename in filenames:
        if filename in OLD_TO_NEW_FILENAMES:
            old_file_path: str = os.path.join(dir_path, filename)
            new_file_path: str = old_file_path.replace(filename, OLD_TO_NEW_FILENAMES[filename])

            if os.path.exists(new_file_path):
                os.remove(old_file_path)
                continue
            os.rename(old_file_path, new_file_path)
