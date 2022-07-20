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
# TODO: remove this as a script eventually

# tests = ["0_20", "0_25", "0_30", "0_35", "0_40", "0_45", "0_50", "0_55",
#          "0_60", "0_65", "0_70", "0_75", "0_80", "0_85", "0_90", "0_95"]

# tests = [1, 2, 4, 8, 16]
tests = [1, 2, 4, 8, 16, 32, 64, 128, 256,
         512, 1024, 2048, 4096, 8192, 16384, 32768, 65536, 131072, 131072*2, 131072*4, 131072*8, 131072*16, 131072*32]

# path = "python_ci"
path = "power_law_ci_batch"

for alpha_ind in range(0, 71):
    for ind in tests:
        alpha = 0.2 + float(alpha_ind) * 0.01
        print("creating graph for alpha = " +
              str(alpha) + " and " + str(ind) + " samples")
        num_bins = 1000

        # for test in tests:
        graph_title = ""
        xaxis_title = ""
        yaxis_title = ""

        is_x_log = False
        is_y_log = False

        path_to_results = "../results/"+path+"/alpha_" + \
            str(alpha_ind) + "/" + str(ind)+"_samples.png"

        graph_title = "alpha = " + str(alpha) + " : samples = " + str(ind)
        xaxis_title = "Error"
        yaxis_title = "Frequency"

        values = []
        weights = []

        data_avg = open(
            "../results/"+path+"/alpha_" + str(alpha_ind) + "/" + str(ind) + "_samples.txt").readlines()
        siz = int(len(data_avg))
        for k in range(0, siz):
            values.append(float(data_avg[k]))
            weights.append(1.0 / float(len(data_avg)))

        plt.title(graph_title)
        plt.xlabel(xaxis_title)
        plt.ylabel(yaxis_title)

        plt.xlim(-3.0, 3.0)
        plt.ylim(0.0, 0.1)

        # plt.xscale("log", basex=2)
        # plt.yscale("log", basey=2)
        plt.grid()

        # plt.hist(values, bins=num_bins, weights=weights, range=(-2.5, 2.5))
        plt.hist(values, bins=num_bins, weights=weights, range=(-3.0, 3.0))

        # plt.legend(loc=3)
        plt.savefig(path_to_results)
        plt.clf()
