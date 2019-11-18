"""
Utility for reading constant pictures from IRMA
"""
import datetime
import time
from typing import List

create_new: bool = False

# create a identifier
now_strf: str = datetime.datetime.now().strftime("%d-%m-%Y.%H:%M")

# assume the compiled version is up-to-date
if create_new:
    output_file = open(f"Trial{now_strf}.dbg", "w+")  # type: ignore
else:
    output_file = open(f"Trial_overwrite.dbg", "w+")  # type: ignore

start_time: float = time.time()
interval_time: float = start_time

while time.time() < start_time + 81600:
    while interval_time + 0.2 > time.time():
        continue
    interval_time += 0.2
    file = open("output-0.txt")
    lines: List[str] = file.readlines()
    lines: List[List[str]] = [[item for item in line.strip().split("\t")] for line in lines]
    lines: str = ",".join([item for sublist in lines for item in sublist]) + "\n"
    print(len(lines))
    output_file.writelines([lines])
    file.close()
