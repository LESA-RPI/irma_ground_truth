import matplotlib as stupid
from matplotlib import cm
from matplotlib.colors import ListedColormap, LinearSegmentedColormap
stupid.use('tkAgg')
import matplotlib.pyplot as plt
import time
# from matplotlib.animation import FuncAnimation
from typing import List

import numpy as np
# import PIL
import os

viridis = cm.get_cmap('viridis', 12)
newcolors = viridis(np.linspace(0,1,256))
newcmp = ListedColormap(newcolors)

current_path = os.path.dirname(os.path.abspath(__file__))
PDFs = os.path.join(current_path, "imgs")
if not os.path.exists(PDFs):
    os.mkdir(PDFs)

# timing
TIMING_PERIOD: float = 0.2  # s

def read_data() -> List[int]:
    file = open("output-0.txt")
    lines: List[str] = file.readlines()
    lines: List[int] = [[int(item) for item in line.strip().split("\t")] for line in lines]
    file.close()
    return lines

def generate_plot(data: List, name: str, cmap):
    fig, axs = plt.subplots(1, figsize=(4, 5), constrained_layout=True)
    psm = axs.pcolormesh(data, cmap=cmap, rasterized=True, vmin=0, vmax=3000)
    fig.colorbar(psm, ax=axs)
    plt.savefig(name)
    plt.close()


if __name__ == "__main__":
    time_index: float = time.time()
    count: int = 0
    while True:
        # pull data
        array: List = read_data()
        generate_plot(data=array, name=f"imgs/index{count}.png", cmap=viridis)
        print(f"time.time() < time_index + TIMING_PERIOD: {time.time() < time_index + TIMING_PERIOD}")
        while time.time() < time_index + TIMING_PERIOD:
            continue
        time_index += TIMING_PERIOD
        count += 1
