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

# for test in tests:
graph_title = ""
xaxis_title = ""
yaxis_title = ""

is_x_log = False
is_y_log = False

paths_to_data = []
paths_to_data.append("../corneria_build/origin_deltas_sqrt.txt")
path_to_results = "../corneria_build/photon_deltas_sqrt.png"
names = []

graph_title = "Photon density estimation deltas"
xaxis_title = "delta index"
yaxis_title = "delta value"

is_x_log = False
is_y_log = True
conv_lines = False

# index = index + 1

# while index < len(sys.argv):
#     print(sys.argv[index].strip())

# if (sys.argv[index].strip() == "--x_log"):
is_x_log = False
# if (sys.argv[index].strip() == "--y_log"):
is_y_log = False
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
        for j in range(len(data)):
            if (j % 2 == 0 and i == 0):
                xs.append(float(data[j]))
            elif j % 2 == 1:
                tmp_y.append(float(data[j]))
        ys.append(tmp_y)

    return xs, ys


xs, ys = parse_all_data(paths_to_data)

# print(xs)
print(len(ys[0]))
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

# if conv_lines:
#     start_y = ys[0][0]

#     half_slope = [start_y]
#     one_slope = [start_y]
#     two_slope = [start_y]
#     onehalf_slope = [start_y]

#     for i in range(1, len(xs)):
#         half_slope.append(one_slope[0] * math.pow(xs[i], -0.5))
#         one_slope.append(one_slope[0] * math.pow(xs[i], -1.0))
#         two_slope.append(two_slope[0] * math.pow(xs[i], -2.0))
#         onehalf_slope.append(onehalf_slope[0] * math.pow(xs[i], -1.5))

#     plt.plot(xs, half_slope, "--k", alpha=0.2)
#     plt.plot(xs, one_slope, "--k", alpha=0.2)
#     plt.plot(xs, onehalf_slope, "--k", alpha=0.2)
#     plt.plot(xs, two_slope, "--k", alpha=0.2)

plt.grid()

# for i in range(1, len(xs)):
#     one_slope.append(one_slope[i-1] * xs[i] / xs[i-1])

if (is_x_log):
    plt.xscale("log", basex=2)
if (is_y_log):
    plt.yscale("log", basey=2)

for i in range(len(ys)):
    plt.plot(xs, ys[0], "-")

plt.legend(loc=3)

print(path_to_results)

plt.savefig(path_to_results)
print("saved fig")
plt.clf()
