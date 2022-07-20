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
# from labellines import labelLines

from matplotlib.pyplot import figure

from math import atan2, degrees
import numpy as np

# Label line with line2D label data


def setup():
    plt.style.use("ggplot")
    textColor = '#444444'
    fontName = 'Linux Biolinium'  # only section headings change to libertine
    # mpl.rc('font', family=fontName)
    mpl.rc('font', size=30)
    mpl.rc('font', weight='light')
    # mpl.rc('text', color=textColor)
    mpl.rc('text', hinting='native')
    siz = 30.0  # 7.97
    # siz = 7.97
    pgf_with_latex = {
        "pgf.texsystem": "pdflatex",
        "text.usetex": True,
        "font.family": "serif",
        "font.serif": [],
        "font.sans-serif": [],
        "font.monospace": [],
        "axes.titlesize":  siz,
        "axes.labelsize":  siz,
        "font.size":  siz,
        "legend.fontsize":  siz,
        "xtick.labelsize":  siz,
        "ytick.labelsize":  siz,
        # "figure.figsize": figsize(0.9),     # default fig size of 0.9 textwidth
        "pgf.preamble": r"\usepackage{libertine} \usepackage[libertine]{newtxmath} \usepackage[T1]{fontenc}"
    }
    mpl.rcParams.update(pgf_with_latex)


def labelLine(line, x, label=None, align=True, **kwargs):

    ax = line.axes
    xdata = line.get_xdata()
    ydata = line.get_ydata()

    if (x < xdata[0]) or (x > xdata[-1]):
        print('x label location is outside data range!')
        return

    # Find corresponding y co-ordinate and angle of the line
    ip = 1
    for i in range(len(xdata)):
        if x < xdata[i]:
            ip = i
            break

    y = ydata[ip-1] + (ydata[ip]-ydata[ip-1]) * \
        (x-xdata[ip-1])/(xdata[ip]-xdata[ip-1])

    if not label:
        label = line.get_label()

    if align:
        # Compute the slope
        dx = xdata[ip] - xdata[ip-1]
        dy = ydata[ip] - ydata[ip-1]
        ang = degrees(atan2(dy, dx)) * 0.80  # hack

        # Transform to screen co-ordinates
        pt = np.array([x, y]).reshape((1, 2))
        trans_angle = ax.transData.transform_angles(np.array((ang,)), pt)[0]
    else:
        trans_angle = 0

    print(trans_angle)

    # Set a bunch of keyword arguments
    if 'color' not in kwargs:
        kwargs['color'] = line.get_color()

    if ('horizontalalignment' not in kwargs) and ('ha' not in kwargs):
        kwargs['ha'] = 'center'

    if ('verticalalignment' not in kwargs) and ('va' not in kwargs):
        kwargs['va'] = 'center'

    if 'backgroundcolor' not in kwargs:
        kwargs['backgroundcolor'] = ax.get_facecolor()

    if 'clip_on' not in kwargs:
        kwargs['clip_on'] = True

    if 'zorder' not in kwargs:
        kwargs['zorder'] = 2.5

    ax.text(x, y, label, rotation=trans_angle, **kwargs)


def labelLines(lines, align=True, xvals=None, **kwargs):

    ax = lines[0].axes
    labLines = []
    labels = []

    # Take only the lines which have labels other than the default ones
    cnt = 0
    for line in lines:
        if (cnt >= 4):
            label = line.get_label()
            if "_line" not in label:
                labLines.append(line)
                labels.append(label)
        cnt = cnt + 1

    if xvals is None:
        xmin, xmax = ax.get_xlim()
        xvals = np.linspace(xmin, xmax, len(labLines)+2)[1:-1]
    # hardcoded because linspace does not apply to loglog plots
    xvals = [16384*32-1, 16384*32-1, 16384*32-1, 16384*32-1, 16384*32-1]

    for line, x, label in zip(labLines, xvals, labels):
        labelLine(line, x, label, align, **kwargs)

# This is meant to be ran from within the same folder
# TODO: remove this as a script eventually

# tests = ["0_20", "0_25", "0_30", "0_35", "0_40", "0_45", "0_50", "0_55",
#          "0_60", "0_65", "0_70", "0_75", "0_80", "0_85", "0_90", "0_95"]


figure(figsize=(8, 6), dpi=80)

setup()

# tests = [1, 2, 4, 8, 16]
tests = [1, 2, 4, 8, 16, 32, 64, 128, 256,
         512, 1024, 2048, 4096, 8192, 16384, 32768, 65536, 131072, 131072*2, 131072*4, 131072*8, 131072*16, 131072*32]

# path = "python_ci"
path = "power_law_ci_batch"

conv_lines = True
path_to_results = "../results/"+path+"/alpha_final"

