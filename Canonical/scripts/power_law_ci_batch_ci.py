# import setup
import sys
import matplotlib.pyplot as plt
# import setup

import matplotlib as mpl
# import matplotlib.pyplot as plt
import matplotlib.font_manager
from matplotlib import rc
from pylab import rcParams
import numpy as np
import math


# This is meant to be ran from within the same folder

# tests = [1, 2, 4, 8, 16]
tests = [1, 2, 4, 8, 16, 32, 64, 128, 256,
         512, 1024, 2048, 4096, 8192, 16384, 32768, 65536, 131072, 131072*2, 131072*4, 131072*8, 131072*16, 131072*32]

# path = "python_ci"
path = "power_law_ci_batch"

dist_1_0 = []
dist_0_5 = []
dist_0_1 = []
dist_0_01 = []

for alpha_ind in range(60, 71):
    xaxis = []
    dist_1_0 = []
    dist_0_5 = []
    dist_0_1 = []
    dist_0_01 = []

    # for test in tests:
    graph_title = ""
    xaxis_title = ""
    yaxis_title = ""

    is_x_log = False
    is_y_log = False

    path_to_results = "../results/"+path+"/alpha_" + \
        str(alpha_ind) + "/epsilons.png"

    for ind in tests:
        dist_1_0_cnt = 0.0
        dist_0_5_cnt = 0.0
        dist_0_1_cnt = 0.0
        dist_0_01_cnt = 0.0

        alpha = 0.2 + float(alpha_ind) * 0.01
        print("creating graph for alpha = " +
              str(alpha) + " and " + str(ind) + " samples")

        values = []

        data_avg = open(
            "../results/"+path+"/alpha_" + str(alpha_ind) + "/" + str(ind) + "_samples.txt").readlines()
        siz = int(len(data_avg))
        for k in range(0, siz):
            values.append(float(data_avg[k]))

        for val in values:
            if math.fabs(val) < 1.0:
                dist_1_0_cnt = dist_1_0_cnt + 1.0
            if math.fabs(val) < 0.5:
                dist_0_5_cnt = dist_0_5_cnt + 1.0
            if math.fabs(val) < 0.1:
                dist_0_1_cnt = dist_0_1_cnt + 1.0
            if math.fabs(val) < 0.01:
                dist_0_01_cnt = dist_0_01_cnt + 1.0

        dist_1_0_cnt = max(dist_1_0_cnt, 1.0)
        dist_0_5_cnt = max(dist_0_5_cnt, 1.0)
        dist_0_1_cnt = max(dist_0_1_cnt, 1.0)
        dist_0_01_cnt = max(dist_0_01_cnt, 1.0)

        xaxis.append(ind)
        dist_1_0.append(dist_1_0_cnt / float(len(values)))
        dist_0_5.append(dist_0_5_cnt / float(len(values)))
        dist_0_1.append(dist_0_1_cnt / float(len(values)))
        dist_0_01.append(dist_0_01_cnt / float(len(values)))

    graph_title = "Percent trials within epsilons of true mean"
    xaxis_title = "secondary samples"
    yaxis_title = "percent"

    plt.title(graph_title)
    plt.xlabel(xaxis_title)
    plt.ylabel(yaxis_title)

    plt.xscale("log", basex=2)
    plt.yscale("log", basey=2)
    plt.grid()

    plt.plot(xaxis, dist_1_0, "-", label="epsilon = 1.0")
    plt.plot(xaxis, dist_0_5, "-", label="epsilon = 0.5")
    plt.plot(xaxis, dist_0_1, "-", label="epsilon = 0.1")
    plt.plot(xaxis, dist_0_01, "-", label="epsilon = 0.01")

    plt.legend()

    # plt.legend(loc=3)
    plt.savefig(path_to_results)
    plt.clf()
