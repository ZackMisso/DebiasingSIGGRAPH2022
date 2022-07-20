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

from math import atan2, degrees
import numpy as np

# Label line with line2D label data


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
        ang = degrees(atan2(dy, dx))

        # Transform to screen co-ordinates
        pt = np.array([x, y]).reshape((1, 2))
        trans_angle = ax.transData.transform_angles(np.array((ang,)), pt)[0]

    else:
        trans_angle = 0

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
    index = 0
    for line in lines:
        index = index + 1
        if (index > 6):  # hardcoded
            label = line.get_label()
            if "_line" not in label:
                labLines.append(line)
                labels.append(label)

    if xvals is None:
        xmin, xmax = ax.get_xlim()
        xvals = np.linspace(xmin, xmax, len(labLines)+2)[1:-1]
    # hardcoded because linspace does not apply to loglog plots
    xvals = [16384*8-1, 16384*8-1, 16384*8-1, 16384*8-1, 16384*8-1]

    for line, x, label in zip(labLines, xvals, labels):
        labelLine(line, x, label, align, **kwargs)


# tests = [1, 2, 4, 8, 16]
tests = [1, 2, 4, 8, 16, 32, 64, 128, 256,
         512, 1024, 2048, 4096, 8192, 16384, 32768, 65536, 131072, 131072*2, 131072*4, 131072*8, 131072*16, 131072*32]

# path = "python_ci"
path = "power_law_ci_batch"

percentiles = [10, 90]

conv_lines = True

samples = []
final_widths = []

alpha_inds = [10, 30, 50, 70]
percentiles = [45, 55]
alpha_labels = ["alpha = 0.3", "alpha = 0.5", "alpha = 0.7",
                "alpha = 0.9"]

for alpha_ind in alpha_inds:
    samples = []
    widths = []

    cnt = 0
    for ind in tests:

        is_x_log = False
        is_y_log = False

        values = []

        data_avg = open(
            "../results/"+path+"/alpha_" + str(alpha_ind) + "/" + str(ind) + "_samples.txt").readlines()
        siz = int(len(data_avg))
        for k in range(0, siz):
            values.append(float(data_avg[k]))

        values.sort()

        perc_ind_one = int(float(len(values)) * float(percentiles[0]) / 100.0)
        perc_ind_two = int(float(len(values)) * float(percentiles[1]) / 100.0)

        if cnt > 10:
            samples.append(ind)
            widths.append(
                math.fabs(values[perc_ind_two] - values[perc_ind_one]))
        cnt = cnt + 1

    final_widths.append(widths)

graph_title = "55th - 45th percentile"
xaxis_title = "Samples"
yaxis_title = "Normalized Width"

plt.title(graph_title)
plt.xlabel(xaxis_title)
plt.ylabel(yaxis_title)

plt.grid()

plt.xscale("log", basex=2)
plt.yscale("log", basey=2)

plt.xlim(2048, 262144 * 8)

# path_to_results = "../results/"+path+"/alpha_" + \
#     str(alpha_ind) + "/percentile_err_"+str(percentile)+".png"
path_to_results = "/Users/corneria/Desktop/width.png"

for index in range(0, len(final_widths)):
    tmp_start_y = final_widths[index][0]
    transl = final_widths[0][0] - tmp_start_y
    for index_2 in range(0, len(final_widths[index])):
        # final_errors[index][index_2] = final_errors[index][index_2] + transl
        final_widths[index][index_2] = final_widths[index][index_2] / tmp_start_y

index = 0
for errors in final_widths:
    # print(index)
    plt.plot(samples, errors, "-", zorder=3.0, label=alpha_labels[index])
    index = index + 1

plt.legend(loc=3)

if conv_lines:
    start_y = final_widths[0][0]
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

# plt.plot(samples, errors, "r-")
plt.savefig(path_to_results)
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

#      # path_to_results = "../results/"+path+"/alpha_" + \
#      #     str(alpha_ind) + "/percentile_avg_err.png"
#      path_to_results = "/Users/corneria/Desktop/"+path+"/alpha_" + \
#           str(alpha_ind) + "/percentile_avg_err.png"

#       cnt = 0
#        for ind in tests:

#             is_x_log = False
#             is_y_log = False

#             values = []

#             data_avg = open(
#                 "../results/"+path+"/alpha_" + str(alpha_ind) + "/" + str(ind) + "_samples.txt").readlines()
#             siz = int(len(data_avg))
#             for k in range(0, siz):
#                 values.append(float(data_avg[k]))

#             values.sort()

#             perc_ind_one = int(float(len(values)) *
#                                float(percentiles[0]) / 100.0)
#             perc_ind_two = int(float(len(values)) *
#                                float(percentiles[1]) / 100.0)
#             # print(perc_ind)
#             # print(math.fabs(values[perc_ind]))

#             if cnt > 4:
#                 err = math.fabs(values[perc_ind_one]) + \
#                     math.fabs(values[perc_ind_two])
#                 samples.append(ind)
#                 errors.append(err / 2.0)
#             cnt = cnt + 1

#         plt.plot(samples, errors, "r-", zorder=3.0)

#         if conv_lines:
#             start_y = errors[0]
#             start_x = samples[0]

#             one_slope = [start_y]
#             two_slope = [start_y]
#             three_slope = [start_y]
#             four_slope = [start_y]
#             five_slope = [start_y]

#             for i in range(1, len(samples)):
#                 one_slope.append(
#                     one_slope[0] / math.pow(start_x, -0.1) * math.pow(samples[i], -0.1))
#                 two_slope.append(
#                     one_slope[0] / math.pow(start_x, -0.2) * math.pow(samples[i], -0.2))
#                 three_slope.append(
#                     two_slope[0] / math.pow(start_x, -0.3) * math.pow(samples[i], -0.3))
#                 four_slope.append(
#                     four_slope[0] / math.pow(start_x, -0.4) * math.pow(samples[i], -0.4))
#                 five_slope.append(
#                     four_slope[0] / math.pow(start_x, -0.5) * math.pow(samples[i], -0.5))

#             plt.plot(samples, one_slope, "--k", alpha=0.2, label="-0.1")
#             plt.plot(samples, two_slope, "--k", alpha=0.2, label="-0.2")
#             plt.plot(samples, three_slope, "--k", alpha=0.2, label="-0.3")
#             plt.plot(samples, four_slope, "--k", alpha=0.2, label="-0.4")
#             plt.plot(samples, five_slope, "--k", alpha=0.2, label="-0.5")

#             labelLines(plt.gca().get_lines(), zorder=2.5, alpha=0.2)

#         plt.savefig(path_to_results)
#         plt.clf()
