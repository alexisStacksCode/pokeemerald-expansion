import os
import subprocess
import shutil


MID2AGB_DIR: str = "tools/mid2agb"
MUSIC_DIR: str = "sound/songs/midi"


music_files_to_process: list[str] = [file for file in os.listdir(MID2AGB_DIR) if os.path.splitext(file)[1] == ".mid"]


if os.name == "posix":
    for music_file in music_files_to_process:
        # Convert source MIDI to GBA AGB.
        subprocess.run([f"{MID2AGB_DIR}/mid2agb", f"{MID2AGB_DIR}/{music_file}", f"{MUSIC_DIR}/{music_file.replace(".mid", ".s")}", "-V050", "-G191"])

        # Copy source MIDI.
        shutil.copy2(f"{MID2AGB_DIR}/{music_file}", f"{MUSIC_DIR}/{music_file}")
else:
    print(f"'{__file__}' will not work on non-Linux systems, exiting...")
