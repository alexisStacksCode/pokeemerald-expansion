from typing import Any
import os
import re


SPECIES_INFO_PATHS: list[str] = [
    "src/data/pokemon/species_info.h",
    "src/data/pokemon/species_info/gen_1_families.h",
    "src/data/pokemon/species_info/gen_2_families.h",
    "src/data/pokemon/species_info/gen_3_families.h",
    "src/data/pokemon/species_info/gen_4_families.h",
    "src/data/pokemon/species_info/gen_5_families.h",
    "src/data/pokemon/species_info/gen_6_families.h",
    "src/data/pokemon/species_info/gen_7_families.h",
    "src/data/pokemon/species_info/gen_8_families.h",
    "src/data/pokemon/species_info/gen_9_families.h",
]


contents: dict[str, str] = {}


for species_info in SPECIES_INFO_PATHS:
    with open(species_info, "rt", encoding="utf-8") as file:
        content: str = file.read()
        front_anim_id_occurrences: list[Any] = re.findall(r"\.frontAnimId = ?.*,?\n        ?", content)
        back_anim_id_occurrences: list[Any] = re.findall(r"\.backAnimId = ?.*,?\n        ?", content)
        front_anim_frames_occurrences: list[Any] = re.findall(r"\.frontAnimFrames\s?=\s?.*?\.", content, re.DOTALL)

        for occurrence in front_anim_id_occurrences:
            content = content.replace(occurrence, "")
        for occurrence in back_anim_id_occurrences:
            content = content.replace(occurrence, "")
        for occurrence in front_anim_frames_occurrences:
            content = content.replace(occurrence, ".")
        contents[species_info] = content

    with open(species_info, "wt", encoding="utf-8") as file:
        file.write(contents[species_info])
