# LESA IRMA Matrix Sensor Executable
# By Tim Bartos

import datetime
import subprocess

create_new: bool = False

# create a identifier
now_strf: str = datetime.datetime.now().strftime("%d-%m-%Y.%H:%M")

# assume the compiled version is up-to-date
if create_new:
    output_file = open(f"Trial{now_strf}.txt", "w+")  # type: ignore
else:
    output_file = open(f"Trial_overwrite.txt", "w+")  # type: ignore

# write header
output_file.writelines(["In,Out,In-Out,DeltaT"])

time_to_record: int = 150       # rounds

for i in range(time_to_record):
    subprocess.call(["./tof"], shell=True, stdout=output_file)
output_file.close()



