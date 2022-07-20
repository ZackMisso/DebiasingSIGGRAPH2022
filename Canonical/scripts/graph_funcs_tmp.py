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


# TODO: fix all of this so that it works
# def setup():
#     plt.style.use("ggplot")
#     textColor = '#444444'
#     fontName = 'Linux Biolinium'
#     #mpl.rc('font', family=fontName)
#     mpl.rc('font', size=10)
#     mpl.rc('font', weight='light')
#     #mpl.rc('text', color=textColor)
#     mpl.rc('text', hinting='native')
#     pgf_with_latex = {
#         "pgf.texsystem": "pdflatex",
#         "text.usetex": True,
#         "font.family": "serif",
#         "font.serif": [],
#         "font.sans-serif": [],
#         "font.monospace": [],
#         "axes.titlesize":  7.97,
#         "axes.labelsize":  7.97,
#         "font.size":  7.97,
#         "legend.fontsize":  7.97,
#         "xtick.labelsize":  7.97,
#         "ytick.labelsize":  7.97,
#         # "figure.figsize": figsize(0.9),     # default fig size of 0.9 textwidth
#         "pgf.preamble": r"\usepackage[utf8x]{inputenc} \usepackage[T1]{fontenc}"
#     }
#     mpl.rcParams.update(pgf_with_latex)


# setup()
# setup.setup()

graph_title = ""
xaxis_title = ""
yaxis_title = ""

is_x_log = False
is_y_log = False
conv_lines = False

num_data = int(sys.argv[1])
index = 2
paths_to_data = []
names = []
for i in range(0, num_data):
    paths_to_data.append(sys.argv[index])
    index = index + 1
    names.append(sys.argv[index])
    index = index + 1

path_to_results = sys.argv[index]
index = index + 1

while index < len(sys.argv):
    print(sys.argv[index].strip())

    if (sys.argv[index].strip() == "--x_log"):
        is_x_log = True
    if (sys.argv[index].strip() == "--y_log"):
        is_y_log = True
    if (sys.argv[index].strip() == "--conv_lines"):
        conv_lines = True
    if (sys.argv[index].strip() == "--title"):
        index = index + 1
        txt = sys.argv[index].strip()
        # print("txt: " + txt + " at index " + str(index))
        while txt != "k":
            graph_title = graph_title + txt + " "
            index = index + 1
            txt = sys.argv[index].strip()
        # index = index + 1
    if (sys.argv[index].strip() == "--x_title"):
        # index = index + 1
        # xaxis_title = sys.argv[index].strip()
        # index = index + 1

        index = index + 1
        txt = sys.argv[index].strip()
        while txt != "k":
            xaxis_title = xaxis_title + txt + " "
            index = index + 1
            txt = sys.argv[index].strip()
        # index = index + 1
    if (sys.argv[index].strip() == "--y_title"):
        # index = index + 1
        # yaxis_title = sys.argv[index].strip()
        # index = index + 1

        index = index + 1
        txt = sys.argv[index].strip()
        while txt != "k":
            yaxis_title = yaxis_title + txt + " "
            index = index + 1
            txt = sys.argv[index].strip()
        # index = index + 1
    index = index + 1


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

print(xs)
print(len(ys))
print(ys[0])

plt.title(graph_title)
plt.xlabel(xaxis_title)
plt.ylabel(yaxis_title)


if conv_lines:
    start_y = ys[0][0]

    half_slope = [start_y]
    one_slope = [start_y]
    two_slope = [start_y]
    onehalf_slope = [start_y]

    for i in range(1, len(xs)):
        half_slope.append(one_slope[0] * math.pow(xs[i], -0.5))
        one_slope.append(one_slope[0] * math.pow(xs[i], -1.0))
        two_slope.append(two_slope[0] * math.pow(xs[i], -2.0))
        onehalf_slope.append(onehalf_slope[0] * math.pow(xs[i], -1.5))

    plt.plot(xs, half_slope, "--k", alpha=0.2)
    plt.plot(xs, one_slope, "--k", alpha=0.2)
    plt.plot(xs, onehalf_slope, "--k", alpha=0.2)
    plt.plot(xs, two_slope, "--k", alpha=0.2)

plt.grid()

# for i in range(1, len(xs)):
#     one_slope.append(one_slope[i-1] * xs[i] / xs[i-1])

if (is_x_log):
    plt.xscale("log", basex=2)
if (is_y_log):
    plt.yscale("log", basey=2)

for i in range(len(ys)):
    plt.plot(xs, ys[i], "-", label=names[i])

plt.legend(loc=3)

plt.savefig(path_to_results)
