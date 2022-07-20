# import setup
import sys
import matplotlib.pyplot as plt
#import setup

import matplotlib as mpl
#import matplotlib.pyplot as plt
import matplotlib.font_manager
from matplotlib import rc
from pylab import rcParams
import numpy as np
import math

# # This is meant to be ran from within the same folder
# # TODO: remove this as a script eventually

# # tests = ["0_20", "0_25", "0_30", "0_35", "0_40", "0_45", "0_50", "0_55",
# #          "0_60", "0_65", "0_70", "0_75", "0_80", "0_85", "0_90", "0_95"]

kernels = ["uni", "cone", "quad"]
# kernels = ["uni"]
alphas = ["55"]

for alpha in alphas:
    deltas = []
    variances = []

    for kernel in kernels:
        data = open(
            "../results/photon_deltas/"+kernel+"/"+alpha+"/results.txt").readlines()

        delta = []
        var = []
        for i in range(0, int(len(data)/2)):
            delta.append(float(data[2*i]))
            var.append(float(data[2*i+1]))

        deltas.append(delta)
        variances.append(var)

    plt.xlim(1, 255)

    plt.title("Delta_j")
    plt.xlabel("j")
    plt.ylabel("Mean Delta_j")

    plt.xscale("log", basex=2)
    plt.yscale("log", basey=2)
    plt.grid()

    js = []
    for i in range(0, len(deltas[0])):
        js.append(i)

    # print(len(deltas[0]))
    # print(len(js))

    for i in range(0, len(kernels)):
        plt.plot(js, deltas[i], "-", label=kernels[i])

    plt.legend()
    plt.savefig("../results/photon_deltas/deltas_cluster.png")
    plt.clf()

    plt.xlim(1, 255)

    # VARIANCES
    plt.title("Variance of Delta_j")
    plt.xlabel("j")
    plt.ylabel("sample variance of Delta_j")

    plt.xscale("log", basex=2)
    plt.yscale("log", basey=2)
    plt.grid()

    js = []
    for i in range(0, len(deltas[0])):
        js.append(i)

    for i in range(0, len(kernels)):
        plt.plot(js, variances[i], "-", label=kernels[i])

    plt.legend()
    plt.savefig("../results/photon_deltas/variances_cluster.png")
    plt.clf()

# for ind in range(30, 91):
#     if ind >= 36 and (ind < 39 or ind > 41) and (ind < 45 or ind > 47) and (ind < 48 or ind > 50) and (ind < 72 or ind > 74):
#         # for test in tests:
#         graph_title = ""
#         xaxis_title = ""
#         yaxis_title = ""

#         is_x_log = False
#         is_y_log = False

#         path_to_results = "../results/photon_emp_equs/res_"+str(ind)+".png"

#         alpha = float(ind) / 100.0

#         graph_title = "Debiased Vs. PPM with base radius = 0.125 for alpha = " + \
#             str(alpha)
#         xaxis_title = "cost (photons emitted)"
#         yaxis_title = "MSE"

#         names = ["debiased", "ppm"]

#         deltas = []
#         delta_ind = []
#         delta_var = []

#         data_avg = open(
#             "../results/photon_emp_equs/res_"+str(ind)+".txt").readlines()
#         siz = int(len(data_avg)/2)
#         for k in range(0, siz):
#             deltas.append(math.fabs(float(data_avg[2*k])))
#             delta_var.append(float(data_avg[2*k+1]))
#             delta_ind.append(k+1)

#         conv_mean = []  # O(j^alpha-1)
#         conv_var = []  # O(j^-alpha)

#         start_x = delta_ind[0]
#         start_y_mean = deltas[0]
#         start_y_var = delta_var[0]

#         # alpha = 2.0 / 3.0

#         conv_mean = [start_y_mean]
#         conv_var = [start_y_var]
#         conv_var_for_mean = [start_y_mean]
#         conv_mean_for_var = [start_y_var]

#         for i in range(1, len(delta_ind)):
#             conv_mean.append(conv_mean[0] / math.pow(start_x,
#                                                      (alpha - 1.0)) * math.pow(delta_ind[i], (alpha - 1.0)))
#             conv_mean_for_var.append(conv_mean_for_var[0] / math.pow(start_x,
#                                                                      -alpha - 1.0) * math.pow(delta_ind[i], -alpha - 1.0))
#             conv_var.append(conv_var[0] / math.pow(start_x,
#                                                    -alpha) * math.pow(delta_ind[i], -alpha))
#             conv_var_for_mean.append(conv_var_for_mean[0] / math.pow(start_x,
#                                                                      -alpha) * math.pow(delta_ind[i], -alpha))

#         plt.title("Mean value of delta_j for alpha = " + str(alpha))
#         plt.xlabel("Delta Index: j")
#         plt.ylabel("Mean Delta")

#         plt.xscale("log", basex=2)
#         plt.yscale("log", basey=2)
#         plt.grid()

#         plt.plot(delta_ind, deltas, "r-", label="empirical")
#         plt.plot(delta_ind, conv_mean, "b--", label="expected")
#         # plt.plot(delta_ind, conv_var_for_mean, "g--", label="expected variance")

#         plt.legend(loc=3)
#         plt.savefig("../results/photon_emp_equs/res/deltas_"+str(ind)+".png")
#         plt.clf()

#         plt.title("Sample variance of delta_j for alpha = " + str(alpha))
#         plt.xlabel("Delta Index: j")
#         plt.ylabel("Sample Variance")

#         plt.xscale("log", basex=2)
#         plt.yscale("log", basey=2)
#         plt.grid()

#         plt.plot(delta_ind, delta_var, "r-", label="empirical")
#         plt.plot(delta_ind, conv_var, "b--", label="expected")
#         # plt.plot(delta_ind, conv_mean_for_var, "g--", label="expected mean")

#         plt.legend(loc=3)
#         plt.savefig("../results/photon_emp_equs/res/vars_"+str(ind)+".png")
#         plt.clf()
