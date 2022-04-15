import sys
import os
import shutil
import glob

for header in glob.glob("headers/*.py"):
    ecu = header[8:-3]
    ECU = ecu.upper()

    # empty out output
    try:
        shutil.rmtree(f"output/{ECU}")
    except:
        pass
    os.mkdir(f"output/{ECU}")

    # create files
    directories = [f"output/{ECU}/CANC", f"output/{ECU}/CANH"]
    extensions = ["c", "h"]

    for exten, direc in zip(extensions, directories):
        os.mkdir(direc)
        for file_path in glob.glob(f"utils/*.{exten}"):
            file_name = file_path[5:]
            shutil.copy(file_path, direc + file_name)

        for file_path in glob.glob(f"targets/*_{exten}.py"):
            file_name = file_path[7:-5]
            os.system(f"python parser.py {header} {file_path} > {direc}/{file_name}.{exten}")








