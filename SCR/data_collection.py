# LESA IRMA Matrix Sensor Executable
# By Tim Bartos

import datetime
import subprocess
import time

create_new: bool = True

# create a identifier
now_strf: str = datetime.datetime.now().strftime("%d-%m-%Y.%H:%M")

# assume the compiled version is up-to-date
if create_new:
    output_file = open(f"Trial{now_strf}.csv", "w+")  # type: ignore
else:
    output_file = open(f"Trial_overwrite.csv", "w+")  # type: ignore

# write header
output_file.write(f"{str(time.time())}\r\n")
output_file.write("In,Out,In-Out,DeltaT\r\n")
subprocess.call(["./tof"], shell=True, stdout=output_file)
output_file.write(f"{str(time.time())}\r\n")

output_file.close()


