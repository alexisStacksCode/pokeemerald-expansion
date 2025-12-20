import os

from PIL import Image
from colorama import Fore


GRAPHICS_DIR_PATH: str = "../../graphics/pokemon"
TARGET_FILENAMES: set[str] = {
    "front.png",
    "front_gba.png",
    "back.png",
    "back_gba.png",
    "frontf.png",
    "frontf_gba.png",
    "backf.png",
    "backf_gba.png",
    "anim_front.png",
    "anim_front_gba.png",
    "anim_back.png",
    "anim_back_gba.png",
    "anim_frontf.png",
    "anim_frontf_gba.png",
    "anim_backf.png",
    "anim_backf_gba.png",
}


def is_number_even(number: int) -> bool:
    return number % 2 == 0


for dir_path, dir_names, filenames in os.walk(GRAPHICS_DIR_PATH):
    if dir_path.endswith("egg"):
        continue

    for filename in filenames:
        if filename in TARGET_FILENAMES:
            file_path: str = os.path.join(dir_path, filename)

            image: Image.Image = Image.open(file_path)
            width, height = image.size

            if not is_number_even(width):
                print(f"{Fore.LIGHTRED_EX}'{file_path}' does not have even width (got {width})")
                continue
            if not is_number_even(height):
                print(f"{Fore.LIGHTRED_EX}'{file_path}' does not have even height (got {height})")
                continue
            if width > height or height != width * 2.0:
                print(f"{Fore.LIGHTRED_EX}'{file_path}' does not follow size rules (got {width}x{height})")
                continue
            if width == height:
                print(f"{Fore.LIGHTYELLOW_EX}'{file_path}' is already square")
                continue
            crop: Image.Image = image.crop((0.0, 0.0, width, height * 0.5))
            crop.save(file_path)
print(Fore.RESET, end=None)