samples = []
errors = []

# 20, 40, 60
for alpha_ind in [70, 60, 40, 20]:
    alpha = 0.2 + float(alpha_ind) * 0.01
    print("creating graphs for alpha = " + str(alpha))

    # first do the percentiles individually

    # for percentile in percentiles:

    samples = []
    errors = []

    # graph_title = str(percentile) + " percentile : alpha = " + \
    #     str(alpha)
    xaxis_title = "Secondary samples"
    yaxis_title = "Relative MAE"

    # plt.title(graph_title)
    plt.xlabel(xaxis_title)
    plt.ylabel(yaxis_title)

    plt.grid()

    plt.xscale("log", basex=2)
    plt.yscale("log", basey=2)

    plt.xlim(16314, 131072 * 16)

    # path_to_results = "../results/"+path+"/alpha_" + \
    #     str(alpha_ind) + "/percentile_err_"+str(percentile)+".png"

    cnt = 0
    for ind in tests:

        is_x_log = False
        is_y_log = False

        values = []

        data_avg = open(
            "../results/"+path+"/alpha_" + str(alpha_ind) + "/" + str(ind) + "_samples.txt").readlines()
        siz = int(len(data_avg))
        mae = 0.0
        for k in range(0, siz):
            mae = mae + (1.0 / float(k+1)) * (abs(float(data_avg[k])) - mae)
        # values.append(mae)

        values.sort()

        # perc_ind = int(float(len(values)) * float(percentile) / 100.0)
        # print(perc_ind)
        # print(math.fabs(values[perc_ind]))

        if cnt > 13:
            samples.append(ind)
            # errors.append(math.fabs(values[perc_ind]))
            errors.append(mae)
        cnt = cnt + 1

    base = errors[0]
    for i in range(0, len(errors)):
        errors[i] = errors[i] / base

    plt.plot(samples, errors, "-", zorder=3.0, linewidth=4,
             label="$\\alpha = " + "{:.1f}".format(alpha) + "$")

    # plt.plot(X, np.arctan(a*X), label=str(a))

    # labelLines(plt.gca().get_lines(), zorder=2.5)

    # plt.plot(samples, errors, "r-")

plt.grid()
plt.legend()

if conv_lines:
    start_y = errors[0]
    start_x = samples[0]

    one_slope = [start_y]
    two_slope = [start_y]
    three_slope = [start_y]
    four_slope = [start_y]
    five_slope = [start_y]

    for i in range(1, len(samples)):
        one_slope.append(
            one_slope[0] / math.pow(start_x, -0.1) * math.pow(samples[i], -0.1))
        two_slope.append(
            one_slope[0] / math.pow(start_x, -0.2) * math.pow(samples[i], -0.2))
        three_slope.append(
            two_slope[0] / math.pow(start_x, -0.3) * math.pow(samples[i], -0.3))
        four_slope.append(
            four_slope[0] / math.pow(start_x, -0.4) * math.pow(samples[i], -0.4))
        five_slope.append(
            four_slope[0] / math.pow(start_x, -0.5) * math.pow(samples[i], -0.5))

    plt.plot(samples, one_slope, "--k", alpha=0.2, label="-0.1")
    plt.plot(samples, two_slope, "--k", alpha=0.2, label="-0.2")
    plt.plot(samples, three_slope, "--k", alpha=0.2, label="-0.3")
    plt.plot(samples, four_slope, "--k", alpha=0.2, label="-0.4")
    plt.plot(samples, five_slope, "--k", alpha=0.2, label="-0.5")

    labelLines(plt.gca().get_lines(), zorder=2.5, alpha=0.2)

# plt.tight_layout()
plt.tight_layout(rect=[-0.05, -0.07, 1.029, 1.036])

plt.savefig(path_to_results + ".png")
plt.savefig(path_to_results + ".pdf")
plt.savefig(path_to_results + ".pgf")
plt.clf()

# TODO: create the log figure

logx = []

for i in range(0, 1000):
    logx.append(float(i+1) / 1000.0)

alpha_4 = []
alpha_6 = []
alpha_8 = []
alpha_9 = []

for i in range(0, len(logx)):
    alpha_4.append(1.0 / pow(logx[i], 0.4))
    alpha_6.append(1.0 / pow(logx[i], 0.6))
    alpha_8.append(1.0 / pow(logx[i], 0.8))
    alpha_9.append(1.0 / pow(logx[i], 0.9))

plt.xlabel("$x$")
# plt.ylabel("$\\frac{1}{x^\\alpha}$")
h = plt.ylabel('$\\frac{1}{x^\\alpha}$', labelpad=10)
h.set_rotation(0)

y2 = []
for i in range(0, len(logx)):
    y2.append(1)

