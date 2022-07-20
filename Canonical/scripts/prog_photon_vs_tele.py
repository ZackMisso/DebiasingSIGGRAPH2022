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

paths_to_data = []
# paths_to_data.append("../results/temp.txt")
paths_to_data.append("../corneria_build/equal_cost_ppm.txt")
paths_to_data.append("../corneria_build/equal_cost_safen.txt")
path_to_results = "../corneria_build/ppm_safen_perf_comp.png"
names = ["ppm", "tele_safe_n"]

graph_title = "PPM vs. Debiased PM"
xaxis_title = "cost (photons emitted)"
yaxis_title = "MSE"

is_x_log = True
is_y_log = True
conv_lines = False

# index = index + 1

# while index < len(sys.argv):
#     print(sys.argv[index].strip())

# if (sys.argv[index].strip() == "--x_log"):
is_x_log = True
# if (sys.argv[index].strip() == "--y_log"):
is_y_log = True
# if (sys.argv[index].strip() == "--conv_lines"):
conv_lines = False
# if (sys.argv[index].strip() == "--title"):
#     index = index + 1
#     txt = sys.argv[index].strip()
#     # print("txt: " + txt + " at index " + str(index))
#     while txt != "k":
#         graph_title = graph_title + txt + " "
#         index = index + 1
#         txt = sys.argv[index].strip()
#     # index = index + 1
# if (sys.argv[index].strip() == "--x_title"):
#     # index = index + 1
#     # xaxis_title = sys.argv[index].strip()
#     # index = index + 1

#     index = index + 1
#     txt = sys.argv[index].strip()
#     while txt != "k":
#         xaxis_title = xaxis_title + txt + " "
#         index = index + 1
#         txt = sys.argv[index].strip()
#     # index = index + 1
# if (sys.argv[index].strip() == "--y_title"):
#     # index = index + 1
#     # yaxis_title = sys.argv[index].strip()
#     # index = index + 1

#     index = index + 1
#     txt = sys.argv[index].strip()
#     while txt != "k":
#         yaxis_title = yaxis_title + txt + " "
#         index = index + 1
#         txt = sys.argv[index].strip()
#     # index = index + 1
# index = index + 1


def parse_all_data(paths):
    xs = []
    ys = []

    print("making paths")
    print(paths)

    for i in range(0, len(paths)):
        data = open(paths[i]).readlines()
        tmp_y = []
        tmp_x = []
        for j in range(len(data)):
            if (j % 4 == 1):  # cost
                tmp_x.append(float(data[j]))
            elif j % 4 == 3:  # mse
                tmp_y.append(float(data[j]))
        ys.append(tmp_y)
        xs.append(tmp_x)

    return xs, ys


xs, ys = parse_all_data(paths_to_data)

# print(xs)
print(len(ys))
# print(ys[0])

plt.title(graph_title)
plt.xlabel(xaxis_title)
plt.ylabel(yaxis_title)

# slopes_x = []
# slopes = []

# for i in range(1, len(xs)):
#     slopes.append((math.log2(ys[0][i]) - math.log2(ys[0][i-1])) /
#                   (math.log2(xs[i]) - math.log2(xs[i-1])))
#     slopes_x.append(xs[i])

final_xs = []
final_ys = []

start_iter = 0
# start_iter = 2048
print(len(xs))

for i in range(0, len(xs)):
    if (i >= start_iter):
        final_xs.append(xs[i])
        final_ys.append(ys[0][i])

if conv_lines:
    start_y = ys[0][start_iter]
    start_x = xs[start_iter]

    # half_slope = [start_y / math.pow(start_x, -0.5)]
    # one_slope = [start_y / math.pow(start_x, -1.0)]
    # two_slope = [start_y / math.pow(start_x, -2.0/3.0)]
    # onehalf_slope = [start_y / math.pow(start_x, -1.5)]
    half_slope = [start_y]
    one_slope = [start_y]
    two_slope = [start_y]
    onehalf_slope = [start_y]

    for i in range(1, len(final_xs)):
        half_slope.append(
            half_slope[0] / math.pow(start_x, -0.5) * math.pow(xs[i+start_iter], -0.5))
        one_slope.append(
            one_slope[0] / math.pow(start_x, -1.0) * math.pow(xs[i+start_iter], -1.0))
        two_slope.append(
            two_slope[0] / math.pow(start_x, -2.0/3.0) * math.pow(xs[i+start_iter], -2.0/3.0))
        onehalf_slope.append(
            onehalf_slope[0] / math.pow(start_x, -1.5) * math.pow(xs[i+start_iter], -1.5))

    plt.plot(final_xs, half_slope, "--k", alpha=0.2)
    plt.plot(final_xs, one_slope, "--k", alpha=0.2)
    plt.plot(final_xs, onehalf_slope, "--k", alpha=0.2)
    plt.plot(final_xs, two_slope, "--r", alpha=0.2)

plt.grid()

# for i in range(1, len(xs)):
#     one_slope.append(one_slope[i-1] * xs[i] / xs[i-1])

if (is_x_log):
    plt.xscale("log", basex=2)
if (is_y_log):
    plt.yscale("log", basey=2)

plt.plot(xs[0], ys[0], "-", label=names[0])
plt.plot(xs[1], ys[1], "-", label=names[1])

# for i in range(len(ys)):
#     plt.plot(final_xs, final_ys, "-")

plt.legend(loc=3)

print(path_to_results)

plt.savefig(path_to_results)
plt.clf()
