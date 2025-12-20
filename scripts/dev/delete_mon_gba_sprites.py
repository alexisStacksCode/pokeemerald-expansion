import os


GRAPHICS_DIR_PATH: str = "../../graphics/pokemon"
TARGET_FILENAMES: set[str] = {
    "front_gba.png",
    "frontf_gba.png",
    "back_gba.png",
    "backf_gba.png",
    "anim_front_gba.png",
    "anim_frontf_gba.png",
    "anim_back_gba.png",
    "anim_backf_gba.png",
}


for dir_path, dir_names, filenames in os.walk(GRAPHICS_DIR_PATH):
    for filename in filenames:
        if filename in TARGET_FILENAMES:
            os.remove(os.path.join(dir_path, filename))
