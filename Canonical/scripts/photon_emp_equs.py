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

# This is meant to be ran from within the same folder
# TODO: remove this as a script eventually

# tests = ["0_20", "0_25", "0_30", "0_35", "0_40", "0_45", "0_50", "0_55",
#          "0_60", "0_65", "0_70", "0_75", "0_80", "0_85", "0_90", "0_95"]

# for test in tests:
graph_title = ""
xaxis_title = ""
yaxis_title = ""

is_x_log = False
is_y_log = False

path_to_results = "../results/photon_emp_equs/res.png"

graph_title = "Debiased Vs. PPM with base radius = 0.125"
xaxis_title = "cost (photons emitted)"
yaxis_title = "MSE"

names = ["debiased", "ppm"]

deltas = []
delta_ind = []
delta_var = []

data_avg = open(
    "../results/photon_emp_equs/res2.txt").readlines()
siz = int(len(data_avg)/2)
for k in range(0, siz):
    deltas.append(math.fabs(float(data_avg[2*k])))
    delta_var.append(float(data_avg[2*k+1]))
    delta_ind.append(k+1)

conv_mean = []  # O(j^alpha-1)
conv_var = []  # O(j^-alpha)

start_x = delta_ind[0]
start_y_mean = deltas[0]
start_y_var = delta_var[0]

alpha = 2.0 / 3.0

conv_mean = [start_y_mean]
conv_var = [start_y_var]
conv_var_for_mean = [start_y_mean]
conv_mean_for_var = [start_y_var]

for i in range(1, len(delta_ind)):
    conv_mean.append(conv_mean[0] / math.pow(start_x,
                                             alpha - 1.0) * math.pow(delta_ind[i], alpha - 1.0))
    conv_mean_for_var.append(conv_mean_for_var[0] / math.pow(start_x,
                                                             alpha - 1.0) * math.pow(delta_ind[i], alpha - 1.0))
    conv_var.append(conv_var[0] / math.pow(start_x,
                                           -alpha) * math.pow(delta_ind[i], -alpha))
    conv_var_for_mean.append(conv_var_for_mean[0] / math.pow(start_x,
                                                             -alpha) * math.pow(delta_ind[i], -alpha))


plt.title("Mean value of delta_j")
plt.xlabel("Delta Index: j")
plt.ylabel("Mean Delta")

plt.xscale("log", basex=2)
plt.yscale("log", basey=2)
plt.grid()

plt.plot(delta_ind, deltas, "r-", label="empirical")
plt.plot(delta_ind, conv_mean, "b--", label="expected")
# plt.plot(delta_ind, conv_var_for_mean, "g--", label="expected variance")

plt.legend(loc=3)
plt.savefig("../results/photon_emp_equs/deltas.png")
plt.clf()

plt.title("Sample variance of delta_j")
plt.xlabel("Delta Index: j")
plt.ylabel("Sample Variance")

plt.xscale("log", basex=2)
plt.yscale("log", basey=2)
plt.grid()

plt.plot(delta_ind, delta_var, "r-", label="empirical")
plt.plot(delta_ind, conv_var, "b--", label="expected")
# plt.plot(delta_ind, conv_mean_for_var, "g--", label="expected mean")

plt.legend(loc=3)
plt.savefig("../results/photon_emp_equs/vars.png")
plt.clf()
