import os

files: list[str] = os.listdir(".")
aif_files: list[str] = [file for file in files if os.path.splitext(file)[1] == ".aif"]

for filename in aif_files:
    if os.path.isfile(filename):
        os.remove(filename)