# plt.xscale("log", basex=2)
# plt.yscale("log", basey=2)
# plt.ylim(1, 32768/64)
# plt.xlim(0.001, 1.1)

plt.ylim(1, 100)
plt.xlim(0.001, 0.1)
plt.grid()


plt.plot(logx, alpha_9, "-",
         label="$\\alpha = 0.9$", linewidth=4)

plt.fill_between(logx, alpha_9, y2, alpha=0.3)

plt.plot(logx, alpha_8, "-",
         label="$\\alpha = 0.8$", linewidth=4)

plt.fill_between(logx, alpha_8, y2, alpha=0.3)

plt.plot(logx, alpha_6, "-",
         label="$\\alpha = 0.6$", linewidth=4)

plt.fill_between(logx, alpha_6, y2, alpha=0.3)

plt.plot(logx, alpha_4, "-",
         label="$\\alpha = 0.4$", linewidth=4)

plt.fill_between(logx, alpha_4, y2, alpha=0.3)

plt.grid()
plt.legend()

plt.tight_layout(rect=[-0.06, -0.075, 1.05, 1.06])
# plt.tight_layout()

plt.savefig(path_to_results + "_log.pgf")
plt.savefig(path_to_results + "_log.png")
plt.savefig(path_to_results + "_log.pdf")

plt.clf()

# then do their average
# print("hello")

# if len(percentiles) == 2:
#     # print("hello")
#     samples = []
#     errors = []

#     graph_title = "average of percentiles : alpha = " + \
#         str(alpha)
#     xaxis_title = "Samples"
#     yaxis_title = "Error"

#     plt.title(graph_title)
#     plt.xlabel(xaxis_title)
#     plt.ylabel(yaxis_title)

#     plt.grid()

#     plt.xscale("log", basex=2)
#     plt.yscale("log", basey=2)

#     plt.xlim(32, 262144)

#     # path_to_results = "../results/"+path+"/alpha_" + \
#     #     str(alpha_ind) + "/percentile_avg_err.png"
#     path_to_results = "../results/"+path+"/alpha_" + \
#         str(alpha_ind) + "/percentile_avg_err.png"

#     cnt = 0
#     for ind in tests:

#         is_x_log = False
#         is_y_log = False

#         values = []

#         data_avg = open(
#             "../results/"+path+"/alpha_" + str(alpha_ind) + "/" + str(ind) + "_samples.txt").readlines()
#         siz = int(len(data_avg))
#         for k in range(0, siz):
#             values.append(float(data_avg[k]))

#         values.sort()

#         perc_ind_one = int(float(len(values)) *
#                            float(percentiles[0]) / 100.0)
#         perc_ind_two = int(float(len(values)) *
#                            float(percentiles[1]) / 100.0)
#         # print(perc_ind)
#         # print(math.fabs(values[perc_ind]))

#         if cnt > 4:
#             err = math.fabs(values[perc_ind_one]) + \
#                 math.fabs(values[perc_ind_two])
#             samples.append(ind)
#             errors.append(err / 2.0)
#         cnt = cnt + 1

#     plt.plot(samples, errors, "r-", zorder=3.0)

#     if conv_lines:
#         start_y = errors[0]
#         start_x = samples[0]

#         one_slope = [start_y]
#         two_slope = [start_y]
#         three_slope = [start_y]
#         four_slope = [start_y]
#         five_slope = [start_y]

#         for i in range(1, len(samples)):
#             one_slope.append(
#                 one_slope[0] / math.pow(start_x, -0.1) * math.pow(samples[i], -0.1))
#             two_slope.append(
#                 one_slope[0] / math.pow(start_x, -0.2) * math.pow(samples[i], -0.2))
#             three_slope.append(
#                 two_slope[0] / math.pow(start_x, -0.3) * math.pow(samples[i], -0.3))
#             four_slope.append(
#                 four_slope[0] / math.pow(start_x, -0.4) * math.pow(samples[i], -0.4))
#             five_slope.append(
#                 four_slope[0] / math.pow(start_x, -0.5) * math.pow(samples[i], -0.5))

#         plt.plot(samples, one_slope, "--k", alpha=0.2, label="-0.1")
#         plt.plot(samples, two_slope, "--k", alpha=0.2, label="-0.2")
#         plt.plot(samples, three_slope, "--k", alpha=0.2, label="-0.3")
#         plt.plot(samples, four_slope, "--k", alpha=0.2, label="-0.4")
#         plt.plot(samples, five_slope, "--k", alpha=0.2, label="-0.5")

#         labelLines(plt.gca().get_lines(), zorder=2.5, alpha=0.2)

#     plt.savefig(path_to_results)
#     plt.clf()
